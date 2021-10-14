#version 460 core

layout (location = 0) in vec3 vpos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_time;

uniform vec3 u_light_0;

out vec3 light_0;
out float ambient_strength_0;

out vec3 pos;

#define PI_f 3.1415926538f

vec3 gerstner_wave(vec4 wave, vec3 pos) {
	float steepness = wave.z;
	float wavelength = wave.w;
	float k = 2.0f * PI_f / wavelength;
	float c = sqrt(9.8f / k);
	vec2 d = normalize(wave.xy);
	float f = k * (dot(d, pos.xz) - c * u_time);
	float a = steepness / k;
	return vec3(d.x * (a * cos(f)), a * sin(f), d.y * (a * cos(f)));
}

void main() {
	// direction.x, direction.y, steepness, wavelength
	vec4 waveA = vec4(1.0f, 1.0f, 0.2f, 60.0f);
	vec4 waveB = vec4(1.0f, 0.6f, 0.2f, 31.0f);
	vec4 waveC = vec4(1.0f, 1.3f, 0.2f, 18.0f);
	vec4 waveD = vec4(0.5f, 1.2f, 0.5f, 5.0f);
	vec4 waveE = vec4(-0.7f, 1.1f, 0.5f, 8.0f);
	vec4 waveF = vec4(-0.2f, 1.2f, 0.55f, 4.0f);
	vec4 waveG = vec4(0.8f, 0.3f, 0.48f, 6.0f);
	vec4 waveH = vec4(0.5f, 0.6f, 0.40f, 100.0f);
	vec4 waveZ = vec4(0.9f, 0.5f, 0.07f, 150.0f);
	
	vec3 grid_pos = vpos;
	vec3 p = grid_pos;
	p += gerstner_wave(waveA, grid_pos);
	p += gerstner_wave(waveB, grid_pos);
	p += gerstner_wave(waveC, grid_pos);
	p += gerstner_wave(waveH, grid_pos) * vec3(0.1f, 0.2f, 0.1f);
	p += gerstner_wave(waveZ, grid_pos);
	
	float sub = max(0.03f, p.y * 0.05f);
	p += gerstner_wave(waveD, grid_pos) * vec3(0.1f, 0.2f - sub, 0.1f);
	p += gerstner_wave(waveE, grid_pos) * vec3(0.1f, 0.2f - sub, 0.1f);
	p += gerstner_wave(waveF, grid_pos) * vec3(0.1f, 0.2f - sub, 0.1f);
	p += gerstner_wave(waveG, grid_pos) * vec3(0.1f, 0.2f - sub, 0.1f);
	
	vec3 P = p;
	
	p.y += vpos.x * vpos.x * -0.00005f + vpos.z * vpos.z * -0.00005f;
	p.y = max(p.y, -110.0f);
	
	gl_Position = u_projection * u_view * u_model * vec4(p.x, p.y, p.z, 1.0f);
	pos = vec3(P.x, P.y, P.z);
	
	light_0 = u_light_0;
	ambient_strength_0 = 1.0f;
}
