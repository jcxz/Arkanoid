#pragma once

#include "core/maths.h"


namespace ark
{
	class Renderer;

	class Paddle
	{
	public:
		Paddle(int x, int y, int w, int h, int winW);
		void handleInput(const bool* keystate, float dt);
		void render(Renderer& renderer);
		Rect getRect() const { return rect; }

	private:
		Rect rect;
		int windowW;
		int speed = 500;
	};
}