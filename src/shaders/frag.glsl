#version 460 core

out vec4 color;

in vec3 pos;

void main() {
	color = vec4(0.5f + pos.y * 0.04f, 0.5f + pos.y * 0.04f, 0.8f + pos.y * 0.04f, 1.0f);
}
