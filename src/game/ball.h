#pragma once

#include "core/maths.h"


namespace ark
{
	class Renderer;

	class Ball
	{
	public:
		Ball(const float x, const float y, const float r)
			: mPos(x, y), mRadius(r)
		{ }

		Rect GetRect() const { return Rect(mPos.x - mRadius, mPos.y - mRadius, 2.0f * mRadius, 2.0f * mRadius); }
		void SetPosition(const float x, const float y) { mPos = glm::vec2(x, y); }
		void SetVelocity(const float x, const float y) { mVelocity = glm::vec2(x, y); }
		void ReflectX() { mVelocity.x = -mVelocity.x; }
		void ReflectY() { mVelocity.y = -mVelocity.y; }
		bool IsLaunched() const { return mLaunched; }

		void Launch();
		void Reset(const float x, const float y);
		void Update(const float dt);
		void Render(Renderer& renderer);

	public:
		glm::vec2 mPos;
		float mRadius;
		glm::vec2 mVelocity = glm::vec2(0.0f, 0.0f);
		float mSpeed = 300.0f;
		bool mLaunched = false;
	};
}