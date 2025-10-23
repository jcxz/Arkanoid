#include "brick.h"

Brick::Brick(int x, int y, int w, int h, BrickColor c, int points_)
	: points(points_)
	, color(c)
{
	rect = { float(x), float(y), float(w), float(h) };
}

void Brick::render(SDL_Renderer* rdr)
{
	if (destroyed)
		return;

	switch (color)
	{
		case BrickColor::Red:   SDL_SetRenderDrawColor(rdr, 200,  50,  50, 255); break;
		case BrickColor::Green: SDL_SetRenderDrawColor(rdr,  50, 200,  50, 255); break;
		case BrickColor::Blue:  SDL_SetRenderDrawColor(rdr,  50,  50, 200, 255); break;
	}

	SDL_RenderFillRect(rdr, &rect);
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_RenderRect(rdr, &rect);
}