#include "game/paddle.h"
#include "renderer/renderer.h"



namespace ark
{
	void Paddle::Reset(const float x, const float y)
	{
		mRect.x = x;
		mRect.y = y;
		mDirection = 0.0f;
	}

	void Paddle::Update(const float dt)
	{
		mRect.x += mDirection * dt;
	}

	void Paddle::Render(Renderer& renderer)
	{
		renderer.DrawRect(mRect, glm::vec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.0f));
	}
}
