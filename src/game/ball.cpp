#include "game/ball.h"
#include "renderer/renderer.h"


namespace ark
{
	void Ball::Launch()
	{
		if (!mLaunched)
		{
			// initial direction up-right
			//vx = speed * 0.7071f;
			//vy = -speed * 0.7071f;

			mVelocity.x = 0;
			mVelocity.y = -mSpeed;

			mLaunched = true;
		}
	}

	void Ball::Update(const float dt)
	{
		if (mLaunched)
		{
			mPos.x += mVelocity.x * dt;
			mPos.y += mVelocity.y * dt;
		}
	}

	void Ball::Render(Renderer& renderer)
	{
		renderer.DrawCircle(mPos, mRadius, glm::vec4(0.75f, 0.75f, 0.0f, 1.0f));
	}
}