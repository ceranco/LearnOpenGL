#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace utils {
	class ShaderProgram
	{
		const GLuint _id = glCreateProgram();

		bool getInUse();

	public:
		enum class InputMode
		{
			Source,
			File
		};

	public:
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) = delete;

		ShaderProgram(const std::string& vertexShader, const std::string& fragmenShader, InputMode mode);

		~ShaderProgram();

		template <typename TUniform>
		void setUniform(const std::string& name, TUniform value)
		{
			throw std::exception("Uniform type unsupported");
		}

		template<>
		void setUniform<int>(const std::string& name, int value)
		{
			if (!getInUse())
			{
				throw std::exception("Shader isn't in use");
			}

			glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
		}

		template<>
		void setUniform<float>(const std::string& name, float value)
		{
			if (!getInUse())
			{
				throw std::exception("Shader isn't in use");
			}

			glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
		}

		template<>
		void setUniform<bool>(const std::string& name, bool value)
		{
			if (!getInUse())
			{
				throw std::exception("Shader isn't in use");
			}

			glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
		}

		template<>
		void setUniform<glm::mat4>(const std::string& name, glm::mat4 value)
		{
			if (!getInUse())
			{
				throw std::exception("Shader isn't in use");
			}

			glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, false, glm::value_ptr(value));
		}

		void use();

		GLuint id() const;
	};
}