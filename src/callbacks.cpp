#include <callbacks.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <scene_args.h>
#include <settings.h>

#include <iostream>

namespace callback {
	
	void error(int id, const char* msg) {
		std::cerr << "[error]: " << msg << " (ID: " << id << ")" << std::endl;
	}
	
	static bool w_pressed = false;
	static bool a_pressed = false;
	static bool s_pressed = false;
	static bool d_pressed = false;
	static bool space_pressed = false;
	static bool shift_pressed = false;
	
	static bool mouse_locked = true;
	
	void update_movement() {
		if (w_pressed) {
			scene_args::camera->move_position(CAM_FORWARD, scene_args::delta_time);
		}
		if (a_pressed) {
			scene_args::camera->move_position(CAM_LEFT, scene_args::delta_time);
		}
		if (s_pressed) {
			scene_args::camera->move_position(CAM_BACKWARD, scene_args::delta_time);
		}
		if (d_pressed) {
			scene_args::camera->move_position(CAM_RIGHT, scene_args::delta_time);
		}
		if (space_pressed) {
			scene_args::camera->move_position(CAM_UP, scene_args::delta_time);
		}
		if (shift_pressed) {
			scene_args::camera->move_position(CAM_DOWN, scene_args::delta_time);
		}
	}
	
	void toggle_mouse_lock() {
		mouse_locked ^= 1;
		if (mouse_locked) {
			glfwSetInputMode(scene_args::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(scene_args::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	
	void keyboard_key(GLFWwindow* window, int key, int scan, int action, int mods) {
		bool set = true;
		if (action == GLFW_RELEASE) {
			set = false;
		}
		switch (key) {
		case GLFW_KEY_W:
			w_pressed = set;
			break;
		case GLFW_KEY_A:
			a_pressed = set;
			break;
		case GLFW_KEY_S:
			s_pressed = set;
			break;
		case GLFW_KEY_D:
			d_pressed = set;
			break;
		case GLFW_KEY_SPACE:
			space_pressed = set;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			shift_pressed = set;
			break;
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS) {
				toggle_mouse_lock();
			}
			break;
		case GLFW_KEY_LEFT_CONTROL:
			if (action == GLFW_PRESS) {
				scene_args::camera->move_speed() = 100.0f;
			} else if (action == GLFW_RELEASE) {
				scene_args::camera->move_speed() = 10.0f;
			}
			break;
		default:
			break;
		};
	}
	
	void window_resize(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		settings::window_width = width;
		settings::window_height = height;
	}
	
	void mouse_move(GLFWwindow* window, double x_pos, double y_pos) {
		static bool first_move = true;
		if (!mouse_locked) {
			first_move = true;
			return;
		}
		static double x_prv, y_prv;
		if (first_move) {
			first_move = false;
			x_prv = x_pos;
			y_prv = y_pos;
		}
		float x_offset = x_pos - x_prv;
		float y_offset = y_pos - y_prv;
		x_prv = x_pos;
		y_prv = y_pos;
		scene_args::camera->move_looking(x_offset, y_offset);
	}
	
	void mouse_scroll(GLFWwindow* window, double x_offset, double y_offset) {
		scene_args::camera->move_zoom(y_offset);
	}
	
} /// namespace callback
