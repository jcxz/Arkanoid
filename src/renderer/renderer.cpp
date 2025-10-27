#include "renderer/renderer.h"
#include "renderer/ogl.h"
#include "core/logger.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"


static std::string kCircleShaderVS = R"(
uniform mat4 mvp;
uniform vec4 rect;

const vec2 vertices[4] = vec2[4](
	vec2(1.0, 0.0),
	vec2(1.0, 1.0),
	vec2(0.0, 0.0),
	vec2(0.0, 1.0)
);

out vec2 quadPos;

void main()
{
	vec2 p = vertices[gl_VertexID];
	quadPos = p * 2.0 - 1.0;
	gl_Position = mvp * vec4(p * rect.zw + rect.xy, 0.0, 1.0);
}
)";

static std::string kCircleShaderFS = R"(
uniform vec4 color;

in vec2 quadPos;

out vec4 fragColor;

void main()
{
	if (dot(quadPos, quadPos) > 1.0)
		discard;
	fragColor = color;
}
)";

static std::string kRectShaderVS = R"(
uniform mat4 mvp;
uniform vec4 rect;

const vec2 vertices[4] = vec2[4](
	vec2(1.0, 0.0),
	vec2(1.0, 1.0),
	vec2(0.0, 0.0),
	vec2(0.0, 1.0)
);

void main()
{
	gl_Position = mvp * vec4(vertices[gl_VertexID] * rect.zw + rect.xy, 0.0, 1.0);
}
)";

static std::string kRectShaderFS = R"(
uniform vec4 color;

out vec4 fragColor;

void main()
{
	fragColor = color;
}
)";

static std::string kRoundedRectShaderVS = R"(
uniform mat4 mvp;
uniform vec4 rect;

out vec2 quadPos;

const vec2 vertices[4] = vec2[4](
	vec2(1.0, 0.0),
	vec2(1.0, 1.0),
	vec2(0.0, 0.0),
	vec2(0.0, 1.0)
);

void main()
{
	quadPos = vertices[gl_VertexID];
	gl_Position = mvp * vec4(vertices[gl_VertexID] * rect.zw + rect.xy, 0.0, 1.0);
}
)";

static std::string kRoundedRectShaderFS = R"(
uniform vec4 color;
uniform float radius;
uniform vec4 rect;

out vec4 fragColor;

in vec2 quadPos;

void main()
{
	vec2 halfSize = rect.zw * 0.5;
	vec2 pos = (quadPos - 0.5) * rect.zw;
	vec2 d = abs(pos) - (halfSize - vec2(radius));
	float dist = length(max(d, 0.0)) - radius;

	if (dist > 0.0)
		discard;  // outside the rounded corner

	fragColor = color;
}
)";


#if !defined(ARK_OS_MACOS) && !defined(ARK_OS_IOS) && !defined(ARK_OS_TVOS)
static void GLAPIENTRY OGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /* length */, const GLchar* message, const void* userParam)
{
	using namespace ark;

	switch (severity)
	{
#if !defined(ARK_OS_MACOS)
	case GL_DEBUG_SEVERITY_HIGH:
		ARK_ERROR("[OpenGL] (renderer={}, source={}, type={}, id={}, severity={}): '{}'", userParam, GLDebugMsgSourceToString(source), GLDebugMsgTypeToString(type), id, GLDebugMsgSeverityToString(severity), message);
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		ARK_WARN("[OpenGL] (renderer={}, source={}, type={}, id={}, severity={}): '{}'", userParam, GLDebugMsgSourceToString(source), GLDebugMsgTypeToString(type), id, GLDebugMsgSeverityToString(severity), message);
		break;

	case GL_DEBUG_SEVERITY_LOW:
		ARK_INFO("[OpenGL] (renderer={}, source={}, type={}, id={}, severity={}): '{}'", userParam, GLDebugMsgSourceToString(source), GLDebugMsgTypeToString(type), id, GLDebugMsgSeverityToString(severity), message);
		break;
#endif
	default:
		ARK_DEBUG("[OpenGL] (renderer={}, source={}, type={}, id={}, severity={}): '{}'", userParam, GLDebugMsgSourceToString(source), GLDebugMsgTypeToString(type), id, GLDebugMsgSeverityToString(severity), message);
		break;
	}
}
#endif


namespace ark
{
	Renderer::~Renderer()
	{
		mpCircleProgram.reset();
		mpRectProgram.reset();
		mpRoundedRectProgram.reset();
		glDeleteVertexArrays(1, &mVAO);
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		SDL_GL_DestroyContext(mpGLContext);
	}

