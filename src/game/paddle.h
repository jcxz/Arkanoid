#pragma once

#include "core/maths.h"


namespace ark
{
	class Renderer;

	class Paddle
	{
	public:
		Paddle(int x, int y, int w, int h, int winW)
			: mRect(float(x), float(y), float(w), float(h))
			, mWindowW(winW)
		{ }

		Rect GetRect() const { return mRect; }
		void MoveLeft() { mDirection = -mSpeed; }
		void MoveRight() { mDirection = mSpeed; }
		void Stop() { mDirection = 0.0f; }
		void Reset(const float x, const float y);
		void Update(const float dt);
		void Render(Renderer& renderer);

	private:
		Rect mRect;
		int mWindowW;
		float mSpeed = 500.0f;
		float mDirection = 0.0f;
	};
}