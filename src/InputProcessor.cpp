#include "SDL/SDL_events.h"

#include "headers/InputProcessor.hpp"
#include "headers/Command.hpp"

InputProcessor::InputProcessor() {
	m_fwdCommand = new ForwardCommand();
	m_stfCommand = new StrafeCommand();
	m_ascCommand = new AscendCommand();
}

InputProcessor::compl InputProcessor() {
	delete m_ascCommand;
	delete m_stfCommand;
	delete m_fwdCommand;
}

Command* InputProcessor::handleInput(SDL_KeyboardEvent evt) {
	if (evt.type == SDL_KEYDOWN) {
		if (evt.keysym.scancode == SDL_SCANCODE_W)
		{
			m_fwdCommand->setIntensity(1.f);
			return m_fwdCommand;
		}
		if (evt.keysym.scancode == SDL_SCANCODE_S)
		{
			m_fwdCommand->setIntensity(-1.f);
			return m_fwdCommand;
		}
		if (evt.keysym.scancode == SDL_SCANCODE_D)
		{
			m_stfCommand->setIntensity(1.f);
			return m_stfCommand;
		}
		if (evt.keysym.scancode == SDL_SCANCODE_A)
		{
			m_stfCommand->setIntensity(-1.f);
			return m_stfCommand;
		}
		if (evt.keysym.scancode == SDL_SCANCODE_E)
		{
			m_ascCommand->setIntensity(1.f);
			return m_ascCommand;
		}
		if (evt.keysym.scancode == SDL_SCANCODE_Q)
		{
			m_ascCommand->setIntensity(-1.f);
			return m_ascCommand;
		}
	}
	return nullptr;
}