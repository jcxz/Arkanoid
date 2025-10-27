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
		enum class State
		{
			Loaded,            //! the game was just loaded. Show start instructions.
			Started,           //! the game is running
			Paused,            //! the game was paused. Show instructions on how to continue.
			PausedVictory,     //! the game was paused, because player won. Inform player and show restart instructions.
			PausedDefeat,      //! the game was paused, because player lost. Inform player and show restart instructions.
			Closed             //! quit game requested
		};

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
		void ShowText(const glm::vec2& pos, const glm::vec4& color, const char* const pText);
		void ShowCenteredMultiText(const glm::vec4& color, const char* const pText1, const char* const pText2);

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
		State mState = State::Loaded;
	};
}