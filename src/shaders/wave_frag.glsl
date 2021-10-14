#version 460 core

out vec4 color;

in vec3 light_0;
in float ambient_strength_0;
in vec3 normal;
in vec3 light_0_pos;

in vec3 pos;

void main() {
	vec3 norm = normalize(normal);
	
	color = vec4(0.3f, 0.3f, 0.6f, 1.0f);
	
	// ambient
	vec3 ambient = light_0 * ambient_strength_0;
	// diffuse
	vec3 light_0_dir = normalize(light_0_pos - pos);
	vec3 diffuse = max(dot(norm, light_0_dir), 0.0f) * light_0;
	// final
	vec3 final = (ambient + diffuse) * color.xyz;
	color = vec4(final, 1.0f);
}
