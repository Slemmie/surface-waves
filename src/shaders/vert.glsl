#version 460 core

layout (location = 0) in vec3 vpos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

uniform float u_time;

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
	vec4 waveA = vec4(1.0f, 1.0f, 0.25f, 60.0f);
	vec4 waveB = vec4(1.0f, 0.6f, 0.25f, 31.0f);
	vec4 waveC = vec4(1.0f, 1.3f, 0.25f, 18.0f);
	
	vec3 grid_pos = vpos;
	vec3 p = grid_pos;
	p += gerstner_wave(waveA, grid_pos);
	p += gerstner_wave(waveB, grid_pos);
	p += gerstner_wave(waveC, grid_pos);
	
	//p *= 0.4f;
	
	gl_Position = u_projection * u_view * u_model * vec4(p.x, p.y, p.z, 1.0f);
	pos = vec3(p.x, p.y, p.z);
}
