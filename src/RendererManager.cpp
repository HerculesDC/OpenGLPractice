#include "headers/RendererManager.hpp"
#include "headers/ShaderManager.hpp"
#include "headers/SingletonHolder.hpp"

#include "GLAD/glad.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/constants.hpp"

template <typename T> T& SingletonHolder<T>::s_instance = *(new T());

RendererManager::RendererManager(): m_pipeline(0), m_VAO(0), m_VBO(0), m_IBO(0),
									m_TEX(0), m_TEX2(0), m_tVAO(0), m_tVBO(0), 
									m_tau(glm::two_pi<float>()), m_imgAngle(0), m_starAngle(0),
									m_imgSpeed(.25f), m_starSpeed(.1f) {
	m_shaders.reserve(3);
	m_programs.reserve(3);
}

RendererManager::compl RendererManager() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteVertexArrays(1, &m_tVAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_tVBO);
	glDeleteBuffers(1, &m_IBO);
}

bool RendererManager::init(){
	if (!setupShaders()) return false;
	if (!createProgramPipeline()) return false;
	if (!initGLObjects()) return false;
	return true;
}

void RendererManager::update(Uint64 delta){
	m_starAngle += m_starSpeed * (delta / 1000.f) - ((m_tau) * ((m_starAngle >= m_tau) - (m_starAngle < 0)));
	m_imgAngle += m_imgSpeed * (delta / 1000.f) - ((m_tau) * ((m_imgAngle >= m_tau) - (m_imgAngle < 0)));

	//std::cout << "Delta: " << delta << "Image speed: " << m_imgSpeed << ", image angle: " << m_imgAngle << std::endl;
}

