#version 330 core

in vec3 passPosition;
in vec3 passNormal;
in vec4 passShadowCoord;

uniform mat4 viewMatrix;
uniform vec3 light_ambient;

uniform sampler2D depthMap;

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
	float cos_phis = dot( normal, lightVector);
	float cos_phi = max(cos_phis, 0.0);
	// Specular
	vec3 eye = normalize( -passPosition);
	vec3 reflection = normalize( reflect( -lightVector, normal));
	float cos_psi_n = pow( max( dot( reflection, eye), 0.0f), mat.shininess);

	// Shadow
	vec3 shadowCoord = passShadowCoord.xyz / passShadowCoord.w;
	shadowCoord = shadowCoord * 0.5 + 0.5;
	float shadow = 0.0;
	float currentDepth = shadowCoord.z;
	if(currentDepth < 1.0)
	{
		float bias = 0.00002 * tan(acos(cos_phi));
		bias = clamp(bias, 0.001, 0.01);
		vec2 texelSize = 1.0 / textureSize(depthMap, 0);
		for(int x = -2; x <= 2; ++x)
		{
			for(int y = -2; y <= 2; ++y)
			{
				float pcfDepth = texture(depthMap, shadowCoord.xy + vec2(x, y) * texelSize).r; 
				shadow += currentDepth - bias > pcfDepth ? 0.0 : 1.0;        
			}    
		}
		shadow /= 25.0;
	}else
		shadow = 1.0;

	fragmentColor = vec4(mat.diffuse * light_ambient + (mat.diffuse * cos_phi + mat.specular * cos_psi_n) * light.col * shadow, 1.0f);
}