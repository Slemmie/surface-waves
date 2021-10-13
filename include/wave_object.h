#pragma once

#include <shader_program.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

class Waves {
	
public:
	
	Waves();
	
	~Waves();
	
	void init(const char* vert_path, const char* frag_path);
	
	void draw(const float time_now);
	
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
	
};
