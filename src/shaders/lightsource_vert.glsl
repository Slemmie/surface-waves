#version 460 core

layout (location = 1) in vec3 vpos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_time;

uniform vec3 u_light_color;

out vec3 light_color;

out float ypos;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(vpos, 1.0f);
	ypos = vpos.y;
	light_color = u_light_color;
}