	void Renderer::DrawRect(const Rect& rect, const glm::vec4& color)
	{
		mpRectProgram->Use();
		mpRectProgram->SetVector("rect", glm::vec4(rect.x, rect.y, rect.w, rect.h));
		mpRectProgram->SetVector("color", color);
		mpRectProgram->SetMatrix("mvp", mProjection);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		ARK_ASSERT_GL();
	}

	void Renderer::DrawRoundedRect(const Rect& rect, const float radius, const glm::vec4& color)
	{
		mpRoundedRectProgram->Use();
		mpRoundedRectProgram->SetVector("rect", glm::vec4(rect.x, rect.y, rect.w, rect.h));
		mpRoundedRectProgram->SetVector("color", color);
		mpRoundedRectProgram->SetFloat("radius", radius);
		mpRoundedRectProgram->SetMatrix("mvp", mProjection);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		ARK_ASSERT_GL();
	}

	void Renderer::DrawCircle(const Rect& rect, const glm::vec4& color)
	{
		mpCircleProgram->Use();
		mpCircleProgram->SetVector("rect", glm::vec4(rect.x, rect.y, rect.w, rect.h));
		mpCircleProgram->SetVector("color", color);
		mpCircleProgram->SetMatrix("mvp", mProjection);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		ARK_ASSERT_GL();
	}

	void Renderer::ResizeViewport(const unsigned int w, const unsigned int h)
	{
		glViewport(0, 0, w, h);
		ARK_ASSERT_GL();
		mProjection = glm::ortho(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, -1.0f, 1.0f); // ortographic projection
	}

	void Renderer::BeginFrame()
	{
		glClearColor(mBckgColor.x, mBckgColor.y, mBckgColor.z, mBckgColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::EndFrame()
	{
		SDL_GL_SwapWindow(mpWindow);
	}

	Renderer* Renderer::Create(SDL_Window* const pWindow)
	{
		std::unique_ptr<Renderer> pRenderer(new Renderer(pWindow));

		// creates a new context and makes it current
		pRenderer->mpGLContext = SDL_GL_CreateContext(pRenderer->mpWindow);
		if (pRenderer->mpGLContext == nullptr)
		{
			ARK_ERROR("Failed to create renderer. SDL_GL_CreateContext failed: '{}'", SDL_GetError());
			return nullptr;
		}

		SDL_GL_SetSwapInterval(1);

		const GLenum glewStatus = glewInit();
		if (glewStatus != GLEW_OK)
		{
			ARK_ERROR("Failed to create renderer. GLEW initialization failed: '{}'", (const char*)glewGetErrorString(glewStatus));
			return nullptr;
		}

		// if debug context was requested, then setup the debug callback
		int contextFlags = 0;
		if (SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &contextFlags) && (contextFlags & SDL_GL_CONTEXT_DEBUG_FLAG))
		{
#if !defined(ARK_OS_MACOS) && !defined(ARK_OS_IOS) && !defined(ARK_OS_TVOS)
			if (glDebugMessageCallback != nullptr)
			{
				glDebugMessageCallback(OGLDebugCallback, nullptr);
				ARK_ASSERT_GL();
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				ARK_ASSERT_GL();
			}
			else
#endif
			{
				ARK_WARN("Unable to setup debug callback, because glDebugMessageCallback is not present");
			}
		}

		// Setup ImGui backends
		ImGui_ImplSDL3_InitForOpenGL(pRenderer->mpWindow, pRenderer->mpGLContext);
		ImGui_ImplOpenGL3_Init(nullptr);

		// make a dummy VAO and bind it, because OpenGL 3.3 mandates one
		glGenVertexArrays(1, &pRenderer->mVAO);
		ARK_ASSERT_GL();

		glBindVertexArray(pRenderer->mVAO);
		ARK_ASSERT_GL();

		// compile shader programs
		pRenderer->mpCircleProgram.reset(ShaderProgram::Create("circle", "circle.vs", kCircleShaderVS, "circle.fs", kCircleShaderFS));
		if (pRenderer->mpCircleProgram == nullptr)
		{
			ARK_ERROR("Failed to initialize circle shader");
			return nullptr;
		}

		pRenderer->mpRectProgram.reset(ShaderProgram::Create("rect", "rect.vs", kRectShaderVS, "rect.fs", kRectShaderFS));
		if (pRenderer->mpRectProgram == nullptr)
		{
			ARK_ERROR("Failed to initialize rect shader");
			return nullptr;
		}

		pRenderer->mpRoundedRectProgram.reset(ShaderProgram::Create("rounded_rect", "rounded_rect.vs", kRoundedRectShaderVS, "rounded_rect.fs", kRoundedRectShaderFS));
		if (pRenderer->mpRoundedRectProgram == nullptr)
		{
			ARK_ERROR("Failed to initialize rounded rect shader");
			return nullptr;
		}

		return pRenderer.release();
	}
}