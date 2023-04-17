#ifndef INPUTPOCESSOR_HPP
#define INPUTPROCESSOR_HPP

class Command;
struct SDL_KeyboardEvent;

class InputProcessor {
	public:
		InputProcessor();
		InputProcessor(const InputProcessor&) = delete;
		InputProcessor(InputProcessor&&) = delete;
		InputProcessor& operator=(InputProcessor&) = delete;
		compl InputProcessor();
	public:
		Command* handleInput(SDL_KeyboardEvent);
	private:
		float m_intensity = 1.f;
		Command* m_fwdCommand; //forward-back
		Command* m_stfCommand; //strafe left-right
		Command* m_ascCommand; //ascend-descend
		//Command* m_yawCommand; //turns left-right aroundy
		//Command* m_pitCommand; //look up-down
		//Command* m_rolCommand; //roll cw-ccw
};

#endif