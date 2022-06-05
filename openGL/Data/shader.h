#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// utility function for checking shader compilation/linking errors.
void checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

class Shader
{
public:
	Shader(const char* shaderfilepath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string shaderCodeStr;
		std::ifstream shaderFile;
		// ensure ifstream objects can throw exceptions:
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			shaderFile.open(shaderfilepath);
			std::stringstream shaderStream;
			// read file's buffer contents into streams
			shaderStream << shaderFile.rdbuf();
			// close file handlers
			shaderFile.close();
			shaderCodeStr = shaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}
		_shaderCode = shaderCodeStr.c_str();
	}
	unsigned int getID() { return ID; }
	void destory()
	{
		glDeleteShader(ID);
	}
protected:
	const char* _shaderCode;
	unsigned int ID;
};

class VertShader :public Shader
{
public:
	VertShader(const char* vertexPath) :Shader(vertexPath)
	{
		// vertex shader
		ID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(ID, 1, &_shaderCode, NULL);
		glCompileShader(ID);
		checkCompileErrors(ID, "VERTEX");
	}
	~VertShader() {}
};

class FragShader :public Shader
{
public:
	FragShader(const char* fragmentPath) :Shader(fragmentPath)
	{
		// fragment Shader
		ID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ID, 1, &_shaderCode, NULL);
		glCompileShader(ID);
		checkCompileErrors(ID, "FRAGMENT");
	}
};

class GeomShader :public Shader
{
public:
	GeomShader(const char* geometryPath) :Shader(geometryPath)
	{
		unsigned int geometry;
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &_shaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
};

class ShaderProgram
{
public:
	ShaderProgram()
	{
		ID = glCreateProgram();
	}
	ShaderProgram(unsigned int vertShader, unsigned int fragShader) :ShaderProgram()
	{
		attachShader(vertShader, fragShader);
		checkCompileErrors(ID, "PROGRAM");
	}
	ShaderProgram(unsigned int vertShader, unsigned int fragShader, unsigned geoShader) :ShaderProgram()
	{
		attachShader(vertShader, fragShader);
		checkCompileErrors(ID, "PROGRAM");
	}
	~ShaderProgram() {}
public:
	template<typename ...T>
	void attachShader(unsigned int shader, const T&...args)
	{
		glAttachShader(ID, shader);
		glDeleteShader(shader);
		attachShader(args...);
	}
	// activate the shader
	void use()
	{
		glUseProgram(ID);
	}

	// utility uniform functions
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
private:
	void attachShader(unsigned int shader)
	{
		glAttachShader(ID, shader);
		glDeleteShader(shader);
		glLinkProgram(ID);
	}

public:
	unsigned int getId() { return ID; }
private:
	unsigned int ID;

};

#endif