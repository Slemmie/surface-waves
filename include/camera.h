#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define CAM_FORWARD     (1 << 0)
#define CAM_BACKWARD    (1 << 1)
#define CAM_LEFT        (1 << 2)
#define CAM_RIGHT       (1 << 3)
#define CAM_UP          (1 << 4)
#define CAM_DOWN        (1 << 5)

class Camera {
	
public:
	
	Camera(
	glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3 _world_up = glm::vec3(0.0f, 1.0f, 0.0f),
	float _yaw = 90.0f, float _pitch = 0.0f);
	
	inline glm::mat4 view_matrix() const {
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}
	inline float zoom() const {
		return m_zoom;
	}
	inline float move_speed() const {
		return m_mvspeed;
	}
	inline float& move_speed() {
		return m_mvspeed;
	}
	inline float mouse_sens() const {
		return m_mouse_sens;
	}
	inline float& mouse_sens() {
		return m_mouse_sens;
	}
	
	void move_position(int cam_dir, float dt);
	
	void move_looking(float offset_x, float offset_y, bool constrain_pitch = true);
	
	void move_zoom(float offset);
	
private:
	
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_world_up;
	
	float m_yaw, m_pitch;
	float m_mvspeed, m_mouse_sens;
	float m_zoom;
	
	void m_update_camera_vectors();
	
};
