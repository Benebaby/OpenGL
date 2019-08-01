#version 330 core

in vec3 passPosition;
in vec3 passNormal;
in mat3 passTBN;
in vec2 passUV;

uniform vec3 light_ambient;
uniform mat4 viewMatrix;
uniform sampler2D basemap;
uniform sampler2D normalmap;
uniform sampler2D specularmap;
uniform sampler2D reflectivemap;
uniform samplerCube cubemap;
uniform float mat_shininess;
uniform vec3 light_col;
uniform vec4 light_pos;
uniform int isNormalmapUsed;

out vec4 fragmentColor;

void main(){
	vec3 diffuse = texture(basemap, passUV).rgb;
	vec3 specular = texture(specularmap, passUV).rgb;
	float reflective = texture(reflectivemap, passUV).r;

	vec3 normal;
	if(isNormalmapUsed > 0){
		normal = normalize(texture(normalmap, passUV).rgb);
		normal = 2.0f * normal - 1.0f;
		normal = normalize(normal);
		vec3 tangent = passTBN[0];
		vec3 bitangent = passTBN[1];
		vec3 calcN = cross(tangent, bitangent);
		float normalsAligned = dot(calcN, passTBN[2]);
		if(normalsAligned < 0){
			mat3 TBN_CORRECT = passTBN;
			tangent = -tangent;
			TBN_CORRECT[0] = tangent;
			normal = normalize(TBN_CORRECT * normal);
		} else {
			normal = normalize(passTBN * normal);
		}
	}else{
		normal = normalize(passNormal);
	}

	vec3 light_camcoord = (viewMatrix * light_pos).xyz;
	vec3 lightVec;
	if (light_pos.w > 0.001f)
		lightVec = normalize( light_camcoord - passPosition);
	else
		lightVec = normalize(light_camcoord);
	vec3 viewVec = normalize(-passPosition);

	vec3 reflectedViewDir = normalize( reflect(-viewVec, normal));

	float cos_phi = max( dot( normal, lightVec), 0.0f);
	float cos_psi_n = pow( max( dot( reflectedViewDir, lightVec), 0.0f), 50.0f);

	reflectedViewDir = (inverse(viewMatrix) * vec4(reflectedViewDir, 0.0)).xyz;

	vec3 mixedDiffuse = mix(diffuse, texture(cubemap, reflectedViewDir).rgb, reflective * 0.9);

	fragmentColor = vec4(light_ambient * mixedDiffuse + (mixedDiffuse * cos_phi + specular * cos_psi_n) * light_col, 1.0);
}