void RendererManager::render() {
	glClearColor(0.1f, 0.1f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindProgramPipeline(m_pipeline);
	glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_programs[0]);
	glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_programs[2]);

	//refresher: program, location, value
	glProgramUniform1f(m_programs[2], 4, m_imgAngle);

	//for reference: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glProgramUniform.xhtml
	auto transfMat4 = glm::mat4(1.f);
	auto vec3z = glm::vec3(0.f, 0.f, 1.f);
	transfMat4 = glm::rotate(transfMat4, m_starAngle, vec3z);

	//std::cout << "Vertex matrix data:\n" << glm::to_string(transfMat4) << std::endl;
	
	//refresher: program, location, number of elements, should transpose, pointer to data
	glProgramUniformMatrix4fv(m_programs[0], 2, 1, GL_FALSE, glm::value_ptr(transfMat4));

	auto imgMat = glm::mat4(1.f);
	auto imgOffset = glm::vec3(.5f, .5f, 0.f);
	imgMat = glm::translate(imgMat, imgOffset);

	/*********************************************************************************************
	 * NOTE:
	 * m_starAngle is used because they both refer to vertices. In Uniform location 2 we store star vertices,
	 *		but in location 3 we store (star-shaped) sampling coordinates. If the sampling points remain fixed
	 *		to the image they sample, when we rotate the drawing, the image will look rotated. The calls to
	 *		translate are also used because the sampling points are normalized to [0,1], meaning that 
	 *		what we see as the center of the image is actually its (0.5, 0.5) point. We need to set the center
	 *		of rotation of the sampling to (0.5, 0.5), rotate the sampling points, and then return the origin
	 *		to its initial state
	 *********************************************************************************************/
	imgMat = glm::rotate(imgMat, m_starAngle, vec3z);
	
	imgMat = glm::translate(imgMat, -imgOffset);
	//std::cout << "Image matrix data:\n" << glm::to_string(imgMat) << std::endl;
	glProgramUniformMatrix4fv(m_programs[0], 3, 1, GL_FALSE, glm::value_ptr(imgMat));
	
	glBindVertexArray(m_VAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_indices.size());
	
	glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_programs[1]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_indices.size());

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
	
	//VERTEX SETUP 
	//vertices will be passed in a vec4 packing style.
	//central point
	m_vertices.push_back(0.f);
	m_vertices.push_back(0.f);
	m_vertices.push_back(0.f);
	m_vertices.push_back(1.f);
	//central text coord
	m_texCoords.push_back(.5f);
	m_texCoords.push_back(.5f);
	//first index
	m_indices.push_back(0);

	auto sides = 7;
	auto d_sides = sides << 1;
	auto angle = glm::two_pi<float>()/d_sides;

	auto spokeLength = 0.9f;
	auto indentLength = spokeLength / 2.5;
	
	for (int i = 0; i <= d_sides; ++i) { //less than or equal so that it generates the last triangle

		//CAREFUL WITH LOGICAL OPERATIONS IN MULTIPLICATIONS!
		auto xCoord = glm::sin(i * angle) * ((i % 2 == 0) ? spokeLength : indentLength);
		m_vertices.push_back(xCoord);
		m_texCoords.push_back(.5f + (.5f * xCoord));
		
		auto yCoord = glm::cos(i * angle) * ((i % 2 == 0) ? spokeLength : indentLength);
		m_vertices.push_back(yCoord);
		m_texCoords.push_back(.5f + (.5f * yCoord));

		m_vertices.push_back(0.f);
		m_vertices.push_back(1.f);

		m_indices.push_back(i + 1);
	}
	m_vertices.shrink_to_fit();
	m_indices.shrink_to_fit();
	m_texCoords.shrink_to_fit();
	
	//Gl proper setup
	auto vertSize = GLsizei(4);
	auto elemNum = GLsizei(m_indices.size()); //could've also been m_vertices.size()/vertSize;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, vertSize, GL_FLOAT, GL_FALSE, vertSize*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//glGenVertexArrays(1, &m_tVAO);
	glGenBuffers(1, &m_tVBO);

	//glBindVertexArray(m_tVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_tVBO);
	glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(float), m_texCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(0));
	glEnableVertexAttribArray(1);

	auto img = IMG_Load("res/img/wall.jpg");
	glGenTextures(1, &m_TEX);
	//std::cout << "Wall Texture name: " << m_TEX << std::endl;
	glActiveTexture(GL_TEXTURE1);
	//int active;
	//glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
	//std::cout << "Currently active texture: " << active << std::endl;
	glBindTexture(GL_TEXTURE_2D, m_TEX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//std::cout << SDL_GetPixelFormatName(img->format->format) << std::endl; //READS IMAGE FORMAT
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
	glGenerateMipmap(GL_TEXTURE_2D); //has to be set
	SDL_FreeSurface(img);

	auto img2 = IMG_Load("res/img/lachesis.png");
	//std::cout << SDL_GetPixelFormatName(img->format->format) << std::endl;
	glGenTextures(1, &m_TEX2);
	//std::cout << "Snake Texture name: " << m_TEX2 << std::endl;
	glActiveTexture(GL_TEXTURE2);
	//glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
	//std::cout << "Currently active texture: " << active << std::endl;
	glBindTexture(GL_TEXTURE_2D, m_TEX2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img2->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(img2);

	//glBindProgramPipeline(m_pipeline);
	//glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_programs[2]);

	//NOTE: Accessing pipeline uniforms requires specific GL calls!!!
	//Also, the program does not have to be in use (it seems)
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_TEX);
	glProgramUniform1i(m_programs[2], 5, 1); //set texture unit 0 to location 5 (sampler2D floorTexture)
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_TEX2);
	glProgramUniform1i(m_programs[2], 6, 2); //set texture unit 1 to location 6 (sampler2D snakeTexture)

	//glEnable(GL_LINE_SMOOTH); //antialiasing
	//glLineWidth(3.f); //self-explanatory

	/*auto texName = "floorTexture";
	std::cout << texName << " Uniform location : " << glGetUniformLocation(m_programs[2], texName) << std::endl;
	texName = "snakeTexture";
	std::cout << texName << " Uniform location : " << glGetUniformLocation(m_programs[2], texName) << std::endl;*/

	return true;
}