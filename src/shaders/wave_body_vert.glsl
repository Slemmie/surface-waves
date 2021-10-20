#version 460 core

layout (location = 2) in vec4 vert_info;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_time;

uniform vec3 u_view_pos;

uniform float u_body_depth;

out vec3 normal;

out vec3 frag_pos;
out vec3 default_pos;

#define PI_f 3.1415926538f

#define WAVE_COUNT 5
// direction.x, direction.y, steepness, wavelength
vec4 waves[WAVE_COUNT] = {
	vec4(1.0f, 1.0f, 0.2f, 60.0f),
	vec4(1.0f, 0.6f, 0.2f, 31.0f),
	vec4(1.0f, 1.3f, 0.2f, 18.0f),
	vec4(0.5f, 0.6f, 0.40f, 100.0f),
	vec4(0.9f, 0.5f, 0.07f, 150.0f)
};
vec3 wave_scale[WAVE_COUNT] = {
	vec3(1.0f, 1.0f, 1.0f),
	vec3(1.0f, 1.0f, 1.0f),
	vec3(1.0f, 1.0f, 1.0f),
	vec3(0.1f, 0.2f, 0.1f),
	vec3(1.0f, 1.0f, 1.0f)
};

vec3 gerstner_wave(vec4 wave, vec3 pos) {
	float steepness = wave.z;
	float wavelength = wave.w;
	float k = 2.0f * PI_f / wavelength;
	float c = sqrt(9.8f / k);
	vec2 d = normalize(wave.xy);
	float b = k * (dot(d, pos.xz) - c * 1.5f * u_time);
	float a = steepness / k;
	return vec3(d.x * (a * cos(b)), a * sin(b), d.y * (a * cos(b)));
}

void main() {
	vec3 vert_pos = vec3(vert_info.x, vert_info.y, vert_info.z);
	bool is_down = vert_info.w >= 0.1f;
	
	vec3 result_pos = vert_pos;
	result_pos.y = 0.0f;
	for (int i = 0; i < WAVE_COUNT; i++) {
		result_pos += gerstner_wave(waves[i], vert_pos) * wave_scale[i];
	}
	result_pos.y += vert_pos.x * vert_pos.x * -0.00005f + vert_pos.z * vert_pos.z * -0.00005f;
	result_pos.y = max(result_pos.y, -110.0f);
	
	if (is_down) {
		result_pos.y = vert_pos.y;
	}
	
	gl_Position = u_projection * u_view * u_model * vec4(result_pos, 1.0f);
	
	frag_pos = vec3(u_model * vec4(result_pos, 1.0f));
	default_pos = vec3(u_model * vec4(vert_pos, 1.0f));
	
	vec3 center = vec3(0.0f, u_body_depth * 0.5, 0.0f);
	
	normal = normalize(vert_pos - center);
}
