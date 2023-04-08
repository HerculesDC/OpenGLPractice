#ifndef RENDERERMANAGER_HPP
#define RENDERERMANAGER_HPP
#include <vector>

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
		std::vector<GLuint> m_programs; //separable
		GLuint m_pipeline; //only one for now

		GLuint m_VAO, m_VBO, m_IBO, m_TEX, m_TEX2, m_tVAO, m_tVBO;
		std::vector<float> m_vertices;
		std::vector<GLuint> m_indices;
		std::vector<float> m_texCoords;

		//remova later
		float m_tau, m_imgAngle, m_starAngle, m_imgSpeed, m_starSpeed;

};

#endif