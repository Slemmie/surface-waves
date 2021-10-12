#version 460 core

layout (location = 0) in vec3 vpos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_time;

out vec3 pos;

void main() {
	float height = sin(0.4 * vpos.x + 0.4 * vpos.z + u_time) * 0.3;
	gl_Position = u_projection * u_view * u_model *
	vec4(vpos.x, vpos.y + height, vpos.z, 1.0);
	pos = vec3(vpos.x, vpos.y + height, vpos.z);
}
