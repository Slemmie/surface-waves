#version 460 core

layout (location = 0) in vec3 vert_pos;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
	gl_Position = u_view * u_projection * vec4(vert_pos, 1.0);
}
