#include <wave_object.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <settings.h>
#include <scene_args.h>

#include <string>
#include <vector>
#include <algorithm>

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
			float which_i = (float)i / ((float)settings::plane_resolution - 1) * (float)settings::plane_size;
			float which_j = (float)j / ((float)settings::plane_resolution - 1) * (float)settings::plane_size;
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
	(float)settings::window_width / (float)settings::window_height , 0.1f, settings::render_distance);
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

Waves::Body::Body() :
m_vao(0),
m_vbo(0),
m_ebo(0),
m_body_depth(settings::water_depth),
m_ambient(1.87f, 1.87f, 2.98f),
m_diffuse(0.55f, 0.6f, 0.95f),
m_specular(0.0f, 0.0f, 0.0f),
m_shininess(32.0f)
{ }

Waves::Body::~Body() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Waves::Body::init(const char* vert_path, const char* frag_path) {
	m_shader_program = std::make_shared <Shader_program> (vert_path, frag_path);
	
	// x, y, z, up/down
	float vertices[(settings::plane_resolution - 1) * 4 * 2 * (3 + 1) + 1 * (3 + 1)];
	int ptr = 0;
	float shift = -1.0f * (float)settings::plane_size / 2.0f;
	for (int i = 0; i < settings::plane_resolution - 1; i++) {
		float which_i = 0;
		float which_j = (float)i / ((float)settings::plane_resolution - 1) * (float)settings::plane_size;
		which_i += shift;
		which_j += shift;
		
		vertices[ptr++] = which_i;
		vertices[ptr++] = 0;
		vertices[ptr++] = which_j;
		vertices[ptr++] = 0.0f;
		
		vertices[ptr++] = which_i;
		vertices[ptr++] = m_body_depth;
		vertices[ptr++] = which_j;
		vertices[ptr++] = 1.0f;
	}
	for (int i = 0; i < settings::plane_resolution - 1; i++) {
		float which_i = (float)i / ((float)settings::plane_resolution - 1) * (float)settings::plane_size;
		float which_j = settings::plane_size;
		which_i += shift;
		which_j += shift;
		
		vertices[ptr++] = which_i;
		vertices[ptr++] = 0;
		vertices[ptr++] = which_j;
		vertices[ptr++] = 0.0f;
		
		vertices[ptr++] = which_i;
		vertices[ptr++] = m_body_depth;
		vertices[ptr++] = which_j;
		vertices[ptr++] = 1.0f;
	}
	for (int i = settings::plane_resolution - 1; i >= 1; i--) {
		float which_i = settings::plane_size;
		float which_j = (float)i / ((float)settings::plane_resolution - 1) * (float)settings::plane_size;
		which_i += shift;
		which_j += shift;
		
		vertices[ptr++] = which_i;
		vertices[ptr++] = 0;
		vertices[ptr++] = which_j;
		vertices[ptr++] = 0.0f;
		
		vertices[ptr++] = which_i;
		vertices[ptr++] = m_body_depth;
		vertices[ptr++] = which_j;
		vertices[ptr++] = 1.0f;
	}
	for (int i = settings::plane_resolution - 1; i >= 1; i--) {
		float which_i = (float)i / ((float)settings::plane_resolution - 1) * (float)settings::plane_size;
		float which_j = 0;
		which_i += shift;
		which_j += shift;
		
		vertices[ptr++] = which_i;
		vertices[ptr++] = 0;
		vertices[ptr++] = which_j;
		vertices[ptr++] = 0.0f;
		
		vertices[ptr++] = which_i;
		vertices[ptr++] = m_body_depth;
		vertices[ptr++] = which_j;
		vertices[ptr++] = 1.0f;
	}
	vertices[ptr++] = 0;
	vertices[ptr++] = m_body_depth;
	vertices[ptr++] = 0;
	vertices[ptr++] = 2;
	
	unsigned int indices[((settings::plane_resolution - 1) * 4) * 3 * (2 + 1)];
	ptr = 0;
	for (int i = 0; i < ((settings::plane_resolution - 1) * 4) - 1; i++) {
		int ind_ptr = i * 2;
		
		indices[ptr++] = ind_ptr;
		indices[ptr++] = ind_ptr + 2;
		indices[ptr++] = ind_ptr + 1;
		
		indices[ptr++] = ind_ptr + 2;
		indices[ptr++] = ind_ptr + 3;
		indices[ptr++] = ind_ptr + 1;
	}
	{
		int ind_ptr = ((settings::plane_resolution - 1) * 4) - 1;
		ind_ptr *= 2;
		
		indices[ptr++] = ind_ptr;
		indices[ptr++] = 0;
		indices[ptr++] = ind_ptr + 1;
		
		indices[ptr++] = 0;
		indices[ptr++] = 1;
		indices[ptr++] = ind_ptr + 1;
	}
	for (int i = 0; i < ((settings::plane_resolution - 1) * 4) - 1; i++) {
		int ind_ptr = i * 2;
		indices[ptr++] = ind_ptr + 1;
		indices[ptr++] = ((settings::plane_resolution - 1) * 4 * 2 + 1) - 1;
		indices[ptr++] = ind_ptr + 3;
	}
	{
		int ind_ptr = ((settings::plane_resolution - 1) * 4) - 1;
		ind_ptr *= 2;
		indices[ptr++] = ind_ptr + 1;
		indices[ptr++] = ((settings::plane_resolution - 1) * 4 * 2 + 1) - 1;
		indices[ptr++] = 1;
	}
	
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Waves::Body::draw(const float time_now, const Light& light) {
	m_shader_program->bind();
	glBindVertexArray(m_vao);
	
	glm::mat4 projection = glm::perspective(glm::radians(scene_args::camera->zoom()),
	(float)settings::window_width / (float)settings::window_height , 0.1f, settings::render_distance);
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
	
	m_shader_program->set_uniform_vec3f("u_material.ambient", m_ambient);
	m_shader_program->set_uniform_vec3f("u_material.diffuse", m_diffuse);
	m_shader_program->set_uniform_vec3f("u_material.specular", m_specular);
	m_shader_program->set_uniform_1f("u_material.shininess", m_shininess);
	
	m_shader_program->set_uniform_1f("u_body_depth", m_body_depth);
	
	int triangles = (settings::plane_resolution - 1) * 4 * 3 * 2;
	int side = triangles / 4;
	
	// low_x, high_z, high_x, low_z
	float shift = -1.0f * (float)settings::plane_size / 2.0f;
	float offset = settings::plane_size + shift;
	std::vector <std::pair <float, int>> order = {
		{ glm::distance(scene_args::camera->position(), glm::vec3(-offset, 0.0f, 0.0f)), 0 },
		{ glm::distance(scene_args::camera->position(), glm::vec3(0.0f, 0.0f, +offset)), 1 },
		{ glm::distance(scene_args::camera->position(), glm::vec3(+offset, 0.0f, 0.0f)), 2 },
		{ glm::distance(scene_args::camera->position(), glm::vec3(0.0f, 0.0f, -offset)), 3 }
	};
	std::sort(order.rbegin(), order.rend());
	
	for (int i = 0; i < 4; i++) {
		glDrawElements(GL_TRIANGLES, side, GL_UNSIGNED_INT, (void*)(order[i].second * side * sizeof(unsigned int)));
	}
	
	void* bottom_offset = (void*)(((settings::plane_resolution - 1) * 4) * 3 * 2 * sizeof(unsigned int));
	glDrawElements(GL_TRIANGLES, ((settings::plane_resolution - 1) * 4) * 3, GL_UNSIGNED_INT, bottom_offset);
	
	glBindVertexArray(0);
	m_shader_program->unbind();
}
