#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>


namespace ark
{
	struct Rect
	{
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;

		Rect() = default;
		Rect(const float x, const float y, const float w, const float h)
			: x(x), y(y), w(w), h(h)
		{ }

		float GetLeft() const { return x; }
		float GetRight() const { return x + w; }
		float GetTop() const { return y; }
		float GetBottom() const { return y + h; }
		float GetCenterX() const { return x + w / 2.0f; }
		float GetCenterY() const { return y + h / 2.0f; }

		bool Intersects(const Rect& other) const noexcept
		{
			return !(x + w < other.x || x > other.x + other.w || y + h < other.y || y > other.y + other.h);
		}
	};
}