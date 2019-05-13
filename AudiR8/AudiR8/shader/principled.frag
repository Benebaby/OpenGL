#version 330 core

in vec3 passNormal;
in vec3 passPosition;
in vec2 passUV;
in vec4 passShadowCoords;
in vec3 passLightVec;
in vec3 passViewVec;

uniform int type;
uniform vec3 ambiente;
uniform vec4 diffuse;
uniform float shininess;
uniform float roughness;
uniform int metalness;
uniform vec3 specular;
uniform vec4 light_pos;
uniform mat4 viewMatrix;

uniform sampler2D basemap;
uniform sampler2D normalmap;
uniform sampler2D roughmap;
uniform sampler2D depthMap;
uniform samplerCube skybox;
uniform samplerCube reflectionCube;

vec3 light_color = vec3(0.8);
float shadow = 0.0;

out vec4 fragmentColor;

void calcShadow(){
	vec3 shadowCoord = passShadowCoords.xyz / passShadowCoords.w;
	shadowCoord = shadowCoord * 0.5 + 0.5;
	float currentDepth = shadowCoord.z;
	if(currentDepth < 1.0)
	{
	vec3 light_camcoord = (viewMatrix * light_pos).xyz;
	vec3 lightVector = normalize( light_camcoord);
	float cos_theta = dot( passNormal, lightVector);
	float bias = 0.00002 * tan(acos(cos_theta));
	bias = clamp(bias, 0.0015, 0.01);
	vec2 texelSize = 1.0 / textureSize(depthMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(depthMap, shadowCoord.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 0.0 : 1.0;        
		}    
	}
	shadow /= 9.0;
	}else
		shadow = 1.0;
}

void main(){
	
	calcShadow();

	if(type == 0){
	}
	else if(type == 1 || type == 2){
		vec3 light_camcoord = (viewMatrix * light_pos).xyz;
		vec3 lightVector = normalize( light_camcoord);
		vec3 eye = normalize( -passPosition);
		vec3 reflection = normalize( reflect( -lightVector, passNormal));

		float cos_phi = max( dot( passNormal, lightVector), 0.0f);
		float cos_psi_n = pow( max( dot( reflection, eye), 0.0f), shininess);

	
		if(type == 2)
			fragmentColor = vec4(texture(basemap, passUV).rgb * ambiente + (texture(basemap, passUV).rgb * cos_phi + specular * cos_psi_n) * shadow * light_color, 1.0f);
		else
			fragmentColor = vec4(diffuse.xyz * ambiente + (diffuse.xyz * cos_phi + specular * cos_psi_n) * shadow * light_color, 1.0f);
	}

	else if(type == 3){
		vec3 mat_diffuse = texture(basemap, passUV).rgb;
		vec3 mat_specular = texture(roughmap, passUV).rgb;
		vec3 normal = normalize(texture(normalmap, passUV).rgb*2.0-1.0);

		vec3 lightVec = normalize(passLightVec);
		vec3 viewVec = normalize(passViewVec);

		vec3 reflectedViewDir = normalize( reflect(viewVec, normal));

		float cos_phi = max(dot(normal, lightVec), 0);
		float cos_psi_n = pow( max( dot( reflectedViewDir, lightVec), 0), shininess);

		fragmentColor = vec4(ambiente * mat_diffuse + (mat_diffuse * cos_phi + mat_specular * cos_psi_n) * shadow * light_color, 1.0f);
	}

	else if(type == 5){
		vec3 normal = normalize(passNormal);
		vec3 lightVec = normalize(passLightVec);
		vec3 viewVec = normalize(passViewVec);

		vec3 reflectedViewDir = reflect(viewVec, normal);

		float cos_phi = max(dot(normal, lightVec), 0);
		float cos_psi_n = pow( max( dot( reflectedViewDir, lightVec), 0), shininess);

		reflectedViewDir = vec3( inverse(viewMatrix) * vec4( reflectedViewDir, 0.0));

		vec3 mixedDiffuse = mix(0.5* diffuse, texture(reflectionCube, reflectedViewDir), 0.4 * metalness).xyz;

		fragmentColor = vec4(ambiente * mixedDiffuse + shadow * (mixedDiffuse * cos_phi + specular * cos_psi_n) * light_color, 1.0f);
	}

	else if(type == 6){
		fragmentColor = texture(skybox, passPosition);
	}
}