#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <scene_args.h>
#include <callbacks.h>
#include <shader_program.h>
#include <camera.h>
#include <wave_object.h>
#include <light_object.h>
#include <fps.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

GLFWwindow* scene_args::window = nullptr;
std::shared_ptr <Camera> scene_args::camera = nullptr;
float scene_args::delta_time = 0.0f;

int settings::window_width = 1366;
int settings::window_height = 768;
float settings::bg_r = 0.2f;
float settings::bg_g = 0.2f;
float settings::bg_b = 0.2f;
const int settings::plane_resolution = 3000;
const int settings::plane_size = 2000;

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
	
	scene_args::camera = std::make_shared <Camera> (glm::vec3(10.0f, 10.0f, 0 * (float)settings::plane_size * -0.5f));
	scene_args::camera->move_speed() = 10.0f;
	scene_args::camera->mouse_sens() = 0.06f;
	
	glfwSetInputMode(scene_args::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	Waves waves;
	waves.init("src/shaders/wave_vert.glsl", "src/shaders/wave_frag.glsl");
	
	std::vector <Light> lights(1);
	lights[0].init("src/shaders/lightsource_vert.glsl", "src/shaders/lightsource_frag.glsl");
	
	Fps fps;
	fps.enable_tracing(true);
	fps.set_trace_interval(10.0f);
	fps.set_trace_low_fps(true, 55.0f);
	
	while (!glfwWindowShouldClose(scene_args::window)) {
		float time_now = glfwGetTime();
		static float time_prv = 0.0f;	
		scene_args::delta_time = time_now - time_prv;
		time_prv = time_now;
		fps.frame(time_now);
		
		callback::update_movement();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		lights[0].draw(time_now);
		
		waves.draw(time_now, lights);
		
		glfwSwapBuffers(scene_args::window);
		
		glfwPollEvents();
	}
	
	glfwDestroyWindow(scene_args::window);
	glfwTerminate();
	
}
