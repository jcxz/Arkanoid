#pragma once

#include "core/maths.h"


namespace ark
{
	class Renderer;

	class Brick
	{
	public:
		enum class Color
		{
			Red,
			Green,
			Blue
		};

	public:
		Brick(int x, int y, int w, int h, Color c, int points)
			: mRect(float(x), float(y), float(w), float(h))
			, mColor(c)
			, mPoints(points)
		{ }

		bool IsDestroyed() const { return mDestroyed; }
		void SetDestroyed() { mDestroyed = true; }
		int GetPoints() const { return mPoints; }
		Rect GetRect() const { return mRect; }
		void Render(Renderer& renderer);

	private:
		Rect mRect;
		Color mColor;
		int mPoints = 0;
		bool mDestroyed = false;
	};
}