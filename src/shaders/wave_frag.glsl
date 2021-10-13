#version 460 core

out vec4 color;

in vec3 light_0;
in float ambient_strength_0;

in vec3 pos;

void main() {
	float col = max(-0.3f, pos.y * 0.04f);
	// blue
	//color = vec4(0.4f + col, 0.4f + col, 0.8f + col, 1.0f);
	// medium blue
	color = vec4(0.3f + col, 0.3f + col, 0.6f + col, 1.0f);
	color.xyz *= light_0 * ambient_strength_0;
	// dark blue
	//color = vec4(0.2f + col, 0.2f + col, 0.4f + col, 1.0f);
}
