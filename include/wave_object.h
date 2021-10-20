#pragma once

#include <shader_program.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <light_object.h>

#include <memory>
#include <vector>

class Waves {
	
public:
	
	Waves();
	
	~Waves();
	
	void init(const char* vert_path, const char* frag_path);
	inline void init_body(const char* vert_path, const char* frag_path) {
		m_body.init(vert_path, frag_path);
	}
	
	void draw(const float time_now, const Light& light);
	void draw_body(const float time_now, const Light& light) {
		m_body.draw(time_now, light);
	}
	
	inline const glm::vec3& ambient() const {
		return m_ambient;
	}
	inline glm::vec3& ambient() {
		return m_ambient;
	}
	
	inline const glm::vec3& diffuse() const {
		return m_diffuse;
	}
	inline glm::vec3& diffuse() {
		return m_diffuse;
	}
	
	inline const glm::vec3& specular() const {
		return m_specular;
	}
	inline glm::vec3& specular() {
		return m_specular;
	}
	
	inline float shininess() const {
		return m_shininess;
	}
	inline float shininess() {
		return m_shininess;
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
	float m_shininess;
	
	class Body {
		
	public:
		
		Body();
		
		~Body();
		
		void init(const char* vert_path, const char* frag_path);
		
		void draw(const float time_now, const Light& light);
		
	private:
		
		unsigned int m_vao, m_vbo, m_ebo;
		
		std::shared_ptr <Shader_program> m_shader_program;
		
		float m_body_depth;
		
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;
		
	} m_body;
	
};
