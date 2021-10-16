#include <wave_object.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <scene_args.h>

#include <string>

Waves::Waves() :
m_vao(0),
m_vbo(0),
m_ebo(0),
m_shader_program(nullptr),
m_ambient(0.5f, 0.5f, 0.7f),
m_diffuse(0.45f, 0.4f, 0.9f),
m_specular(0.6f, 0.6f, 0.7f),
m_shininess(64.0f)
{
	
}

Waves::~Waves() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Waves::init(const char* vert_path, const char* frag_path) {
	m_shader_program = std::make_shared <Shader_program> (vert_path, frag_path);
	
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
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Waves::draw(const float time_now, const Light& light) {
	m_shader_program->bind();
	glBindVertexArray(m_vao);
	
	glm::mat4 projection = glm::perspective(glm::radians(scene_args::camera->zoom()),
	(float)settings::window_width / (float)settings::window_height , 0.1f, 1000.0f);
	m_shader_program->set_uniform_mat4f("u_projection", projection);
	
	glm::mat4 view = scene_args::camera->view_matrix();
	m_shader_program->set_uniform_mat4f("u_view", view);
	
	glm::mat4 model = glm::mat4(1.0f);
	m_shader_program->set_uniform_mat4f("u_model", model);
	
	m_shader_program->set_uniform_1f("u_time", time_now);
	
	m_shader_program->set_uniform_vec3f("u_light.position", light.light_pos());
	m_shader_program->set_uniform_vec3f("u_light.ambient", light.ambient());
	m_shader_program->set_uniform_vec3f("u_light.diffuse", light.diffuse());
	m_shader_program->set_uniform_vec3f("u_light.specular", light.specular());
	
	m_shader_program->set_uniform_vec3f("u_view_pos", scene_args::camera->position());
	
	m_shader_program->set_uniform_vec3f("u_material.ambient", ambient());
	m_shader_program->set_uniform_vec3f("u_material.diffuse", diffuse());
	m_shader_program->set_uniform_vec3f("u_material.specular", specular());
	m_shader_program->set_uniform_1f("u_material.shininess", shininess());
	
	int triangles = (settings::plane_resolution - 1) * (settings::plane_resolution - 1);
	glDrawElements(GL_TRIANGLES, triangles * 3 * 2, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	m_shader_program->unbind();
}
