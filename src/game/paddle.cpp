#include "game/paddle.h"
#include "renderer/renderer.h"
#include <SDL3/SDL.h>


namespace ark
{
	Paddle::Paddle(int x, int y, int w, int h, int winW)
		: rect(float(x), float(y), float(w), float(h))
		, windowW(winW)
	{ }

	void Paddle::HandleInput(const bool* keystate)
	{
		if (keystate[SDL_SCANCODE_LEFT])
			direction = -speed;
		else if (keystate[SDL_SCANCODE_RIGHT])
			direction = speed;
		else
			direction = 0.0f;
	}

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
