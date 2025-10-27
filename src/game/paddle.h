#pragma once

#include "core/maths.h"


namespace ark
{
	class Renderer;

	class Paddle
	{
	public:
		Paddle(int x, int y, int w, int h)
			: mRect(float(x), float(y), float(w), float(h))
		{ }

		const Rect& GetRect() const { return mRect; }
		float GetWidth() const { return mRect.w; }
		float GetHeight() const { return mRect.h; }
		float GetLeft() const { return mRect.GetLeft(); }
		float GetRight() const { return mRect.GetRight(); }
		float GetTop() const { return mRect.GetTop(); }
		float GetBottom() const { return mRect.GetBottom(); }
		float GetCenterX() const { return mRect.GetCenterX(); }
		float GetCenterY() const { return mRect.GetCenterY(); }

		void MoveLeft() { mDirection = -mSpeed; }
		void MoveRight() { mDirection = mSpeed; }
		void Stop() { mDirection = 0.0f; }
		void SetPositionX(const float x) { mRect.x = x; }
		void SetPositionY(const float y) { mRect.y = y; }
		void Reset(const float x, const float y);
		void Update(const float dt);
		void Render(Renderer& renderer);

	private:
		Rect mRect;
		float mSpeed = 500.0f;
		float mDirection = 0.0f;
	};
}