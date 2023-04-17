#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <functional>

#include "Camera.hpp"

class Command {
	public:
		void setIntensity(float i) { m_intensity = i; }
		virtual std::function<void()> execute(Camera& cam) = 0;
	protected:
		float m_intensity = 0;
};

#include "glm/vec3.hpp"

class ForwardCommand : public Command {
	public:
		std::function<void()> execute(Camera& cam) {
			return[&cam, *this]() { return cam.move(glm::vec3(0.f, 0.f, -m_intensity)); };
		}
};

class StrafeCommand : public Command {
	public:
		std::function<void()> execute(Camera& cam) {
			return [&cam, *this]() {return cam.move(glm::vec3(m_intensity, 0.f, 0.f)); };
		}
};

class AscendCommand : public Command {
	public:
		std::function<void()> execute(Camera& cam) {
			return [&cam, *this]() {return cam.move(glm::vec3(0.f, m_intensity, 0.f)); };
		}
};

#endif