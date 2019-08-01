#version 330 core

in vec3 passPosition;
in vec3 passNormal;

uniform mat4 viewMatrix;
uniform vec3 light_ambient;

uniform struct
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
} mat;

uniform struct
{
	vec4 pos;
	vec3 col;
} light;

out vec4 fragmentColor;

void main(){
	vec3 normal = normalize(passNormal);
	vec3 lightVector;
	// Diffuse
	vec3 light_camcoord = (viewMatrix * light.pos).xyz;
	if (light.pos.w > 0.001f)
		lightVector = normalize( light_camcoord - passPosition);
	else
		lightVector = normalize(light_camcoord);
	float cos_phi = max(dot( normal, lightVector), 0.0);
	// Specular
	vec3 eye = normalize( -passPosition);
	vec3 reflection = normalize( reflect( -lightVector, normal));
	float cos_psi_n = pow( max( dot( reflection, eye), 0.0f), mat.shininess);

	fragmentColor = vec4(mat.diffuse * light_ambient + (mat.diffuse * cos_phi + mat.specular * cos_psi_n) * light.col, 1.0f);
}