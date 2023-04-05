#include "headers/WindowManager.hpp"
#include "SDL/SDL_video.h"

WindowManager::WindowManager() : m_window(nullptr)
{

}

bool WindowManager::init() {
	m_window = SDL_CreateWindow("OpenGL Practice", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	return m_window != nullptr;
}

SDL_Window* WindowManager::getWindow() const { return m_window; }