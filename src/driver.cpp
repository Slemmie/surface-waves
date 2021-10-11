#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <scene_args.h>
#include <callbacks.h>
#include <shader_program.h>
#include <camera.h>

#include <glm/glm.hpp>

#include <iostream>
#include <memory>

GLFWwindow* scene_args::window = nullptr;
std::shared_ptr <Shader_program> scene_args::shader_program = nullptr;
std::shared_ptr <Camera> scene_args::camera = nullptr;
float scene_args::delta_time = 0.0f;

int settings::window_width = 640;
int settings::window_height = 480;
float settings::bg_r = 0.2f;
float settings::bg_g = 0.2f;
float settings::bg_b = 0.2f;
float settings::plane_resolution = 10.0f;
const char* settings::vert_path = "src/shaders/vert.glsl";
const char* settings::frag_path = "src/shaders/frag.glsl";

int main() {
	
	glfwSetErrorCallback(callback::error);
	
	if (!glfwInit()) {
		std::cerr << "glfwInit() failed" << std::endl;
		return 1 << 20;
	}
	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	
	scene_args::window = glfwCreateWindow(settings::window_width, settings::window_height,
	"window", nullptr, nullptr);
	if (!scene_args::window) {
		std::cerr << "glfw window failed to be created" << std::endl;
		glfwTerminate();
		return 1 << 20;
	}
	
	glfwMakeContextCurrent(scene_args::window);
	
	glfwSetKeyCallback(scene_args::window, callback::keyboard_key);
	glfwSetFramebufferSizeCallback(scene_args::window, callback::window_resize);
	glfwSetCursorPosCallback(scene_args::window, callback::mouse_move);
	glfwSetScrollCallback(scene_args::window, callback::mouse_scroll);
	
	if (glewInit() != GLEW_OK) {
		std::cerr << "glewInit() failed" << std::endl;
		glfwDestroyWindow(scene_args::window);
		glfwTerminate();
		return 1 << 20;
	}
	glGetError();
	
	std::cerr << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cerr << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	
	glfwSwapInterval(1);
	
	glClearColor(settings::bg_r, settings::bg_g, settings::bg_b, 0.0f);
	glEnable(GL_DEPTH_TEST);
	
	scene_args::shader_program = std::make_shared <Shader_program> (settings::vert_path, settings::frag_path);
	scene_args::camera = std::make_shared <Camera> (glm::vec3(0.0f, 0.0f, 3.0f));
	
	while (!glfwWindowShouldClose(scene_args::window)) {
		float time_now = glfwGetTime();
		static float time_prv = 0.0f;
		scene_args::delta_time = time_now - time_prv;
		time_prv = time_now;
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		scene_args::shader_program->bind();
		
		glm::mat4 projection = glm::perspective(glm::radians(scene_args::camera->zoom()),
		(float)settings::window_width / (float)settings::window_height , 0.1f, 100.0f);
		scene_args::shader_program->set_uniform_mat4f("u_projection", projection);
		
		glm::mat4 view = scene_args::camera->view_matrix();
		scene_args::shader_program->set_uniform_mat4f("u_view", view);
		
		glfwSwapBuffers(scene_args::window);
		
		glfwPollEvents();
	}
	
	glfwDestroyWindow(scene_args::window);
	glfwTerminate();
	
}
