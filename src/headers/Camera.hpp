#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera {
	public:
		Camera();
	public:
		void updateCamera();
		void lookAt(glm::vec3);
		void move(glm::vec3);
		void pan(float); //rotate around camera up
		void tilt(float); //rotate around camera right
		void roll(float); //rotate around camera forward
		void zoom(float); //should change FoV
		glm::vec3 getPosition() const;
		glm::vec3 getTarget() const;
		Camera getCamera();
		glm::mat4 getCameraView() const;
		glm::mat4 getCameraProjection() const;
		const float* getCameraViewData() const;
		const float* getCameraProjectionData() const;

	private:
		void updateCameraView();
		void updateCameraProjection();
	private:
		bool m_invertX, m_invertY;
		int m_width, m_height;
		float m_FoV, m_near, m_far, m_roll, m_pitch, m_yaw;
		glm::vec3 m_position, m_forward, m_up, m_right; //m_up is camera up, not world up
		glm::mat4 m_cameraView;
		glm::mat4 m_projection;
};

#endif