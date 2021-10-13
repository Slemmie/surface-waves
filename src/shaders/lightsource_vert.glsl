#version 460 core

layout (location = 1) in vec3 vpos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_time;

out float ypos;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(vpos, 1.0f);
	ypos = vpos.y;
}
