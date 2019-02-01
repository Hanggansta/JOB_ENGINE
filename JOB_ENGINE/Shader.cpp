#include "Shader.h"
#include<fstream>
#include<sstream>
#include<iostream>
Shader::Shader()
{
}
Shader::Shader(std::string ShaderName,const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	this->ShaderName = ShaderName;
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	
	checkCompileErrors(vertex, "VERTEX");
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	
	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}
Shader::Shader(std::string ShaderName, const char* vertexPath, const char* tessConPath, const char* tessEvaPath, const char* fragmentPath, const char* geometryPath)
{
	this->ShaderName = ShaderName;
	std::string vertexCode;
	std::string tcCode;
	std::string teCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream tcShaderFile;
	std::ifstream teShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		tcShaderFile.open(tessConPath);
		teShaderFile.open(tessEvaPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream,tcShaderStream,teShaderStream,fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		tcShaderStream << tcShaderFile.rdbuf();
		teShaderStream << teShaderFile.rdbuf();		
		fShaderStream << fShaderFile.rdbuf();

	
		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		tcCode = tcShaderStream.str();
		teCode = teShaderStream.str();
		fragmentCode = fShaderStream.str();
		
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* tcShaderCode = tcCode.c_str();
	const char* teShaderCode = teCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	GLuint vertex,tessControl,tessEvaluation, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);	
	glShaderSource(tessControl, 1, &tcShaderCode, NULL);
	glCompileShader(tessControl);
	checkCompileErrors(tessControl, "TESS_CONTROL");


	tessEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tessEvaluation, 1, &teShaderCode, NULL);
	glCompileShader(tessEvaluation);
	checkCompileErrors(tessEvaluation, "TESS_EVALUATION");
	
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, tessControl);
	glAttachShader(ID, tessEvaluation);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}
void Shader::Use()
{
	glUseProgram(this->ID);
}
void Shader::checkCompileErrors(GLuint Shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(Shader, 1024, NULL, infoLog);
			std::cout << "SHADER NAME : "<<this->ShaderName<<" ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(Shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(Shader, 1024, NULL, infoLog);
			std::cout << "SHADER NAME : " << this->ShaderName <<" ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}


}
