#pragma once
#include <glew.h>
#include <string>
#include <ext/matrix_float4x4.hpp>
#include<unordered_map>
class Shader
{
public:
	Shader();
	Shader(const std::string& vertName, const std::string& fragName, const std::string& geoName="");
	~Shader();
	void Bind();
	void Unload();
	bool Load(const std::string& vertName, const std::string& fragName);
	bool Load(const std::string& vertName, const std::string& fragName, const std::string& geoName);
	void SetMatrixUniform(const char* name, const glm::mat4& matrix);
	void SetVectorUniform(const char* name, const glm::vec3& vec);
	GLuint GetUniform(const std::string& uniformName);
	void SetVectorUniform(const char* name, float x, float y, float z);
	void SetFloatUniform(const char* name, const float value);
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), value);
	}
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
	GLuint mGeometryShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
	std::unordered_map < std::string,GLuint> uniformsCache;

};

