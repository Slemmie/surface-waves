#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <scene_args.h>
#include <callbacks.h>
#include <shader_program.h>
#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

GLFWwindow* scene_args::window = nullptr;
std::shared_ptr <Shader_program> scene_args::shader_program = nullptr;
std::shared_ptr <Camera> scene_args::camera = nullptr;
float scene_args::delta_time = 0.0f;

int settings::window_width = 1366;
int settings::window_height = 768;
float settings::bg_r = 0.2f;
float settings::bg_g = 0.2f;
float settings::bg_b = 0.2f;
const int settings::plane_resolution = 2500;
const int settings::plane_size = 4000;
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
	
	scene_args::camera = std::make_shared <Camera> (glm::vec3(10.0f, 10.0f, 0 * (float)settings::plane_size * -0.5f));
	scene_args::camera->move_speed() = 10.0f;
	scene_args::camera->mouse_sens() = 0.06f;
	
	glfwSetInputMode(scene_args::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	float vertices[settings::plane_resolution * settings::plane_resolution * 3];
	for (int i = 0, ptr = 0; i < settings::plane_resolution; i++) {
		for (int j = 0; j < settings::plane_resolution; j++) {
			float shift = -1.0f * (float)settings::plane_size / 2.0f;
			float which_i = (float)i / (float)settings::plane_resolution * (float)settings::plane_size;
			float which_j = (float)j / (float)settings::plane_resolution * (float)settings::plane_size;
			which_i += shift;
			which_j += shift;
			vertices[ptr++] = which_i;
			vertices[ptr++] = 0;
			vertices[ptr++] = which_j;
		}
	}
	unsigned int indices[(settings::plane_resolution - 1) * (settings::plane_resolution - 1) * 3 * 2];
	for (int i = 0, ptr = 0; i + 1 < settings::plane_resolution; i++) {
		for (int j = 0; j + 1 < settings::plane_resolution; j++) {
			indices[ptr++] = i * settings::plane_resolution + j;
			indices[ptr++] = i * settings::plane_resolution + j + 1;
			indices[ptr++] = (i + 1) * settings::plane_resolution + j;
			
			indices[ptr++] = i * settings::plane_resolution + j + 1;
			indices[ptr++] = (i + 1) * settings::plane_resolution + j + 1;
			indices[ptr++] = (i + 1) * settings::plane_resolution + j;
		}
	}
	
	unsigned int vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	while (!glfwWindowShouldClose(scene_args::window)) {
		float time_now = glfwGetTime();
		static float time_prv = 0.0f;
		scene_args::delta_time = time_now - time_prv;
		time_prv = time_now;
		static float fps_prv_time = 0.0f;
		static unsigned int fps_frame_cnt = 0;
		fps_frame_cnt++;
		if (time_now - fps_prv_time >= 1.0f) {
			std::cerr << "fps: " << fps_frame_cnt << std::endl;
			fps_frame_cnt = 0;
			fps_prv_time = time_now;
		}
		
		callback::update_movement();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		scene_args::shader_program->bind();
		glBindVertexArray(vao);
		
		glm::mat4 projection = glm::perspective(glm::radians(scene_args::camera->zoom()),
		(float)settings::window_width / (float)settings::window_height , 0.1f, 1000.0f);
		scene_args::shader_program->set_uniform_mat4f("u_projection", projection);
		
		glm::mat4 view = scene_args::camera->view_matrix();
		scene_args::shader_program->set_uniform_mat4f("u_view", view);
		
		glm::mat4 model = glm::mat4(1.0f);
		scene_args::shader_program->set_uniform_mat4f("u_model", model);
		
		scene_args::shader_program->set_uniform_1f("u_time", time_now);
		
		int triangles = (settings::plane_resolution - 1) * (settings::plane_resolution - 1);
		glDrawElements(GL_TRIANGLES, triangles * 3 * 2, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		glfwSwapBuffers(scene_args::window);
		
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	
	glfwDestroyWindow(scene_args::window);
	glfwTerminate();
	
}
