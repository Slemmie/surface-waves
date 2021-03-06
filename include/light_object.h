#pragma once

#include <shader_program.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

class Light {
	
public:
	
	Light();
	
	~Light();
	
	void init(const char* vert_path, const char* frag_path);
	
	void draw(const float time_now);
	
	inline const glm::vec3& ambient() const {
		return m_ambient;
	}
	
	inline const glm::vec3& diffuse() const {
		return m_diffuse;
	}
	
	inline const glm::vec3& specular() const {
		return m_specular;
	}
	
	inline const glm::vec3& light_pos() const {
		return m_position;
	}
	
	inline unsigned int vao() const {
		return m_vao;
	}
	
	inline unsigned int vbo() const {
		return m_vbo;
	}
	
	inline unsigned int ebo() const {
		return m_ebo;
	}
	
private:
	
	unsigned int m_vao, m_vbo, m_ebo;
	
	std::shared_ptr <Shader_program> m_shader_program;
	
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	
	glm::vec3 m_position, m_scale;
	
};
