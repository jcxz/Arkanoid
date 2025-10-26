#include "game/ball.h"
#include "renderer/renderer.h"


namespace ark
{
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

	void Ball::render(Renderer& renderer)
	{
		renderer.DrawCircle(glm::vec2(x, y), r, glm::vec4(0.75f, 0.75f, 0.0f, 1.0f));
	}

	void Ball::setPosition(float nx, float ny) { x = nx; y = ny; }
	void Ball::reflectX() { vx = -vx; }
	void Ball::reflectY() { vy = -vy; }
}