#version 460 core

layout (location = 0) in vec3 vert_pos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_time;

uniform vec3 u_view_pos;

out vec3 normal;

out vec3 frag_pos;

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

vec3 dfdp0_gerstner_wave(vec4 wave, vec3 pos) {
	float steepness = wave.z;
	float wavelength = wave.w;
	vec2 d = normalize(wave.xy);
	float b = 2.0f * PI_f / wavelength * (dot(d, pos.xz) - sqrt(9.8f * wavelength / (2.0f * PI_f)) * 1.5f * u_time);
	return vec3(
	-d.x * d.x * steepness * sin(b),
	d.x * steepness * cos(b),
	-d.x * d.y * steepness * sin(b));
}

vec3 dfdp1_gerstner_wave(vec4 wave, vec3 pos) {
	float steepness = wave.z;
	float wavelength = wave.w;
	vec2 d = normalize(wave.xy);
	float b = 2.0f * PI_f / wavelength * (dot(d, pos.xz) - sqrt(9.8f * wavelength / (2.0f * PI_f)) * 1.5f * u_time);
	return vec3(
	-d.x * d.y * steepness * sin(b),
	d.y * steepness * cos(b),
	-d.y * d.y * steepness * sin(b));
}

void main() {
	
	vec3 result_pos = vert_pos;
	for (int i = 0; i < WAVE_COUNT; i++) {
		result_pos += gerstner_wave(waves[i], vert_pos) * wave_scale[i];
	}
	result_pos.y += vert_pos.x * vert_pos.x * -0.00005f + vert_pos.z * vert_pos.z * -0.00005f;
	result_pos.y = max(result_pos.y, -110.0f);
	
	vec3 p_dfdp0 = vec3(1.0f, 0.0f, 0.0f);
	vec3 p_dfdp1 = vec3(0.0f, 0.0f, 1.0f);
	for (int i = 0; i < WAVE_COUNT; i++) {
		p_dfdp0 += dfdp0_gerstner_wave(waves[i], result_pos) * wave_scale[i];
		p_dfdp1 += dfdp1_gerstner_wave(waves[i], result_pos) * wave_scale[i];
	}
	p_dfdp0 += result_pos.x * -0.0001f;
	p_dfdp1 += result_pos.z * -0.0001f;
	
	gl_Position = u_projection * u_view * u_model * vec4(result_pos, 1.0f);
	
	frag_pos = vec3(u_model * vec4(result_pos, 1.0f));
	
	normal = normalize(mat3(transpose(inverse(u_model))) * normalize(cross(p_dfdp1, p_dfdp0)));
}
