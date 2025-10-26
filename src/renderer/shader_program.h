#pragma once

#include "core/common.h"
#include "core/maths.h"

#include <string>
#include <unordered_map>


namespace ark
{
	class Texture;
	class TextureCube;

	class ShaderProgram
	{
	private:
		struct AttributeTraits;
		struct UniformTraits;

		struct ShaderResourceInfo
		{
			unsigned int location;
			unsigned int count;
			unsigned int type;
		};

		typedef std::unordered_map<std::string, ShaderResourceInfo> ShaderResourceMap;

	public:
		const std::string& GetName() const { return mName; }

		bool HasUniform(const std::string& name) const { return mUniforms.contains(name); }

		void Use();

		bool SetBool(const std::string& name, const bool value) { const int intValue = value; return SetUniform(name, 1, &intValue); }
		bool SetInt(const std::string& name, const int value) { return SetUniform(name, 1, &value); }
		bool SetUInt(const std::string& name, const unsigned int value) { return SetUniform(name, 1, &value); }
		bool SetFloat(const std::string& name, const float value) { return SetUniform(name, 1, &value); }
		bool SetVector(const std::string& name, const glm::vec2& value) { return SetUniform(name, 1, glm::value_ptr(value)); }
		bool SetVector(const std::string& name, const glm::vec3& value) { return SetUniform(name, 1, glm::value_ptr(value)); }
		bool SetVector(const std::string& name, const glm::vec4& value) { return SetUniform(name, 1, glm::value_ptr(value)); }
		bool SetVectorArray(const std::string& name, const std::vector<glm::vec2>& values) { return SetUniform(name, static_cast<unsigned int>(values.size()), glm::value_ptr(values[0])); }
		bool SetVectorArray(const std::string& name, const std::vector<glm::vec3>& values) { return SetUniform(name, static_cast<unsigned int>(values.size()), glm::value_ptr(values[0])); }
		bool SetVectorArray(const std::string& name, const std::vector<glm::vec4>& values) { return SetUniform(name, static_cast<unsigned int>(values.size()), glm::value_ptr(values[0])); }
		bool SetMatrix(const std::string& name, const glm::mat2& value) { return SetUniform(name, 1, glm::value_ptr(value)); }
		bool SetMatrix(const std::string& name, const glm::mat3& value) { return SetUniform(name, 1, glm::value_ptr(value)); }
		bool SetMatrix(const std::string& name, const glm::mat4& value) { return SetUniform(name, 1, glm::value_ptr(value)); }

		static ShaderProgram* Create(
			const std::string& name,
			const std::string& vertShaderName,
			const std::string& vertShaderSource,
			const std::string& fragShaderName,
			const std::string& fragShaderSource);

	private:
		explicit ShaderProgram(const std::string& name, unsigned int id);
		bool SetUniform(const std::string& name, const unsigned int count, const void* const pValues);

	private:
		static unsigned int CompileShader(const unsigned int type, const std::string& source, const std::string& name);
		static unsigned int LinkProgram(const unsigned int vsID, const unsigned int fsID, const std::string& name);
		template <typename ResourceTraits>
		static void InitResourceInfo(const unsigned int programID, ShaderResourceMap& resourceMap);

	private:
		unsigned int mProgramID = 0;    //! opengl's handle to the shader program
		std::string mName;              //! name of the shader program (mostly for debugging)
		ShaderResourceMap mAttributes;  //! input attributes required by the program
		ShaderResourceMap mUniforms;    //! uniforms required by this shader program
	};
}
