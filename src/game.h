#pragma once

#include "paddle.h"
#include "ball.h"
#include "brick.h"
#include <SDL3/SDL.h>
#include <vector>
#include <memory>


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
	SDL_Renderer* renderer = nullptr;
	SDL_GLContext gl_context = nullptr;

	int windowW = 800;
	int windowH = 600;

	bool running = false;
	std::unique_ptr<Paddle> paddle;
	std::unique_ptr<Ball> ball;
	std::vector<std::unique_ptr<Brick>> bricks;

	int lives = 3;
	int score = 0;
};