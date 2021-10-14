#version 460 core

layout (location = 0) in vec3 vpos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_time;

uniform vec3 u_light_0;
uniform vec3 u_light_0_pos;

out vec3 light_0;
out float ambient_strength_0;
out vec3 normal;
out vec3 light_0_pos;

out vec3 pos;

#define PI_f 3.1415926538f

// direction.x, direction.y, steepness, wavelength
vec4 waveA = vec4(1.0f, 1.0f, 0.2f, 60.0f);
vec4 waveB = vec4(1.0f, 0.6f, 0.2f, 31.0f);
vec4 waveC = vec4(1.0f, 1.3f, 0.2f, 18.0f);
vec4 waveH = vec4(0.5f, 0.6f, 0.40f, 100.0f);
vec4 waveZ = vec4(0.9f, 0.5f, 0.07f, 150.0f);

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

vec3 get_normal(vec3 pos) {
	vec3 p_dfdp0 = vec3(1.0f, 0.0f, 0.0f) + 
	dfdp0_gerstner_wave(waveA, pos) +
	dfdp0_gerstner_wave(waveB, pos) +
	dfdp0_gerstner_wave(waveC, pos) +
	dfdp0_gerstner_wave(waveH, pos) * vec3(0.1f, 0.2f, 0.1f) +
	dfdp0_gerstner_wave(waveZ, pos) + 
	pos.x * -0.00005f * 2.0f;
	
	vec3 p_dfdp1 = vec3(0.0f, 0.0f, 1.0f) + 
	dfdp1_gerstner_wave(waveA, pos) +
	dfdp1_gerstner_wave(waveB, pos) +
	dfdp1_gerstner_wave(waveC, pos) +
	dfdp1_gerstner_wave(waveH, pos) * vec3(0.1f, 0.2f, 0.1f) +
	dfdp1_gerstner_wave(waveZ, pos) + 
	pos.z * -0.00005f * 2.0f;
	
	return cross(p_dfdp1, p_dfdp0);
}

void main() {
	
	vec3 p = vpos;
	p += gerstner_wave(waveA, vpos);
	p += gerstner_wave(waveB, vpos);
	p += gerstner_wave(waveC, vpos);
	p += gerstner_wave(waveH, vpos) * vec3(0.1f, 0.2f, 0.1f);
	p += gerstner_wave(waveZ, vpos);
	
	vec3 P = p;
	
	p.y += vpos.x * vpos.x * -0.00005f + vpos.z * vpos.z * -0.00005f;
	p.y = max(p.y, -110.0f);
	
	gl_Position = u_projection * u_view * u_model * vec4(p.x, p.y, p.z, 1.0f);
	pos = vec3(u_model * vec4(P.x, P.y, P.z, 1.0f));
	
	light_0 = u_light_0;
	ambient_strength_0 = 0.15f;
	normal = normalize(mat3(transpose(inverse(u_model))) * normalize(get_normal(p)));
	light_0_pos = u_light_0_pos;
}
