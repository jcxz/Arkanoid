#pragma once

#include "core/maths.h"


namespace ark
{
	class Renderer;

	class Paddle
	{
	public:
		Paddle(int x, int y, int w, int h, int winW);
		Rect GetRect() const { return rect; }
		void HandleInput(const bool* keystate);
		void Update(const float dt);
		void Render(Renderer& renderer);

	private:
		Rect rect;
		int windowW;
		float speed = 500.0f;
		float direction = 0.0f;
	};
}