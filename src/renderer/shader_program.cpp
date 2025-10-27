#include "renderer/shader_program.h"
#include "renderer/ogl.h"
#include "core/logger.h"



namespace ark
{
	struct ShaderProgram::AttributeTraits
	{
		static const GLenum MAX_LENGTH_FLAG = GL_ACTIVE_ATTRIBUTE_MAX_LENGTH;
		static const GLenum COUNT_FLAG = GL_ACTIVE_ATTRIBUTES;

		static void GetResourceInfo(const GLuint id, const GLuint i, const GLsizei maxLen, GLint* const size, GLenum* const type, GLchar* const name)
		{
			return glGetActiveAttrib(id, i, maxLen, nullptr, size, type, name);
		}

		static GLint GetResourceLocation(const GLuint id, const GLchar* const name)
		{
			return glGetAttribLocation(id, name);
		}
	};

	struct ShaderProgram::UniformTraits
	{
		static const GLenum MAX_LENGTH_FLAG = GL_ACTIVE_UNIFORM_MAX_LENGTH;
		static const GLenum COUNT_FLAG = GL_ACTIVE_UNIFORMS;

		static void GetResourceInfo(const GLuint id, const GLuint i, const GLsizei maxLen, GLint* const size, GLenum* const type, GLchar* const name)
		{
			return glGetActiveUniform(id, i, maxLen, nullptr, size, type, name);
		}

