#include <light_object.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <scene_args.h>

Light::Light() :
m_vao(0),
m_vbo(0),
m_ebo(0),
m_shader_program(nullptr),
m_light_color(0.95f, 0.95f, 0.95f)
{
	
}

Light::~Light() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Light::init(const char* vert_path, const char* frag_path) {
	m_shader_program = std::make_shared <Shader_program> (vert_path, frag_path);
	
	float vertices[8 * 3] = {
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f
	};
	
	unsigned int indices[3 * 2 * 6] {
		0, 1, 3,
		1, 2, 3,
		4, 5, 7,
		5, 6, 7,
		0, 1, 4,
		1, 5, 4,
		1, 5, 2,
		5, 6, 2,
		2, 6, 3,
		6, 7, 3,
		3, 7, 0,
		7, 4, 0
	};
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Light::draw(const float time_now) {
	m_shader_program->bind();
	glBindVertexArray(m_vao);
	
	glm::mat4 projection = glm::perspective(glm::radians(scene_args::camera->zoom()),
	(float)settings::window_width / (float)settings::window_height , 0.1f, 1000.0f);
	m_shader_program->set_uniform_mat4f("u_projection", projection);
	
	glm::mat4 view = scene_args::camera->view_matrix();
	m_shader_program->set_uniform_mat4f("u_view", view);
	
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 scale = glm::vec3(5.0f, 5.0f, 5.0f);
	model = glm::scale(model, scale);
	glm::vec3 position = glm::vec3(10.0f, 10.0f, 10.0f);
	model = glm::translate(model, position);
	m_shader_program->set_uniform_mat4f("u_model", model);
	
	m_shader_program->set_uniform_1f("u_time", time_now);
	
	m_shader_program->set_uniform_vec3f("u_light_color", m_light_color);
	
	glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	m_shader_program->unbind();
}
