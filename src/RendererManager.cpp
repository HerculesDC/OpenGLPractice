#include "headers/RendererManager.hpp"
#include "headers/ShaderManager.hpp"
#include "headers/SingletonHolder.hpp"
#include "headers/GeometryGenerator.hpp"

#include "GLAD/glad.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/constants.hpp"

template <typename T> T& SingletonHolder<T>::s_instance = *(new T());

RendererManager::RendererManager(): m_pipeline(0), m_tau(glm::two_pi<float>()), m_imgAngle(0), m_starAngle(0),
									m_imgSpeed(.25f), m_starSpeed(.1f), m_gg(GeometryGenerator()),
									m_camera(Camera())
{
	m_shaders.reserve(3);
	m_programs.reserve(3);
	m_pos.reserve(10);
}

RendererManager::compl RendererManager() {}

Camera& RendererManager::getCamera() { return m_camera; }

bool RendererManager::init(){
	if (!setupShaders()) return false;
	if (!createProgramPipeline()) return false;
	if (!initGLObjects()) return false;
	return true;
}

float cycle(float current, float amt, float min, float max) { 
	return current + amt - ((max - min)*(current >= max)-(current < min));
}

void RendererManager::update(Uint64 delta){
	auto timeFactor = delta / 1000.f;
	m_starAngle = cycle(m_starAngle, m_starSpeed*timeFactor, 0, m_tau);
	m_imgAngle = cycle(m_imgAngle, m_imgSpeed*timeFactor, 0, m_tau);
	m_camera.updateCamera();
}

void RendererManager::render() {
	glClearColor(0.1f, 0.1f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//model, view, projection matrices
	auto model = glm::mat4(1.f);
	model = glm::rotate(model, -1.1f, glm::vec3(1.f, 0.f, 0.f));

	glBindProgramPipeline(m_pipeline);
	glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_programs[0]);

	//uniform set for model-view-projection (vertex shader program)
	//for reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glProgramUniform.xhtml
	glProgramUniformMatrix4fv(m_programs[0], 0, 1, GL_FALSE, m_camera.getCameraProjectionData());
	glProgramUniformMatrix4fv(m_programs[0], 1, 1, GL_FALSE, m_camera.getCameraViewData());
	glProgramUniformMatrix4fv(m_programs[0], 2, 1, GL_FALSE, glm::value_ptr(model));


	Geometry geom = m_gg.getGeometry("Hexahedron");
	glBindVertexArray(geom.vao);

	for (size_t i = 0; i < m_pos.size(); ++i) {

		glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_programs[1+(i%2)]);
		
		glProgramUniform1f(m_programs[2], 7, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (i % 2 == 0) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		auto transfMat4 = glm::mat4(1.f);
		transfMat4 = glm::translate(transfMat4, m_pos[i]);
		auto angle = m_starAngle;
		if (i % 3 == 0) {
			angle *= -5;
		}
		else if (i % 2 == 0) {
			angle = 0;
		}
		transfMat4 = glm::rotate(transfMat4, angle, glm::vec3(0.f, 0.f, 1.f));
		transfMat4 = glm::scale(transfMat4, glm::vec3(.5f));

		//refresher: program, location, number of elements, should transpose, pointer to data
		glProgramUniformMatrix4fv(m_programs[0], 5, 1, GL_FALSE, glm::value_ptr(transfMat4)); //shader00.vert program, location 5 (vertex transformation matrix)

		glDrawElements(GL_TRIANGLE_STRIP, geom.ibo_sz, GL_UNSIGNED_INT, 0);
	}
	
	glBindVertexArray(0);
}

bool RendererManager::setupShaders(){ //simply reads sources and compiles them. Reuses variable (check for clarity)

	auto shaderName = SingletonHolder<ShaderManager>::s_instance.createShader(GL_VERTEX_SHADER, "res/shaders/shader00.vert");
	m_shaders.push_back(shaderName);
	SingletonHolder<ShaderManager>::s_instance.resetStatus();
	
	shaderName = SingletonHolder<ShaderManager>::s_instance.createShader(GL_FRAGMENT_SHADER, "res/shaders/shader00.frag");
	m_shaders.push_back(shaderName);
	SingletonHolder<ShaderManager>::s_instance.resetStatus();
	
	shaderName =  SingletonHolder<ShaderManager>::s_instance.createShader(GL_FRAGMENT_SHADER, "res/shaders/shader01.frag");
	m_shaders.push_back(shaderName);
	SingletonHolder<ShaderManager>::s_instance.resetStatus();

	m_shaders.shrink_to_fit();

	return true;
}

bool RendererManager::createProgramPipeline(){
	
	for (size_t i = 0; i < m_shaders.size(); ++i) {
		auto program = glCreateProgram();
		glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glAttachShader(program, m_shaders[i]);
		glLinkProgram(program);

		int success;
		const int logSize = 512;
		GLchar progLog[logSize];
		glGetProgramiv(program, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, logSize, nullptr, progLog);
			std::cerr << "program link error: " << progLog << std::endl;
			return false;
		}
		m_programs.push_back(program);
		//can delete shader, ain't doing for now
	}
	glGenProgramPipelines(1, &m_pipeline);
	return true;
}

bool RendererManager::initGLObjects(){	
	
	Geometry g = m_gg.generateGeometry("Hexahedron", 3); //Vertex Shader vertex attribute location;

	m_pos.push_back(glm::vec3(  0.f,  0.f,  0.f));
	m_pos.push_back(glm::vec3(  .5f,  .3f,  .7f));
	m_pos.push_back(glm::vec3( -.5f, -.3f, 1.5f));
	m_pos.push_back(glm::vec3( 1.5f,  .5f,  1.f));
	m_pos.push_back(glm::vec3(-1.5f,  .5f, 1.7f));
	m_pos.push_back(glm::vec3(  .5f, -.7f,  .7f));
	m_pos.push_back(glm::vec3( -.5f, -.3f,  .7f));
	m_pos.push_back(glm::vec3( .25f,  .3f, .27f));
	m_pos.push_back(glm::vec3(  .5f,-1.3f,  .0f));
	m_pos.push_back(glm::vec3(-1.5f, -.5f,  .7f));

	glBindVertexArray(g.vao);
	glGenBuffers(1, &m_tVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_tVBO);
	glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(float), m_texCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0)); //vertex attrib pointer at location 4 (texInData)
	glEnableVertexArrayAttrib(g.vao, 4);

	auto img = IMG_Load("res/img/wall.jpg");
	glGenTextures(1, &m_TEX);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_TEX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
	glGenerateMipmap(GL_TEXTURE_2D); //has to be set
	SDL_FreeSurface(img); //CLEANUP

	glProgramUniform1i(m_programs[2], 8, 1); //set texture unit 1 to location 8 (sampler2D floorTexture)

	auto img2 = IMG_Load("res/img/lachesis.png");
	glGenTextures(1, &m_TEX2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_TEX2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img2->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(img2);

	glProgramUniform1i(m_programs[2], 9, 2); //set texture unit 2 to location 9 (sampler2D snakeTexture)

	glEnable(GL_DEPTH_TEST); //this is what causes face occlusion

	return true;
}