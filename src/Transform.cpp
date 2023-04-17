#include "headers/Transform.hpp"

Transform::Transform(const char* name) : Component(name), m_position(glm::vec3(0.f)),
										 m_rotation(glm::vec3(0.f)), m_scale(glm::vec3(1.f))
{
	m_rotQuat = glm::qua<float>(m_rotation);
}

void Transform::start(){}

void Transform::update(Uint64 delta){}