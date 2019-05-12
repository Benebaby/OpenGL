#version 330 core

in vec2 passTexCoords;
in vec3 passLightVec;
in vec3 passViewVec;

uniform samplerCube cubemap;
uniform sampler2D albedo;
uniform sampler2D roughness;
uniform sampler2D metalness;
uniform sampler2D normalmap;
uniform mat4 viewMatrix;

out vec4 fragmentColor;

void main(){
	vec3 diffuse = texture(albedo, passTexCoords).rgb;
	vec3 specular = texture(roughness, passTexCoords).rgb;
	float metal = texture(metalness, passTexCoords).r;
	vec3 normal = normalize(texture(normalmap, passTexCoords).rgb * 2.0 - 1.0);
	vec3 ambiente  = vec3(0.4);
	
	vec3 lightVec = normalize(passLightVec);
	vec3 viewVec = normalize(passViewVec);

	vec3 reflectedViewDir = normalize( reflect(viewVec, normal));

	float cos_phi = max( dot( normal, lightVec), 0.0f);
	float cos_psi_n = pow( max( dot( reflectedViewDir, lightVec), 0.0f), 50.0f);

	reflectedViewDir = vec3(inverse(viewMatrix) * vec4(reflectedViewDir, 0));
	
	vec3 mixedDiffuse = mix(vec4(diffuse, 1.0), texture( cubemap, reflectedViewDir), 0.4 * metal).xyz;

	fragmentColor = vec4(ambiente * mixedDiffuse + (cos_phi * mixedDiffuse + cos_psi_n * specular),1.0);
}