		static GLint GetResourceLocation(const GLuint id, const GLchar* const name)
		{
			return glGetUniformLocation(id, name);
		}
	};

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(mID);
		ARK_ASSERT_GL();
	}

	void ShaderProgram::Use()
	{
		glUseProgram(mID);
		ARK_ASSERT_GL();
	}

	ShaderProgram* ShaderProgram::Create(
		const std::string& name,
		const std::string& vertShaderName,
		const std::string& vertShaderSource,
		const std::string& fragShaderName,
		const std::string& fragShaderSource)
	{
		// compile both shaders and link them
		const unsigned int vsID = CompileShader(GL_VERTEX_SHADER, vertShaderSource, vertShaderName);
		if (vsID == 0)
		{
			ARK_ERROR("Failed to compile vertex shader for program {}", name);
			return nullptr;
		}

		const unsigned int fsID = CompileShader(GL_FRAGMENT_SHADER, fragShaderSource, fragShaderName);
		if (fsID == 0)
		{
			ARK_ERROR("Failed to compile fragment shader for program {}", name);
			glDeleteShader(vsID);
			ARK_ASSERT_GL();
			return nullptr;
		}

		const unsigned int programID = LinkProgram(vsID, fsID, name);
		if (programID == 0)
		{
			ARK_ERROR("Failed to link shader program '{}'", name);
			glDeleteShader(vsID);
			ARK_ASSERT_GL();
			glDeleteShader(fsID);
			ARK_ASSERT_GL();
			return nullptr;
		}

		// after linking we don't need shader objects anymore (they are referenced by the program)
		glDeleteShader(vsID);
		ARK_ASSERT_GL();

		glDeleteShader(fsID);
		ARK_ASSERT_GL();

		// initialize shader program object
		return new ShaderProgram(name, programID);
	}

	ShaderProgram::ShaderProgram(const std::string& name, unsigned int id)
		: mName(name)
		, mID(id)
	{
		InitResourceInfo<AttributeTraits>(mID, mAttributes);
		InitResourceInfo<UniformTraits>(mID, mUniforms);
	}

	bool ShaderProgram::SetUniform(const std::string& name, const unsigned int count, const void* const pValues)
	{
		const auto it = mUniforms.find(name);
		if (it == mUniforms.end())
		{
			ARK_ERROR("The shader program '{}' does not have a uniform named '{}'", mName, name);
			return false;
		}

		if (it->second.count != count)
		{
			ARK_ERROR("Provided uniform array size (={}) does not match expected shader uniform array size (={})", count, it->second.count);
			return false;
		}

		switch (it->second.type)
		{
		case GL_BOOL:              glUniform1iv        (it->second.location, it->second.count,           reinterpret_cast<const GLint  *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_BOOL_VEC2:         glUniform2iv        (it->second.location, it->second.count,           reinterpret_cast<const GLint  *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_BOOL_VEC3:         glUniform3iv        (it->second.location, it->second.count,           reinterpret_cast<const GLint  *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_BOOL_VEC4:         glUniform4iv        (it->second.location, it->second.count,           reinterpret_cast<const GLint  *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_INT:               glUniform1iv        (it->second.location, it->second.count,           reinterpret_cast<const GLint  *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_INT_VEC2:          glUniform2iv        (it->second.location, it->second.count,           reinterpret_cast<const GLint  *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_INT_VEC3:          glUniform3iv        (it->second.location, it->second.count,           reinterpret_cast<const GLint  *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_INT_VEC4:          glUniform4iv        (it->second.location, it->second.count,           reinterpret_cast<const GLint  *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_UNSIGNED_INT:      glUniform1uiv       (it->second.location, it->second.count,           reinterpret_cast<const GLuint *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_UNSIGNED_INT_VEC2: glUniform2uiv       (it->second.location, it->second.count,           reinterpret_cast<const GLuint *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_UNSIGNED_INT_VEC3: glUniform3uiv       (it->second.location, it->second.count,           reinterpret_cast<const GLuint *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_UNSIGNED_INT_VEC4: glUniform4uiv       (it->second.location, it->second.count,           reinterpret_cast<const GLuint *>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT:             glUniform1fv        (it->second.location, it->second.count,           reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_VEC2:        glUniform2fv        (it->second.location, it->second.count,           reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_VEC3:        glUniform3fv        (it->second.location, it->second.count,           reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_VEC4:        glUniform4fv        (it->second.location, it->second.count,           reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT2:        glUniformMatrix2fv  (it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT2x3:      glUniformMatrix2x3fv(it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT2x4:      glUniformMatrix2x4fv(it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT3x2:      glUniformMatrix3x2fv(it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT3:        glUniformMatrix3fv  (it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT3x4:      glUniformMatrix3x4fv(it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT4x2:      glUniformMatrix4x2fv(it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT4x3:      glUniformMatrix4x3fv(it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;
		case GL_FLOAT_MAT4:        glUniformMatrix4fv  (it->second.location, it->second.count, GL_FALSE, reinterpret_cast<const GLfloat*>(pValues)); ARK_ASSERT_GL(); return true;

		default:
			ARK_WTF("Unsupported shader resource type");
			return false;;
		}

		return true;
	}

	unsigned int ShaderProgram::CompileShader(const unsigned int type, const std::string& source, const std::string& name)
	{
		const GLuint shaderID = glCreateShader(type);
		ARK_ASSERT_GL();

		if (shaderID == 0)
		{
			ARK_ERROR("glCreateShader failed");
			return 0;
		}

		//SetObjectName(ObjectType::Shader, shaderID, name.c_str());

		const GLchar* shaderSources[] = {
			"#version 330 core\n",
			source.c_str(),
		};

		glShaderSource(shaderID, (GLsizei)ARK_ARRAY_COUNT(shaderSources), shaderSources, nullptr);
		ARK_ASSERT_GL();

		glCompileShader(shaderID);
		ARK_ASSERT_GL();

		GLint compileStatus;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
		ARK_ASSERT_GL();

		if (!compileStatus)
		{
			GLint infoLogLength;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
			ARK_ASSERT_GL();

			std::vector<GLchar> log(infoLogLength);
			glGetShaderInfoLog(shaderID, infoLogLength, nullptr, log.data());
			ARK_ASSERT_GL();

			ARK_ERROR("Failed to compile {} shader {}: '{}'", GLShaderTypeToString(type), name, static_cast<const char*>(log.data()));

			glDeleteShader(shaderID);
			ARK_ASSERT_GL();

			return 0;
		}

		return shaderID;
	}

	unsigned int ShaderProgram::LinkProgram(const unsigned int vsID, const unsigned int fsID, const std::string& name)
	{
		GLuint programID = glCreateProgram();
		if (programID == 0)
		{
			ARK_ERROR("glCreateProgram failed");
			return 0;
		}

		glAttachShader(programID, vsID);
		ARK_ASSERT_GL();

		glAttachShader(programID, fsID);
		ARK_ASSERT_GL();

		glLinkProgram(programID);
		ARK_ASSERT_GL();

		GLint linkStatus;
		glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
		ARK_ASSERT_GL();

		if (!linkStatus)
		{
			GLint infoLogLength;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			ARK_ASSERT_GL();

			std::vector<GLchar> log(infoLogLength);
			glGetProgramInfoLog(programID, infoLogLength, nullptr, log.data());
			ARK_ERROR("Failed to link program {}: '{}'", name, log.data());

			glDeleteProgram(programID);
			ARK_ASSERT_GL();

			return 0;
		}

		return programID;
	}

	template <typename ResourceTraits>
	void ShaderProgram::InitResourceInfo(const unsigned int programID, ShaderResourceMap& resourceMap)
	{
		// preallocate a long enough buffer for uniform names
		GLint maxNameLen;
		glGetProgramiv(programID, ResourceTraits::MAX_LENGTH_FLAG, &maxNameLen);
		ARK_ASSERT_GL();

		std::vector<GLchar> buffer(maxNameLen);

		// query the number of active uniforms
		GLint resourceCount;
		glGetProgramiv(programID, ResourceTraits::COUNT_FLAG, &resourceCount);
		ARK_ASSERT_GL();
		resourceMap.reserve(resourceCount);

		// iterate over all active uniforms
		for (GLint i = 0; i < resourceCount; ++i)
		{
			GLint size;
			GLenum type;
			ResourceTraits::GetResourceInfo(programID, i, maxNameLen, &size, &type, buffer.data());
			ARK_ASSERT_GL();

			const unsigned int location = ResourceTraits::GetResourceLocation(programID, buffer.data());
			ARK_ASSERT_GL();

			ShaderResourceInfo& info = resourceMap[buffer.data()];
			info.location = location;
			info.count = size;
			info.type = type;
		}
	}
}