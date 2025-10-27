#pragma once

#include "core/maths.h"


namespace ark
{
	class Renderer;

	class Paddle
	{
	public:
		Paddle(int x, int y, int w, int h, int winW)
			: rect(float(x), float(y), float(w), float(h))
			, windowW(winW)
		{ }

		Rect GetRect() const { return rect; }
		void MoveLeft() { direction = -speed; }
		void MoveRight() { direction = speed; }
		void Stop() { direction = 0.0f; }
		void Update(const float dt);
		void Render(Renderer& renderer);

	private:
		Rect rect;
		int windowW;
		float speed = 500.0f;
		float direction = 0.0f;
	};
}