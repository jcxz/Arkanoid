#include "game.h"
#include "core/logger.h"
#include "renderer/ogl.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"


static bool rectsIntersect(const ark::Rect& a, const ark::Rect& b)
{
	return !(a.x + a.w < b.x || a.x > b.x + b.w || a.y + a.h < b.y || a.y > b.y + b.h);
}


namespace ark
{
	bool Game::Init()
	{
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
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
		paddle = std::make_unique<Paddle>(mWindowW / 2 - 60, mWindowH - 40, 120, 16, mWindowW);
		ball = std::make_unique<Ball>(mWindowW / 2.0f, mWindowH - 60.0f, 8.0f);
		spawnLevel();
		resetBall();
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

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();

			Update(dt);
			Render();
		}
	}

	void Game::Terminate()
	{
		mpRenderer.reset();
		SDL_DestroyWindow(mpWindow);
	}

	void Game::spawnLevel()
	{
		bricks.clear();
		int rows = 5;
		int cols = 10;
		int bw = 64;
		int bh = 24;
		int startX = (mWindowW - cols * bw) / 2;
		int startY = 60;
		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < cols; c++) {
				BrickColor col = (r % 3 == 0 ? BrickColor::Red : (r % 3 == 1 ? BrickColor::Green : BrickColor::Blue));
				int pts = (r % 3 == 0 ? 100 : (r % 3 == 1 ? 50 : 30));
				bricks.emplace_back(std::make_unique<Brick>(startX + c * bw, startY + r * bh, bw - 4, bh - 4, col, pts));
			}
		}
	}

	void Game::resetBall()
	{
		ball->launched = false;
		ball->speed = 300.0f;
		ball->setPosition(float(paddle->getRect().x + paddle->getRect().w / 2), float(paddle->getRect().y - 12));
		ball->vx = 0; ball->vy = 0;
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
			ball->launch();
			break;

		case SDL_SCANCODE_LEFT:
			break;

		case SDL_SCANCODE_RIGHT:
			break;

		default:
			break;
		}
	}

	void Game::Update(float dt)
	{
		const bool* keystate = SDL_GetKeyboardState(NULL);
		paddle->handleInput(keystate, dt);
		// if ball not launched, keep it above paddle
		if (!ball->launched)
		{
			ball->setPosition(float(paddle->getRect().x + paddle->getRect().w / 2), float(paddle->getRect().y - 12));
		}
		else
		{
			ball->update(dt);
		}

		// wall collisions
		if (ball->x - ball->r < 0)
		{
			ball->x = ball->r;
			ball->reflectX();
		}

		if (ball->x + ball->r > mWindowW)
		{
			ball->x = mWindowW - ball->r;
			ball->reflectX();
		}

		if (ball->y - ball->r < 0)
		{
			ball->y = ball->r;
			ball->reflectY();
		}

		// bottom (lose life)
		if (ball->y - ball->r > mWindowH)
		{
			lives -= 1;
			if (lives <= 0)
			{
				ARK_INFO("You lost !");
				running = false; // game over
			}
			resetBall();
		}

		// paddle collision (basic)
		Rect brect = ball->getRect();
		Rect prect = paddle->getRect();
		if (rectsIntersect(brect, prect) && ball->vy > 0)
		{
			// reflect and tweak angle depending on hit position
			float hitPos = ((brect.x + brect.w / 2.0f) - (prect.x + prect.w / 2.0f)) / (prect.w / 2.0f);
			float angle = hitPos * (3.14f / 3.0f); // +/- 60 degrees
			float s = ball->speed;
			ball->vx = s * sinf(angle);
			ball->vy = -fabsf(s * cosf(angle));
		}

		// bricks collisions
		for (auto& b : bricks)
		{
			if (!b->destroyed && rectsIntersect(brect, b->getRect()))
			{
				b->destroyed = true;
				score += b->points;
				// reflect ball - simple approach: reverse Y
				ball->reflectY();
				break; // only one brick per frame
			}
		}

		// win condition
		bool anyLeft = false;
		for (auto& b : bricks)
		{
			if (!b->destroyed)
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
	}

	void Game::Render()
	{
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

		ImGui::Render();
		ImGuiIO& io = ImGui::GetIO();
		//SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

		mpRenderer->BeginFrame();

		// draw bricks
		for (auto& b : bricks)
			b->render(*mpRenderer);

		// paddle
		paddle->render(*mpRenderer);

		// ball
		ball->render(*mpRenderer);

		// HUD: score and lives
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mpRenderer->EndFrame();
	}
}
