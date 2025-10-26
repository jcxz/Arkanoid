#pragma once

#include "renderer/renderer.h"
#include "game/paddle.h"
#include "game/ball.h"
#include "game/brick.h"
#include <SDL3/SDL.h>
#include <vector>
#include <memory>


namespace ark
{
	class Game
	{
	public:
		bool Init();
		void Run();
		void Terminate();

	private:
		void HandleInput();
		void HandleKeyboardEvent(const SDL_KeyboardEvent& event);
		void Update(float dt);
		void Render();
		void resetBall();
		void spawnLevel();

	private:
		SDL_Window* mpWindow = nullptr;
		std::unique_ptr<Renderer> mpRenderer;
		std::unique_ptr<Paddle> paddle;
		std::unique_ptr<Ball> ball;
		std::vector<std::unique_ptr<Brick>> bricks;
		int mWindowW = 800;
		int mWindowH = 600;
		int lives = 3;
		int score = 0;
		bool running = false;
	};
}