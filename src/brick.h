#pragma once

#include <SDL3/SDL.h>

enum class BrickColor { Red, Green, Blue };

class Brick
{
public:
	Brick(int x, int y, int w, int h, BrickColor c, int points);
	void render(SDL_Renderer* rdr);
	SDL_FRect getRect() const { return rect; }

public:
	bool destroyed = false;
	int points = 0;
	BrickColor color;
	SDL_FRect rect;
};