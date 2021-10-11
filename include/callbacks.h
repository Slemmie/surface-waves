#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace callback {
	
	extern void error(int id, const char* msg);
	
	extern void keyboard_key(GLFWwindow* window, int key, int scan, int action, int mods);
	
	extern void window_resize(GLFWwindow* window, int width, int height);
	
	extern void mouse_move(GLFWwindow* window, double x_pos, double y_pos);
	
	extern void mouse_scroll(GLFWwindow* window, double x_offset, double y_offset);
	
} /// namespace callback
