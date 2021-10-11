#pragma once

#include <GLEW/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>

class Shader_program {
	
public:
	
	Shader_program(const char* _vertex_path, const char* _fragment_path);
	~Shader_program();
	
	inline void bind() const {
		glUseProgram(m_program_id);
	}
	inline void unbind() const {
		glUseProgram(m_program_id);
	}
	
	void set_uniform_mat4f(const char* name, const glm::mat4& v0);
	
private:
	
	unsigned int m_program_id;
	
	std::unordered_map <std::string, int> m_uniform_location_map;
	
	int m_get_uniform_location(const char* name);
	unsigned int m_create_shader(const char* vertex_source, const char* fragment_source);
	unsigned int m_compile_shader(unsigned int shader_type, const char* shader_source);
	void m_read_source(const char* filepath, char*& buffer);
	
};
