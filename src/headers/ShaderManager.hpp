#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;

template <typename T> class SingletonHolder;

class ShaderManager { //will not keep track of shaders for now
	public:
		GLuint createShader(GLenum shaderType, const char*);
		void resetStatus();
		compl ShaderManager();

		friend class SingletonHolder<ShaderManager>;
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager(ShaderManager&&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;
	private:
		ShaderManager();
		void readShaderSource(const char*);
		void compileShader();
	private:
		GLenum m_shaderType;
		const char* m_shaderSource;
		size_t m_shaderSize;
		GLuint m_shaderName;
};

#endif