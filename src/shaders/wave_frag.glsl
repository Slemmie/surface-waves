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

#define PI_f 3.1415926538f

void main() {
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(u_light.position - frag_pos);
	vec3 view_dir = normalize(u_view_pos - frag_pos);
	
	// don't color if facing away
	vec3 surf_normal = cross(dFdx(frag_pos), dFdy(frag_pos));
	surf_normal = normalize(surf_normal * sign(surf_normal.z));
	if (acos(dot(view_dir, surf_normal)) > PI_f * 1.0f) {
		frag_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}
	
	// ambient
	vec3 ambient = u_light.ambient * u_material.ambient;
	
	// diffuse
	float diff = max(dot(norm, light_dir), 0.0f);
	vec3 diffuse = u_light.diffuse * (u_material.diffuse * diff);
	
	// specular
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), u_material.shininess);
	vec3 specular = u_light.specular * (u_material.specular * spec);
	
	// final
	vec3 final = ambient + diffuse + specular;
	frag_color = vec4(final, 0.8f);
}
