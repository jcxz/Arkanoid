#include "ball.h"
#include <cmath>



Ball::Ball(float x_, float y_, float r_)
	: x(x_), y(y_), r(r_)
{ }

void Ball::launch()
{
	if (launched) return;
	launched = true;
	// initial direction up-right
	vx = speed * 0.7071f;
	vy = -speed * 0.7071f;
}

void Ball::update(float dt)
{
	if (!launched) return;
	x += vx * dt;
	y += vy * dt;
}

void Ball::render(SDL_Renderer* rdr)
{
	// simple filled circle approximation via many rects (cheap & short)
	SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
	const int NUM = 12;
	for (int i = 0; i < NUM; i++)
	{
		float a1 = (float)i / NUM * 2.0f * 3.1415926f;
		float a2 = (float)(i + 1) / NUM * 2.0f * 3.1415926f;
		float x1 = x + cosf(a1) * (r-0.5f);
		float y1 = y + sinf(a1) * (r-0.5f);
		SDL_FRect rr = { x1, y1, 2, 2 };
		SDL_RenderFillRect(rdr, &rr);
	}
}

void Ball::setPosition(float nx, float ny) { x = nx; y = ny; }
void Ball::reflectX() { vx = -vx; }
void Ball::reflectY() { vy = -vy; }