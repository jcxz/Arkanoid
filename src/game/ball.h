#pragma once

#include "core/maths.h"


namespace ark
{
	class Renderer;

	class Ball
	{
	public:
		Ball(float x, float y, float r);
		void launch();
		void update(float dt);
		void render(Renderer& renderer);
		void setPosition(float x, float y);
		Rect getRect() const { return { x - r, y - r, 2 * r, 2 * r }; }
		void reflectX();
		void reflectY();

	public:
		float x, y, r;
		float vx = 0, vy = 0; // velocity
		float speed = 300.0f;
		bool launched = false;
	};
}