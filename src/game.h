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
	~Game();
	bool init();
	void run();

private:
	void processInput();
	void update(float dt);
	void render();
	void resetBall();
	void spawnLevel();

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int windowW = 800;
	int windowH = 600;

	bool running = false;
	std::unique_ptr<Paddle> paddle;
	std::unique_ptr<Ball> ball;
	std::vector<std::unique_ptr<Brick>> bricks;

	int lives = 3;
	int score = 0;
};