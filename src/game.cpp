#include "game.h"
#include "core/logger.h"
#include "imgui.h"
#include "../external/imgui_impl_sdl3.h"
#include "../external/imgui_impl_sdlrenderer3.h"



static bool rectsIntersect(const SDL_FRect& a, const SDL_FRect& b)
{
	return !(a.x + a.w < b.x || a.x > b.x + b.w || a.y + a.h < b.y || a.y > b.y + b.h);
}


void Game::Terminate()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window) SDL_DestroyWindow(window);
}

bool Game::init()
{
	window = SDL_CreateWindow("Arkanoid - SDL2", windowW, windowH, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, nullptr);
	if (!window || !renderer)
	{
		ARK_ERROR("CreateWindow/Renderer error: {}", SDL_GetError());
		return false;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	paddle = std::make_unique<Paddle>(windowW/2 - 60, windowH - 40, 120, 16, windowW);
	ball = std::make_unique<Ball>(windowW/2.0f, windowH - 60.0f, 8.0f);
	spawnLevel();
	resetBall();
	running = true;
	return true;
}

void Game::spawnLevel()
{
	bricks.clear();
	int rows = 5;
	int cols = 10;
	int bw = 64;
	int bh = 24;
	int startX = (windowW - cols*bw)/2;
	int startY = 60;
	for (int r=0;r<rows;r++){
		for (int c=0;c<cols;c++){
			BrickColor col = (r%3==0?BrickColor::Red: (r%3==1?BrickColor::Green:BrickColor::Blue));
			int pts = (r%3==0?100: (r%3==1?50:30));
			bricks.emplace_back(std::make_unique<Brick>(startX + c*bw, startY + r*bh, bw-4, bh-4, col, pts));
		}
	}
}

void Game::resetBall()
{
	ball->launched = false;
	ball->speed = 300.0f;
	ball->setPosition(float(paddle->getRect().x + paddle->getRect().w/2), float(paddle->getRect().y - 12));
	ball->vx = 0; ball->vy = 0;
}

void Game::processInput()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		ImGui_ImplSDL3_ProcessEvent(&ev);
		if (ev.type == SDL_EVENT_QUIT)
			running = false;
		if (ev.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && ev.window.windowID == SDL_GetWindowID(window))
			running = false;
		if (ev.type == SDL_EVENT_KEY_DOWN)
		{
			if (ev.key.scancode == SDL_SCANCODE_SPACE)
			{
				if (!ball->launched)
					ball->launch();
			}
		}
	}
}

void Game::update(float dt)
{
	const bool* keystate = SDL_GetKeyboardState(NULL);
	paddle->handleInput(keystate, dt);
	// if ball not launched, keep it above paddle
	if (!ball->launched)
	{
		ball->setPosition(float(paddle->getRect().x + paddle->getRect().w/2), float(paddle->getRect().y - 12));
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

	if (ball->x + ball->r > windowW)
	{
		ball->x = windowW - ball->r;
		ball->reflectX();
	}

	if (ball->y - ball->r < 0)
	{
		ball->y = ball->r;
		ball->reflectY();
	}

	// bottom (lose life)
	if (ball->y - ball->r > windowH)
	{
		lives -= 1;
		if (lives <= 0)
			running = false; // game over
		resetBall();
	}

	// paddle collision (basic)
	SDL_FRect brect = ball->getRect();
	SDL_FRect prect = paddle->getRect();
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
		if (b->destroyed)
			continue;
		if (rectsIntersect(brect, b->getRect()))
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
		running = false; // win -> stop loop
}

void Game::render()
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
	SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);

	// draw bricks
	for (auto& b : bricks)
		b->render(renderer);

	// paddle
	paddle->render(renderer);

	// ball
	ball->render(renderer);

	// HUD: score and lives
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
	SDL_RenderPresent(renderer);
}

void Game::run()
{
	Uint64 last = SDL_GetPerformanceCounter();
	while (running)
	{
		const Uint64 now = SDL_GetPerformanceCounter();
		const float dt = float(now - last) / float(SDL_GetPerformanceFrequency());
		last = now;
		processInput();

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		update(dt);
		render();
	}
}
