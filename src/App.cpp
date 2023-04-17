#define SDL_MAIN_HANDLED
#include "headers/App.hpp"
#include "headers/WindowManager.hpp"
#include "headers/RendererManager.hpp"
#include "headers/SingletonHolder.hpp"
#include "headers/InputProcessor.hpp"
#include "headers/Command.hpp"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
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
	m_iTargetDelta = (int)(1000.f / m_iFPS);
}

App::compl App(){}

void App::run() {
	if (init()) {
		while (m_bIsRun) {
			wake();
			processInput();
			update(SDL_GetTicks64()-m_iEnd);
			render();
			wait();
		}
	}
	cleanup();
}

bool App::init() {
	if (m_bStarted) return false; //already init
	m_bStarted = true;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

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

	if (!m_bIsRun) return m_bIsRun;

	m_bIsRun = SingletonHolder<RendererManager>::s_instance.init();
	
	if (!m_bIsRun) return m_bIsRun;
	

	return m_bIsRun;
}

void App::processInput(){
	SDL_Event evt;
	InputProcessor inp;
	while (SDL_PollEvent(&evt)) {
		if (evt.type == SDL_QUIT) {
			quit();
		}
		if (evt.type == SDL_KEYDOWN) {
			auto cmd = inp.handleInput(evt.key);
			if (cmd) {
				decltype(auto) cam = SingletonHolder<RendererManager>::s_instance.getCamera();
				auto actualCommand = cmd->execute(cam);
				actualCommand();
			}
		}
	}
}

void App::update(Uint64 delta){
	SingletonHolder<RendererManager>::s_instance.update(delta);
}

void App::render() {
	
	SingletonHolder<RendererManager>::s_instance.render();
	SDL_GL_SwapWindow(SingletonHolder<WindowManager>::s_instance.getWindow());
}

void App::wake() {
	m_iTimeSinceStarted = m_iStart = SDL_GetTicks64();
}

void App::wait() {
	m_iEnd = SDL_GetTicks64();
	m_iCurrentDelta = m_iEnd - m_iStart;
	//bit of branchless programming here
	SDL_Delay((m_iTargetDelta - m_iCurrentDelta) * (m_iCurrentDelta < m_iTargetDelta));
}

void App::quit() { m_bIsRun = false; }

void App::cleanup() {
	IMG_Quit();
	SDL_Quit();
}