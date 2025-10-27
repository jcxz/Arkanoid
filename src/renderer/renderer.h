#pragma once

#include "core/maths.h"
#include "renderer/shader_program.h"
#include <SDL3/SDL.h>
#include <memory>


namespace ark
{
	class Renderer
	{
	public:
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void SetBackgroundColor(const glm::vec4& bckgColor) { mBckgColor = bckgColor; }
		void DrawRect(const Rect& rect, const glm::vec4& color);
		void DrawRoundedRect(const Rect& rect, const float radius, const glm::vec4& color);
		void DrawCircle(const Rect& rect, const glm::vec4& color);
		void ResizeViewport(const unsigned int w, const unsigned int h);
		void BeginFrame();
		void EndFrame();

		static Renderer* Create(SDL_Window* const pWindow);

	private:
		explicit Renderer(SDL_Window* const pWindow)
			: mpWindow(pWindow)
		{ }

	private:
		// a non-owning reference to the window
		SDL_Window* mpWindow = nullptr;
		SDL_GLContext mpGLContext = nullptr;
		unsigned int mVAO = 0;
		glm::mat4 mProjection;
		glm::vec4 mBckgColor;
		std::unique_ptr<ShaderProgram> mpCircleProgram;
		std::unique_ptr<ShaderProgram> mpRectProgram;
		std::unique_ptr<ShaderProgram> mpRoundedRectProgram;
	};
}