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
		mpPaddle = std::make_unique<Paddle>(mWindowW / 2 - 60, mWindowH - 40, 120, 16, mWindowW);
		mpBall = std::make_unique<Ball>(mWindowW / 2.0f, mWindowH - 60.0f, 8.0f);
		RestartLevel();
		running = true;
		return true;
	}

	void Game::Run()
	{
		Uint64 last = SDL_GetPerformanceCounter();
		while (running)
		{
			const Uint64 now = SDL_GetPerformanceCounter();
			const float dt = float(now - last) / float(SDL_GetPerformanceFrequency());
			last = now;
			HandleInput();
			Update(dt);
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
		int rows = 5;
		int cols = 10;
		int bw = 64;
		int bh = 24;
		int startX = (mWindowW - cols * bw) / 2;
		int startY = 60;
		for (int r = 0; r < rows; ++r)
		{
			for (int c = 0; c < cols; ++c)
			{
				Brick::Color col = (r % 3 == 0 ? Brick::Color::Red : (r % 3 == 1 ? Brick::Color::Green : Brick::Color::Blue));
				int pts = (r % 3 == 0 ? 100 : (r % 3 == 1 ? 50 : 30));
				mBricks.emplace_back(std::make_unique<Brick>(startX + c * bw, startY + r * bh, bw - 4, bh - 4, col, pts));
			}
		}

		ResetBall();
	}

	void Game::ResetBall()
	{
		mpBall->mLaunched = false;
		mpBall->mSpeed = 300.0f;
		mpBall->SetPosition(float(mpPaddle->GetRect().x + mpPaddle->GetRect().w / 2), float(mpPaddle->GetRect().y - 12));
		mpBall->SetVelocity(0, 0);
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
				running = false;
				break;

			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				running = (event.window.windowID == SDL_GetWindowID(mpWindow));
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
			mpBall->Launch();
			break;

		case SDL_SCANCODE_LEFT:
			mpPaddle->MoveLeft();
			break;

		case SDL_SCANCODE_RIGHT:
			mpPaddle->MoveRight();
			break;

		default:
			break;
		}
	}

	void Game::UpdateGame(const float dt)
	{
		mpPaddle->Update(dt);

		// if mpBall not launched, keep it above mpPaddle
		if (!mpBall->mLaunched)
		{
			mpBall->SetPosition(float(mpPaddle->GetRect().x + mpPaddle->GetRect().w / 2), float(mpPaddle->GetRect().y - mpBall->mRadius * 2.0f));
		}
		else
		{
			mpBall->Update(dt);
		}

		// wall collisions
		if (mpBall->mPos.x - mpBall->mRadius < 0)
		{
			mpBall->mPos.x = mpBall->mRadius;
			mpBall->ReflectX();
		}

		if (mpBall->mPos.x + mpBall->mRadius > mWindowW)
		{
			mpBall->mPos.x = mWindowW - mpBall->mRadius;
			mpBall->ReflectX();
		}

		if (mpBall->mPos.y - mpBall->mRadius < 0)
		{
			mpBall->mPos.y = mpBall->mRadius;
			mpBall->ReflectY();
		}

		// bottom (lose life)
		if (mpBall->mPos.y - mpBall->mRadius > mWindowH)
		{
			mLives -= 1;
			if (mLives <= 0)
			{
				ARK_INFO("You lost !");
				running = false; // game over
			}
			ResetBall();
		}

		// mpPaddle collision (basic)
		const Rect brect = mpBall->GetRect();
		const Rect prect = mpPaddle->GetRect();
		if (brect.Intersects(prect) && mpBall->mVelocity.y > 0)
		{
			// reflect and tweak angle depending on hit position
			float hitPos = ((brect.x + brect.w / 2.0f) - (prect.x + prect.w / 2.0f)) / (prect.w / 2.0f);
			float angle = hitPos * (3.14f / 3.0f); // +/- 60 degrees
			float s = mpBall->mSpeed;
			mpBall->mVelocity.x = s * sinf(angle);
			mpBall->mVelocity.y = -fabsf(s * cosf(angle));
		}

		// mBricks collisions
		for (auto& b : mBricks)
		{
			if (!b->IsDestroyed() && brect.Intersects(b->GetRect()))
			{
				b->SetDestroyed();
				mScore += b->GetPoints();
				// reflect mpBall - simple approach: reverse Y
				mpBall->ReflectY();
				break; // only one brick per frame
			}
		}

		// win condition
		bool anyLeft = false;
		for (auto& b : mBricks)
		{
			if (!b->IsDestroyed())
			{
				anyLeft = true;
				break;
			}
		}

		if (!anyLeft)
		{
			ARK_INFO("You won !");
			running = false; // win -> stop loop
		}

	void Game::UpdateUI()
	{
		// Start the Dear ImGui frame and generate some UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(10, 10)); // Top-left corner of the screen

		const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav;

		ImGui::Begin("Overlay", nullptr, flags);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
		ImGui::TextUnformatted(fmt::format("Score: {}   Lives: {}", score, lives).c_str());
		ImGui::PopStyleColor();
		ImGui::End();
	}

	void Game::Render()
	{
		ImGui::Render();

		mpRenderer->BeginFrame();

		// draw mBricks
		for (auto& b : mBricks)
			b->Render(*mpRenderer);

		// mpPaddle
		mpPaddle->Render(*mpRenderer);

		// mpBall
		mpBall->Render(*mpRenderer);

		// HUD: score and lives
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mpRenderer->EndFrame();
	}
}
