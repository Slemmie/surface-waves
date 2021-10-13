#pragma once

#include <shader_program.h>

#include <memory>

class Light {
	
public:
	
	Light();
	
	~Light();
	
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
