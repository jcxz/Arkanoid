#pragma once

#include <SDL3/SDL.h>


class Paddle
{
public:
	Paddle(int x, int y, int w, int h, int winW);
	void handleInput(const bool* keystate, float dt);
	void render(SDL_Renderer* rdr);
	SDL_FRect getRect() const { return rect; }

private:
	SDL_FRect rect;
	int windowW;
	int speed = 500;
};