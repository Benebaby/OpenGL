#version 330 core

uniform sampler2D normalMap;
uniform sampler2D diffuseMap;
uniform sampler2D roughMap;
uniform sampler2D heightMap;
in vec2 passTexCoord;
in vec3 passLightVec;
in vec3 passViewVec;

out vec4 fragmentColor;

void main(){
	vec3 mat_diffuse = texture(diffuseMap, passTexCoord).rgb;
	vec3 normal = normalize(texture(normalMap, passTexCoord).rgb*2.0-1.0);
	vec3 mat_specular = texture(roughMap, passTexCoord).rgb;

	vec3 lightVec = normalize(passLightVec);
	vec3 viewVec = normalize(passViewVec);

	vec3 reflectedViewDir = normalize( reflect(viewVec, normal));

	float cos_phi = max(dot(normal, lightVec), 0);
	float cos_psi_n = pow( max( dot( reflectedViewDir, lightVec), 0.0f), 25.0);

	fragmentColor = vec4(vec3(0.3f) * mat_diffuse + (mat_diffuse * cos_phi + mat_specular * cos_psi_n) * vec3(1.0f), 1.0f);
}