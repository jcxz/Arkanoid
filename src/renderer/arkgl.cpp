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
