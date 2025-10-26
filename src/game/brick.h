#pragma once

#include "core/maths.h"


namespace ark
{
	enum class BrickColor { Red, Green, Blue };

	class Renderer;

	class Brick
	{
	public:
		Brick(int x, int y, int w, int h, BrickColor c, int points);
		void render(Renderer& renderer);
		Rect getRect() const { return rect; }

	public:
		bool destroyed = false;
		int points = 0;
		BrickColor color;
		Rect rect;
	};
}