#version 460 core

out vec4 color;

in vec3 light_color;

in float ypos;

void main() {
	color = vec4(
	light_color.x - ypos * 0.4f,
	light_color.y - ypos * 0.4f,
	light_color.z - ypos * 0.4f,
	1.0f);
}
