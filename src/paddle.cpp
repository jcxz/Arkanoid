#include "paddle.h"
#include <spdlog/spdlog.h>


Paddle::Paddle(int x, int y, int w, int h, int winW)
{
	rect = { float(x), float(y), float(w), float(h) };
	windowW = winW;
}

void Paddle::handleInput(const bool* keystate, float dt)
{
	float move = 0.0f;
	if (keystate[SDL_SCANCODE_LEFT])
		move -= 1.0f;
	if (keystate[SDL_SCANCODE_RIGHT])
		move += 1.0f;

	rect.x += move * speed * dt;

	if (rect.x < 0)
		rect.x = 0;

	if (rect.x + rect.w > windowW)
		rect.x = windowW - rect.w;
}

void Paddle::render(SDL_Renderer* rdr)
{
	SDL_SetRenderDrawColor(rdr, 200, 200, 200, 255);
	SDL_RenderFillRect(rdr, &rect);
}
