#include "headers/Camera.hpp"
#include "glm/gtc/matrix_transform.hpp" //for lookAt

//CONSTRUCTOR
Camera::Camera() : m_width(800), m_height(800), m_FoV(1.f), m_near(.1f), m_far(100.f), m_roll(0.f),
				   m_pitch(0.f), m_yaw(0.f), m_position(glm::vec3(0.f, .0f, -3.f)),
				   m_forward(glm::vec3(0.f, 0.f, -1.f)), m_up(glm::vec3(0.f)), m_right(glm::vec3(0.f))
{
	m_cameraView = glm::mat4(1.f);
	m_projection = glm::mat4(1.f);
	updateCamera();
}

//PRIVATE METHODS
void Camera::updateCameraView() { 
	m_cameraView = glm::lookAt(m_position, /*m_position +*/ m_forward, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::updateCameraProjection() {
	m_projection = glm::perspective(m_FoV, (float)m_width / (float)m_height, m_near, m_far);
}

//PUBLIC METHODS
void Camera::updateCamera() {
	updateCameraView();
	updateCameraProjection();
}

void Camera::lookAt(glm::vec3 target){
	m_forward = target;
	updateCameraView();
}

void Camera::move(glm::vec3 moveBy) {
	m_position = m_position + moveBy;
	updateCameraView();
}

void Camera::pan(float angle) { /*IMPLEMENT*/ }

void Camera::tilt(float angle) { /*IMPLEMENT*/ }

void Camera::roll(float angle) { /*IMPLEMENT*/ }

void Camera::zoom(float newFoV) {
	m_FoV = newFoV;
	updateCameraProjection();
}

glm::vec3 Camera::getPosition() const { return m_position; }

glm::vec3 Camera::getTarget() const { return m_forward; }

Camera Camera::getCamera() { return *this; }

glm::mat4 Camera::getCameraView() const { return m_cameraView; }

glm::mat4 Camera::getCameraProjection() const { return m_projection; }

const float* Camera::getCameraViewData() const { return glm::value_ptr(m_cameraView); }

const float* Camera::getCameraProjectionData() const { return glm::value_ptr(m_projection); }