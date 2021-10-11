#include <callbacks.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <scene_args.h>

#include <iostream>

namespace callback {
	
	void error(int id, const char* msg) {
		std::cerr << "[error]: " << msg << " (ID: " << id << ")" << std::endl;
	}
	
	void keyboard_key(GLFWwindow* window, int key, int scan, int action, int mods) {
		static bool w_pressed = false;
		static bool a_pressed = false;
		static bool s_pressed = false;
		static bool d_pressed = false;
		switch (key) {
		case GLFW_KEY_W:
			w_pressed = action == GLFW_PRESS;
			break;
		case GLFW_KEY_A:
			a_pressed = action == GLFW_PRESS;
			break;
		case GLFW_KEY_S:
			s_pressed = action == GLFW_PRESS;
			break;
		case GLFW_KEY_D:
			d_pressed = action == GLFW_PRESS;
			break;
		};
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
	}
	
	void window_resize(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}
	
	void mouse_move(GLFWwindow* window, double x_pos, double y_pos) {
		static bool first_move = true;
		static double x_prv, y_prv;
		if (first_move) {
			first_move = false;
			x_prv = x_pos;
			y_prv = y_pos;
		}
		float x_offset = x_pos - x_prv;
		float y_offset = y_pos - y_prv;
		scene_args::camera->move_looking(x_offset, y_offset);
	}
	
	void mouse_scroll(GLFWwindow* window, double x_offset, double y_offset) {
		scene_args::camera->move_zoom(y_offset);
	}
	
} /// namespace callback
