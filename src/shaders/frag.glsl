#version 460 core

out vec4 color;

in vec3 pos;

void main() {
	float col = max(-0.3f, pos.y * 0.04f);
	// blue
	//color = vec4(0.4f + col, 0.4f + col, 0.8f + col, 1.0f);
	// medium blue
	color = vec4(0.3f + col, 0.3f + col, 0.6f + col, 1.0f);
	// dark blue
	//color = vec4(0.2f + col, 0.2f + col, 0.4f + col, 1.0f);
}
