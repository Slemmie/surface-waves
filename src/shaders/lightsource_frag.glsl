#version 460 core

out vec4 color;

in float ypos;

void main() {
	color = vec4(0.95f - ypos * 0.4f, 0.95f - ypos * 0.4f, 0.95f - ypos * 0.4f, 1.0f);
}
