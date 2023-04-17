#ifndef RENDERERMANAGER_HPP
#define RENDERERMANAGER_HPP
#include <vector>

#include "glm/vec3.hpp"
#include "GeometryGenerator.hpp"
#include "Camera.hpp"

typedef unsigned long long int Uint64;
typedef unsigned int GLuint;
typedef int GLint;

template <typename T> class SingletonHolder;

class RendererManager {
	public:
		RendererManager(const RendererManager&) = delete;
		RendererManager(RendererManager&&) = delete;
		RendererManager& operator=(const RendererManager&) = delete;
		compl RendererManager();
	public:
		friend class SingletonHolder<RendererManager>;
		Camera& getCamera();
		bool init();
		void update(Uint64);
		void render();
	private:
		RendererManager();
	private:
		bool setupShaders();
		bool createProgramPipeline();
		bool initGLObjects();
	private:
		std::vector<GLuint> m_shaders; //needed to transfer info between methods
		std::vector<GLuint> m_programs; //separable, one per shader
		GLuint m_pipeline; //only one for now

		Camera m_camera;

		GeometryGenerator m_gg;
		std::vector<glm::vec3> m_pos;

		//remove later
		float m_tau, m_imgAngle, m_starAngle, m_imgSpeed, m_starSpeed;
};

#endif