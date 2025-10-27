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
		void UpdateGame(const float dt);
		void UpdateUI();
		void Render();
		void ResetBall();
		void RestartLevel();

	private:
		SDL_Window* mpWindow = nullptr;
		std::unique_ptr<Renderer> mpRenderer;
		std::unique_ptr<Paddle> mpPaddle;
		std::unique_ptr<Ball> mpBall;
		std::vector<std::unique_ptr<Brick>> mBricks;
		int mWindowW = 800;
		int mWindowH = 600;
		int mLives = 3;
		int mScore = 0;
	};
}