// This is a wrapper header for OpenGL - includes GL for every supported platform and defines a couple of GL related utilities

#pragma once

#include <GL/glew.h>

namespace ark
{
	extern const char* GLShaderTypeToString(const GLenum type);
	extern const char* GLErrorToString(const GLenum error);
	extern void AssertGL(const char* const file, const int line);
}

#ifdef _DEBUG
#define ARK_ASSERT_GL() ark::AssertGL(__FILE__, __LINE__)
#else
#define ARK_ASSERT_GL()
#endif
