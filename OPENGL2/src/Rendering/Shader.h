#pragma once
#include <glew.h>
#include <string>
#include <ext/matrix_float4x4.hpp>

class Shader
{
public:
	Shader();
	~Shader();
	void SetActive();
	void Unload();
	bool Load(const std::string& vertName, const std::string& fragName);
	void SetMatrixUniform(const char* name, const glm::mat4& matrix);
	void SetVectorUniform(const char* name, const glm::vec3& vec);
	void SetVectorUniform(const char* name, float x, float y, float z);
	void SetFloatUniform(const char* name, const float value);
private:
	bool CompileShader(const std::string& fileName,
		GLenum shaderType, GLuint& outShader);
	// Tests whether shader compiled successfully
	bool IsCompiled(GLuint shader);
	// Tests whether vertex/fragment programs link
	bool IsValidProgram();
private:
	// Store the shader object IDs
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;

};

