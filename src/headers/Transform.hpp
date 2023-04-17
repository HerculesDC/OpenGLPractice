#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Component.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

class Transform : public Component {
	public:
		Transform() = delete;
		Transform(const char*);

	public:
		void start() override;
		void update(Uint64) override;

	private:
		glm::qua<float> m_rotQuat;
		glm::vec3 m_position, m_rotation, m_scale;
};

#endif