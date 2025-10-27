#include "game/paddle.h"
#include "renderer/renderer.h"



namespace ark
{
	void Paddle::Update(const float dt)
	{
		rect.x += direction *dt;

		if (rect.x < 0)
			rect.x = 0;

		if (rect.x + rect.w > windowW)
			rect.x = windowW - rect.w;
	}

	void Paddle::Render(Renderer& renderer)
	{
		renderer.DrawRect(rect, glm::vec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.0f));
	}
}
