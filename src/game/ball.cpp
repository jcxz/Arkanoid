#include "game/ball.h"
#include "renderer/renderer.h"


namespace ark
{
	void Ball::Launch()
	{
		// initial direction up-right
		//vx = speed * 0.7071f;
		//vy = -speed * 0.7071f;

		mVelocity.x = 0;
		mVelocity.y = -mSpeed;
		mLaunched = true;
	}

	void Ball::Reset(const float x, const float y)
	{
		mLaunched = false;
		mSpeed = 300.0f;
		SetPosition(x, y);
		SetVelocity(0, 0);
	}

	void Ball::Update(const float dt)
	{
		mPos.x += mVelocity.x * dt;
		mPos.y += mVelocity.y * dt;
	}

	void Ball::Render(Renderer& renderer)
	{
		renderer.DrawCircle(GetRect(), glm::vec4(0.75f, 0.75f, 0.0f, 1.0f));
	}
}