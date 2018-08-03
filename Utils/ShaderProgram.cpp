#include "ShaderProgram.h"
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace utils;

bool utils::ShaderProgram::getInUse()
{
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	return id == _id;
}

ShaderProgram::ShaderProgram(const std::string& vertexShaderInput, const std::string& fragmentShaderInput, InputMode mode)
{
	auto getShaderSource = [](const std::string& shaderInput, InputMode mode) {
		std::string shaderSource;
		if (mode == InputMode::Source)
		{
			shaderSource = shaderInput;
		}
		else
		{
			std::ifstream fs{ shaderInput };
			shaderSource = { (std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>() };
			fs.close();
		}

		return shaderSource;
	};
	auto vertexSource = getShaderSource(vertexShaderInput, mode);
	auto fragmentSource = getShaderSource(fragmentShaderInput, mode);

	auto vertexSourceCStr = vertexSource.c_str();
	auto fragmentSourceCStr = fragmentSource.c_str();

	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
	glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	auto checkCompilation = [](GLuint shader, const char* errorMsg) {
		int success;
		char infoLog[512];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cout << errorMsg << "\n\t" << infoLog << std::endl;
			return false;
		}
		return true;
	};
	checkCompilation(vertexShader, "ERROR: VERTEX SHADER COMPILATION FAILED");
	checkCompilation(fragmentShader, "ERROR: VERTEX FRAGMENT COMPILATION FAILED");

	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);
	glLinkProgram(_id);

	auto checkLinking = [](GLuint program, const char* errorMsg) {
		int success;
		char infoLog[512];

		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			std::cout << errorMsg << "\n\t" << infoLog << std::endl;;

			return false;
		}
		return true;
	};
	checkLinking(_id, "ERROR: SHADER PROGRAM LINKING FAILED");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);

}

void ShaderProgram::use()
{
	glUseProgram(_id);
}

GLuint ShaderProgram::id() const
{
	return _id;
}
