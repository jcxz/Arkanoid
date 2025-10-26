#include "game/brick.h"
#include "renderer/renderer.h"


namespace ark
{
	Brick::Brick(int x, int y, int w, int h, BrickColor c, int points_)
		: points(points_)
		, color(c)
	{
		rect = { float(x), float(y), float(w), float(h) };
	}

	void Brick::render(Renderer& renderer)
	{
		if (destroyed)
			return;

		switch (color)
		{
		case BrickColor::Red:   renderer.DrawRect(rect, glm::vec4(200.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f)); break;
		case BrickColor::Green: renderer.DrawRect(rect, glm::vec4(50.0f / 255.0f, 200.0f / 255.0f, 50.0f / 255.0f, 1.0f)); break;
		case BrickColor::Blue:  renderer.DrawRect(rect, glm::vec4(50.0f / 255.0f, 50.0f / 255.0f, 200.0f / 255.0f, 1.0f)); break;
		}
	}
}