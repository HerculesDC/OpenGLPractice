#define SDL_MAIN_HANDLED
#include "headers/App.hpp"
#include "headers/WindowManager.hpp"
#include "headers/SingletonHolder.h"

#include "SDL/SDL.h"
#include "GLAD/glad.h"

template <typename T> T& SingletonHolder<T>::s_instance = *(new T());
App::App() : m_bIsRun(false),
			 m_bStarted(false),
			 m_iCurrentDelta(0),
			 m_iFPS(60),
			 m_iTargetDelta(0),
			 m_iStart(0),
			 m_iEnd(0),
			 m_iTimeSinceStarted(0)
{

}

App::compl App(){}

void App::run() {
	if (init()) {
		while (m_bIsRun) {
			processInput();
			update(0);
			render();
		}
	}
	cleanup();
}

bool App::init() {
	if (m_bStarted) return false; //already init
	m_bStarted = true;
	m_bIsRun = SingletonHolder<WindowManager>::s_instance.init();

	if (!m_bIsRun) return m_bIsRun;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext ctx = SDL_GL_CreateContext(SingletonHolder<WindowManager>::s_instance.getWindow());
	SDL_GL_MakeCurrent(SingletonHolder<WindowManager>::s_instance.getWindow(), ctx);
	m_bIsRun = ctx != nullptr;

	if (!m_bIsRun) return m_bIsRun;

	int result = gladLoadGLLoader((GLADloadproc)(SDL_GL_GetProcAddress));
	m_bIsRun = result > 0;

	return m_bIsRun;
}

void App::processInput(){
	SDL_Event evt;
	while (SDL_PollEvent(&evt)) {
		if (evt.type == SDL_QUIT) {
			quit();
		}
	}
}

void App::update(Uint64 delta){}

void App::render() {
	glClearColor(0.1f, 0.1f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(SingletonHolder<WindowManager>::s_instance.getWindow());
}

void App::wake() {
	m_iTimeSinceStarted = m_iStart = SDL_GetTicks64();
}

void App::wait() {
	m_iEnd = SDL_GetTicks64();
	m_iCurrentDelta = m_iEnd - m_iStart;
	if (m_iCurrentDelta < m_iTargetDelta) {
		SDL_Delay(m_iTargetDelta - m_iCurrentDelta);
	}
}

void App::quit() { m_bIsRun = false; }

void App::cleanup() {}