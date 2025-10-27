#include "game/game.h"
#include "core/logger.h"
#include "renderer/ogl.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"



namespace ark
{
	bool Game::Init()
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		//mpWindow = SDL_CreateWindow("Arkanoid - SDL2", mWindowW, mWindowH, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		mpWindow = SDL_CreateWindow("Arkanoid - SDL2", mWindowW, mWindowH, SDL_WINDOW_OPENGL);
		if (mpWindow == nullptr)
		{
			ARK_ERROR("SDL_CreateWindow failed: {}", SDL_GetError());
			return false;
		}

		mpRenderer.reset(Renderer::Create(mpWindow));
		mpRenderer->ResizeViewport(mWindowW, mWindowH);
		mpRenderer->SetBackgroundColor(glm::vec4(30.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f, 1.0f));
		mpPaddle = std::make_unique<Paddle>(mWindowW / 2 - 60, mWindowH - 40, 120, 16);
		mpBall = std::make_unique<Ball>(mWindowW / 2.0f, mWindowH - 60.0f, 8.0f);
		RestartLevel();
		return true;
	}

	void Game::Run()
	{
		Uint64 last = SDL_GetPerformanceCounter();
		while (mState != State::Closed)
		{
			const Uint64 now = SDL_GetPerformanceCounter();
			const float dt = float(now - last) / float(SDL_GetPerformanceFrequency());
			last = now;
			HandleInput();
			if (mState == State::Started)
				UpdateGame(dt);
			UpdateUI();
			Render();
		}
	}

	void Game::Terminate()
	{
		mpRenderer.reset();
		SDL_DestroyWindow(mpWindow);
	}

	void Game::RestartLevel()
	{
		mBricks.clear();
		const int rows = 5;
		const int cols = 10;
		const int bw = 64;
		const int bh = 24;
		const int startX = (mWindowW - cols * bw) / 2;
		const int startY = 60;
		for (int r = 0; r < rows; ++r)
		{
			for (int c = 0; c < cols; ++c)
			{
				Brick::Color col = (r % 3 == 0 ? Brick::Color::Red : (r % 3 == 1 ? Brick::Color::Green : Brick::Color::Blue));
				const int pts = (r % 3 == 0 ? 100 : (r % 3 == 1 ? 50 : 30));
				mBricks.emplace_back(std::make_unique<Brick>(startX + c * bw, startY + r * bh, bw - 4, bh - 4, col, pts));
			}
		}

		mLives = 3;
		mScore = 0;
		mBricksLeft = cols * rows;

		mpPaddle->Reset(float(mWindowW / 2 - 60), float(mWindowH - 40));
		ResetBall();
	}

	void Game::ResetBall()
	{
		mpBall->Reset(mpPaddle->GetCenterX(), mpPaddle->GetTop() - mpBall->GetRadius());
	}

	void Game::HandleInput()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				mState = State::Closed;
				break;

			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				if (event.window.windowID == SDL_GetWindowID(mpWindow))
					mState = State::Closed;
				break;

			//case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			//	mpRenderer->ResizeViewport(event.window.data1, event.window.data2);
			//	break;

			case SDL_EVENT_KEY_DOWN:
				HandleKeyboardEvent(event.key);
				break;

			case SDL_EVENT_KEY_UP:
				if ((event.key.scancode == SDL_SCANCODE_LEFT) || (event.key.scancode == SDL_SCANCODE_RIGHT))
					mpPaddle->Stop();
				break;

			default:
				break;
			}
		}
	}

	void Game::HandleKeyboardEvent(const SDL_KeyboardEvent& event)
	{
		switch (event.scancode)
		{
		case SDL_SCANCODE_SPACE:
			switch (mState)
			{
			case State::Paused:
				mState = State::Started;
				break;

			case State::PausedDefeat:
			case State::PausedVictory:
				RestartLevel();
				mpBall->Launch();
				mState = State::Started;
				break;

			case State::Loaded:
				mpBall->Launch();
				mState = State::Started;
				break;

			case State::Started:
				if (mpBall->IsLaunched())
					mState = State::Paused;
				else
					mpBall->Launch();
				break;
			}
			break;

		case SDL_SCANCODE_LEFT:
			mpPaddle->MoveLeft();
			break;

		case SDL_SCANCODE_RIGHT:
			mpPaddle->MoveRight();
			break;

		case SDL_SCANCODE_ESCAPE:
			mState = State::Closed;
			break;

		default:
			break;
		}
	}

	void Game::UpdateGame(const float dt)
	{
		// update paddle
		mpPaddle->Update(dt);

		// paddle/wall collisions
		if (mpPaddle->GetLeft() < 0)
			mpPaddle->SetPositionX(0);
		else if (mpPaddle->GetRight() > mWindowW)
			mpPaddle->SetPositionX(mWindowW - mpPaddle->GetWidth());

		// update ball
		if (mpBall->IsLaunched())
		{
			mpBall->Update(dt);

			// ball/wall collisions
			if (mpBall->GetLeft() < 0)
			{
				mpBall->SetPositionX(mpBall->GetRadius());
				mpBall->ReflectX();
			}

			if (mpBall->GetRight() > mWindowW)
			{
				mpBall->SetPositionX(mWindowW - mpBall->GetRadius());
				mpBall->ReflectX();
			}

			if (mpBall->GetTop() < 0)
			{
				mpBall->SetPositionY(mpBall->GetRadius());
				mpBall->ReflectY();
			}

			if (mpBall->GetTop() > mWindowH)
			{
				if (--mLives <= 0)
				{
					mState = State::PausedDefeat;
				}
				else
				{
					ResetBall();  // if we fall through we need to reset the ball
				}
			}

			// paddle/ball collisions
			const Rect prect = mpPaddle->GetRect();
			const Rect brect = mpBall->GetRect();
			if (brect.Intersects(prect) && mpBall->GetVelocityY() > 0.0f)
			{
				// reflect and tweak angle depending on hit position
				const float hitPos = ((brect.x + brect.w / 2.0f) - (prect.x + prect.w / 2.0f)) / (prect.w / 2.0f);
				const float angle = hitPos * (3.14f / 3.0f); // +/- 60 degrees
				const float speed = mpBall->GetSpeed();
				mpBall->SetVelocity(speed * std::sin(angle), -std::abs(speed * std::cos(angle)));
			}

			// ball/brick collisions
			for (auto& b : mBricks)
			{
				if (!b->IsDestroyed() && brect.Intersects(b->GetRect()))
				{
					b->SetDestroyed();
					--mBricksLeft;
					mScore += b->GetPoints();
					// reflect ball - simple approach: reverse Y
					mpBall->ReflectY();
					break; // only one brick per frame
				}
			}

			// win condition
			if (mBricksLeft <= 0)
				mState = State::PausedVictory;
		}
		else
		{
			mpBall->SetPositionX(mpPaddle->GetCenterX());
			mpBall->SetPositionY(mpPaddle->GetTop() - mpBall->GetRadius());
		}
	}

	void Game::UpdateUI()
	{
		// Start the Dear ImGui frame and generate some UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		switch (mState)
		{
		case State::Loaded:
			return ShowCenteredMultiText(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Arkanoid", "Press Space to start the game or Esc to quit.");

		case State::Started:
			return ShowText(glm::vec2(10.0f, 10.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), fmt::format("Score: {}   Lives: {}", mScore, mLives).c_str());

		case State::Paused:
			return ShowCenteredMultiText(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Game Paused", "Press Space to continue the game or Esc to quit.");

		case State::PausedVictory:
			return ShowCenteredMultiText(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), "You Won!", "Press Space to restart the game or Esc to quit.");

		case State::PausedDefeat:
			return ShowCenteredMultiText(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), "You Lost!", "Press Space to restart the game or Esc to quit.");
		}
	}

	void Game::Render()
	{
		ImGui::Render();

		mpRenderer->BeginFrame();

		// draw mBricks
		for (auto& b : mBricks)
			b->Render(*mpRenderer);

		// draw the paddle
		mpPaddle->Render(*mpRenderer);

		// draw the ball
		mpBall->Render(*mpRenderer);

		// draw UI
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mpRenderer->EndFrame();
	}

	void Game::ShowText(const glm::vec2& pos, const glm::vec4& color, const char* const pText)
	{
		static const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoInputs
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoNav;

		ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y)); // Top-left corner of the screen
		ImGui::Begin("Overlay", nullptr, windowFlags);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.r, color.g, color.b, color.a));
		ImGui::TextUnformatted(pText);
		ImGui::SetWindowFontScale(1.0f);
		ImGui::PopStyleColor();
		ImGui::End();
	}

	void Game::ShowCenteredMultiText(const glm::vec4& color, const char* const pText1, const char* const pText2)
	{
		static const float kBigFontScale = 2.0f;
		static const float kSmallFontScale = 1.0f;
		static const ImGuiWindowFlags kWindowFlags = ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoInputs
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoNav;

		ImGui::Begin("Overlay", nullptr, kWindowFlags);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.r, color.g, color.b, color.a));

		// Get window size
		ImGuiIO& io = ImGui::GetIO();
		const ImVec2 windowSize = io.DisplaySize;

		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(windowSize);

		// Calculate text sizes
		ImGui::SetWindowFontScale(kBigFontScale);
		const ImVec2 textSize1 = ImGui::CalcTextSize(pText1);

		ImGui::SetWindowFontScale(kSmallFontScale);
		const ImVec2 textSize2 = ImGui::CalcTextSize(pText2);

		// Calculate positions (centered)
		const float totalHeight = textSize1.y + 10.0f + textSize2.y; // 10px spacing
		const float startY = (windowSize.y - totalHeight) * 0.5f;

		// Center the first line
		ImGui::SetCursorPos(ImVec2((windowSize.x - textSize1.x) * 0.5f, startY));
		ImGui::SetWindowFontScale(kBigFontScale);
		ImGui::TextUnformatted(pText1);

		// Center the second line
		ImGui::SetWindowFontScale(kSmallFontScale);
		ImGui::SetCursorPos(ImVec2((windowSize.x - textSize2.x) * 0.5f, startY + textSize1.y + 10.0f));
		ImGui::TextUnformatted(pText2);

		// Reset font scale
		ImGui::SetWindowFontScale(1.0f);

		ImGui::PopStyleColor();

		ImGui::End();
	}
}
