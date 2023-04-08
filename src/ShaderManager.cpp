#include "headers/ShaderManager.hpp"
#include "GLAD/glad.h"
#include "SDL/SDL.h"


ShaderManager::ShaderManager() : m_shaderName(0), m_shaderSize(0), m_shaderType(GL_INVALID_VALUE)
{
	m_shaderSource = "";
}

ShaderManager::compl ShaderManager() {
	resetStatus();
	delete m_shaderSource;
}

#include <iostream>
void ShaderManager::readShaderSource(const char* filename) {
	m_shaderSource = (const char*)SDL_LoadFile(filename, &m_shaderSize);
	if (m_shaderSource == nullptr || m_shaderSource == "") {
		std::cerr << "No Shader Source... SDL Log: " << SDL_GetError() << std::endl;
	}
	glShaderSource(m_shaderName, 1, &m_shaderSource, nullptr);
}

void ShaderManager::compileShader() {
	glCompileShader(m_shaderName);
	int success;
	const int logSize = 512;
	GLchar shaderLog[logSize];
	glGetShaderiv(m_shaderName, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(m_shaderName, logSize, nullptr, shaderLog);
		std::cerr << "Shader compilation error: " << shaderLog << std::endl;
	}
}

GLuint ShaderManager::createShader(GLenum shaderType, const char* filename) {
	m_shaderName = glCreateShader(shaderType);
	if (!glIsShader(m_shaderName)) {
		std::cerr << "not created" << std::endl;
	}
	readShaderSource(filename);
	compileShader();
	return m_shaderName;
}

void ShaderManager::resetStatus(){
	m_shaderSource = "";
	m_shaderName = 0;
	m_shaderSize = 0;
	m_shaderType = GL_INVALID_VALUE;
}