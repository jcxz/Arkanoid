#pragma once

#include <SDL3/SDL.h>

class Ball
{
public:
	Ball(float x, float y, float r);
	void launch();
	void update(float dt);
	void render(SDL_Renderer* rdr);
	void setPosition(float x, float y);
	SDL_FRect getRect() const { return {x-r, y-r, 2*r, 2*r}; }
	void reflectX();
	void reflectY();

public:
	float x, y, r;
	float vx = 0, vy = 0; // velocity
	float speed = 300.0f;
	bool launched = false;
};