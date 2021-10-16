#version 460 core

out vec4 frag_color;

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light u_light;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material u_material;

uniform vec3 u_view_pos;

in vec3 normal;

in vec3 frag_pos;

void main() {
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(u_light.position - frag_pos);
	
	// ambient
	vec3 ambient = u_light.ambient * u_material.ambient;
	
	// diffuse
	float diff = max(dot(norm, light_dir), 0.0f);
	vec3 diffuse = u_light.diffuse * (u_material.diffuse * diff);
	
	// specular
	vec3 view_dir = normalize(u_view_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), u_material.shininess);
	vec3 specular = u_light.specular * (u_material.specular * spec);
	
	// final
	vec3 final = ambient + diffuse + specular;
	frag_color = vec4(final, 1.0f);
}
