#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <gtc/type_ptr.hpp>
#include "Debuger.h"

Shader::Shader()
{
}

Shader::~Shader()
{
	Unload();
}

void Shader::SetMatrixUniform(const char* name, const glm::mat4& matrix)
{
	// Find the uniform by this name
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the matrix data to the uniform
	GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetVectorUniform(const char* name, const glm::vec3& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the vector data
	glUniform3fv(loc, 1, glm::value_ptr(vector));
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the float data
	glUniform1f(loc, value);
}

void Shader::SetActive()
{
	GLCall(glUseProgram(mShaderProgram));
}

void Shader::Unload()
{
	// Delete the program/shaders
	GLCall(glDeleteProgram(mShaderProgram));
	GLCall(glDeleteShader(mVertexShader));
	GLCall(glDeleteShader(mFragShader));
}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	bool isFragment = CompileShader(fragName,
		GL_FRAGMENT_SHADER,
		mFragShader);
	bool isVertex = CompileShader(vertName,
		GL_VERTEX_SHADER,
		mVertexShader);
	if (!isFragment || !isVertex)
	{
		return false;
	}

	// Now create a shader program that
	// links together the vertex/frag shaders
	mShaderProgram = glCreateProgram();
	GLCall(glAttachShader(mShaderProgram, mVertexShader));
	GLCall(glAttachShader(mShaderProgram, mFragShader));
	GLCall(glLinkProgram(mShaderProgram));

	// Verify that the program linked successfully
	if (!IsValidProgram())
	{
		std::cout << "failed to link shaders" << "\n";
		return false;
	}

	return true;
}

bool Shader::CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	// Open file
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// Read all the text into a string
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		// Create a shader of the specified type
		outShader = glCreateShader(shaderType);
		// Set the source characters and try to compile
		GLCall(glShaderSource(outShader, 1, &(contentsChar), nullptr));
		GLCall(glCompileShader(outShader));
		if (!IsCompiled(outShader))
		{
			std::cout << ("Failed to compile shader %s", fileName.c_str()) << "\n";
			return false;
		}
	}
	else
	{
		std::cout <<("Shader file not found: %s", fileName.c_str()) << "\n";
		return false;
	}
	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// Query the compile status
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		GLCall(glGetShaderInfoLog(shader, 511, nullptr, buffer));
		std::cout << ("GLSL Compile Failed:\n%s", buffer) << "\n";
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	// Query the link status
	GLCall(glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status));
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		GLCall(glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer));
		std::cout << ("GLSL Link Status:\n%s", buffer) << "\n";
		return false;
	}

	return true;
}
