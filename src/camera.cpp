#include <camera.h>

Camera::Camera(glm::vec3 _position, glm::vec3 _world_up, float _yaw, float _pitch) :
m_position(_position),
m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
m_world_up(_world_up),
m_yaw(_yaw),
m_pitch(_pitch),
m_mvspeed(2.5f),
m_mouse_sens(0.1f),
m_zoom(45.0f)
{
	m_update_camera_vectors();
}

void Camera::move_position(int cam_dir, float dt) {
	float vel = m_mvspeed * dt;
	switch (cam_dir) {
	case CAM_FORWARD:
		m_position += m_front * vel;
		break;
	case CAM_BACKWARD:
		m_position -= m_front * vel;
		break;
	case CAM_LEFT:
		m_position -= m_right * vel;
		break;
	case CAM_RIGHT:
		m_position += m_right * vel;
		break;
	default:
		break;
	};
}

void Camera::move_looking(float offset_x, float offset_y, bool constrain_pitch) {
	offset_x *= m_mouse_sens;
	offset_y *= m_mouse_sens;
	m_yaw += offset_x;
	m_pitch += offset_y;
	if (constrain_pitch) {
		m_pitch = std::min(m_pitch, 89.0f);
		m_pitch = std::max(m_pitch, -89.0f);
	}
	m_update_camera_vectors();
}

void Camera::move_zoom(float offset) {
	m_zoom -= offset;
	m_zoom = std::max(m_zoom, 1.0f);
	m_zoom = std::min(m_zoom, 45.0f);
}

void Camera::m_update_camera_vectors() {
	glm::vec3 front = glm::vec3(
	std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
	std::sin(glm::radians(m_pitch)),
	std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)));
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(front, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, front));
}
