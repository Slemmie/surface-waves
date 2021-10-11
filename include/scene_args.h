#pragma once

#include <GLFW/glfw3.h>

#include <shader_program.h>
#include <camera.h>

#include <memory>

namespace scene_args {
	
	extern GLFWwindow* window;
	
	extern std::shared_ptr <Shader_program> shader_program;
	
	extern std::shared_ptr <Camera> camera;
	
	extern float delta_time;
	
} /// namespace scene_args
