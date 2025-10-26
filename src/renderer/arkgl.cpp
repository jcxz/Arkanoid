#include "renderer/arkgl.h"
#include "core/logger.h"



namespace ark
{
	const char* GLShaderTypeToString(const GLenum type)
	{
#define CASE(lbl) case lbl: return #lbl

		switch (type)
		{
			CASE(GL_VERTEX_SHADER);
			CASE(GL_TESS_CONTROL_SHADER);
			CASE(GL_TESS_EVALUATION_SHADER);
			CASE(GL_GEOMETRY_SHADER);
			CASE(GL_FRAGMENT_SHADER);
			CASE(GL_COMPUTE_SHADER);

		default:
			ARK_WTF("Unknown GL shader type");
			return "";
		}

#undef CASE
	}

	const char* GLDebugMsgSourceToString(const GLenum source)
	{
#define CASE(lbl) case lbl: return #lbl

		switch (source)
		{
#if !defined(ARK_OS_MACOS)
			CASE(GL_DEBUG_SOURCE_API);
			CASE(GL_DEBUG_SOURCE_WINDOW_SYSTEM);
			CASE(GL_DEBUG_SOURCE_SHADER_COMPILER);
			CASE(GL_DEBUG_SOURCE_THIRD_PARTY);
			CASE(GL_DEBUG_SOURCE_APPLICATION);
			CASE(GL_DEBUG_SOURCE_OTHER);
#endif
		default:
			ARK_WTF("Unknown source");
			return "";
		};

#undef CASE
	}

	const char* GLDebugMsgTypeToString(const GLenum type)
	{
#define CASE(lbl) case lbl: return #lbl

		switch (type)
		{
#if !defined(ARK_OS_MACOS)
			CASE(GL_DEBUG_TYPE_ERROR);
			CASE(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
			CASE(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
			CASE(GL_DEBUG_TYPE_PORTABILITY);
			CASE(GL_DEBUG_TYPE_PERFORMANCE);
			CASE(GL_DEBUG_TYPE_MARKER);
			CASE(GL_DEBUG_TYPE_PUSH_GROUP);
			CASE(GL_DEBUG_TYPE_POP_GROUP);
			CASE(GL_DEBUG_TYPE_OTHER);
#endif
		default:
			ARK_WTF("Unknown type");
			return "";
		};

#undef CASE
	}

	const char* GLDebugMsgSeverityToString(const GLenum severity)
	{
#define CASE(lbl) case lbl: return #lbl

		switch (severity)
		{
#if !defined(ARK_OS_MACOS)
			CASE(GL_DEBUG_SEVERITY_HIGH);
			CASE(GL_DEBUG_SEVERITY_MEDIUM);
			CASE(GL_DEBUG_SEVERITY_LOW);
			CASE(GL_DEBUG_SEVERITY_NOTIFICATION);
#endif
		default:
			ARK_WTF("Unknown severity");
			return "";
		};

#undef CASE
	}

	const char* GLErrorToString(const GLenum error)
	{
#define CASE(lbl) case lbl: return #lbl

		switch (error)
		{
			CASE(GL_NO_ERROR);
			CASE(GL_INVALID_ENUM);
			CASE(GL_INVALID_VALUE);
			CASE(GL_INVALID_OPERATION);
			CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
			CASE(GL_OUT_OF_MEMORY);
#if defined(GL_STACK_OVERFLOW)
			CASE(GL_STACK_OVERFLOW);
#endif
#if defined(GL_STACK_UNDERFLOW)
			CASE(GL_STACK_UNDERFLOW);
#endif
			default:
				ARK_WTF("Unknown GL error type");
				return "";
		}

#undef CASE
	}

	void AssertGL(const char* const file, const int line)
	{
		GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR)
		{
			ARK_ERROR("AssertGL: {} (0x{:X}) file:{} line:{}", GLErrorToString(error), error, file, line);
		}
	}
}
