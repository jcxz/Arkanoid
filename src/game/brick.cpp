#include "game/brick.h"
#include "renderer/renderer.h"


namespace ark
{
	void Brick::Render(Renderer& renderer)
	{
		if (mDestroyed)
			return;

		switch (mColor)
		{
		case Color::Red:   renderer.DrawRect(mRect, glm::vec4(200.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f)); break;
		case Color::Green: renderer.DrawRect(mRect, glm::vec4(50.0f / 255.0f, 200.0f / 255.0f, 50.0f / 255.0f, 1.0f)); break;
		case Color::Blue:  renderer.DrawRect(mRect, glm::vec4(50.0f / 255.0f, 50.0f / 255.0f, 200.0f / 255.0f, 1.0f)); break;
		}
	}
}