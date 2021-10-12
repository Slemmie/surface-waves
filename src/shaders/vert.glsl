#version 460 core

layout (location = 0) in vec3 vpos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(vpos, 1.0);
	//gl_Position = vec4(vpos.x, vpos.y, vpos.z, 1.0);
}
