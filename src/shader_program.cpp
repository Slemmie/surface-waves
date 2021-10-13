#include <shader_program.h>

#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <iostream>

Shader_program::Shader_program(const char* _vertex_path, const char* _fragment_path) :
m_program_id(0)
{
	char* vertex_buffer = nullptr;
	m_read_source(_vertex_path, vertex_buffer);
	char* fragment_buffer = nullptr;
	m_read_source(_fragment_path, fragment_buffer);
	m_program_id = m_create_shader(vertex_buffer, fragment_buffer);
	free(vertex_buffer);
	free(fragment_buffer);
}

Shader_program::~Shader_program() {
	glDeleteProgram(m_program_id);
}

void Shader_program::set_uniform_mat4f(const char* name, const glm::mat4& v0) {
	glUniformMatrix4fv(m_get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(v0));
}

void Shader_program::set_uniform_1f(const char* name, const float v0) {
	glUniform1f(m_get_uniform_location(name), v0);
}

void Shader_program::set_uniform_vec3f(const char* name, const glm::vec3& v0) {
	glUniform3fv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

int Shader_program::m_get_uniform_location(const char* name) {
	auto it = m_uniform_location_map.find(name);
	if (it != m_uniform_location_map.end()) {
		return it->second;
	}
	return m_uniform_location_map[name] = glGetUniformLocation(m_program_id, name);
}

unsigned int Shader_program::m_create_shader(const char* vertex_source, const char* fragment_source) {
	unsigned int program = glCreateProgram();
	unsigned int vert_sh = m_compile_shader(GL_VERTEX_SHADER, vertex_source);
	unsigned int frag_sh = m_compile_shader(GL_FRAGMENT_SHADER, fragment_source);
	glAttachShader(program, vert_sh);
	glAttachShader(program, frag_sh);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vert_sh);
	glDeleteShader(frag_sh);
	return program;
}

unsigned int Shader_program::m_compile_shader(unsigned int shader_type, const char* shader_source) {
	assert(shader_type == GL_VERTEX_SHADER || shader_type == GL_FRAGMENT_SHADER);
	unsigned int id = glCreateShader(shader_type);
	glShaderSource(id, 1, &shader_source, nullptr);
	glCompileShader(id);
	int compilation_result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compilation_result);
	if (compilation_result == GL_FALSE) {
		int err_len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &err_len);
		char* err_msg = (char*)alloca(sizeof(char) * err_len);
		glGetShaderInfoLog(id, err_len, &err_len, err_msg);
		std::cerr << "failed to compile " <<
		(shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
		std::cerr << err_msg << std::endl;
		glDeleteShader(id);
	}
	return id;
}

void Shader_program::m_read_source(const char* filepath, char*& buffer) {
	FILE* file = fopen(filepath, "rt");
	if (!file) {
		std::cerr << "failed to open file '" << filepath << "'" << std::endl;
		buffer = nullptr;
		return;
	}
	fseek(file, 0, SEEK_END);
	auto size = ftell(file);
	rewind(file);
	buffer = (char*)malloc(sizeof(char) * (size + 1));
	auto end = fread(buffer, sizeof(char), size, file);
	buffer[end] = '\0';
	fclose(file);
}
