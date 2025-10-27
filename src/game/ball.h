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

		float GetLeft() const { return mPos.x - mRadius; }
		float GetRight() const { return mPos.x + mRadius; }
		float GetTop() const { return mPos.y - mRadius; }
		float GetBottom() const { return mPos.y + mRadius; }
		float GetRadius() const { return mRadius; }

		void SetPosition(const float x, const float y) { mPos = glm::vec2(x, y); }
		void SetPositionX(const float x) { mPos.x = x; }
		void SetPositionY(const float y) { mPos.y = y; }

		float GetVelocityX() const { return mVelocity.x; }
		float GetVelocityY() const { return mVelocity.y; }
		void SetVelocity(const float x, const float y) { mVelocity = glm::vec2(x, y); }
		void ReflectX() { mVelocity.x = -mVelocity.x; }
		void ReflectY() { mVelocity.y = -mVelocity.y; }

		float GetSpeed() const { return mSpeed; }
		bool IsLaunched() const { return mLaunched; }

		void Launch();
		void Reset(const float x, const float y);
		void Update(const float dt);
		void Render(Renderer& renderer);

	private:
		glm::vec2 mPos;
		glm::vec2 mVelocity = glm::vec2(0.0f, 0.0f);
		float mRadius = 0.0f;
		float mSpeed = 300.0f;
		bool mLaunched = false;
	};
}