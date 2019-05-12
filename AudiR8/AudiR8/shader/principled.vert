#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;

uniform int type;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;
uniform vec4 light_pos;

out vec3 passNormal;
out vec3 passPosition;
out vec2 passUV;
out vec4 passShadowCoords;
out vec3 passLightVec;
out vec3 passViewVec;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
	passShadowCoords = lightProjectionMatrix * lightViewMatrix * modelMatrix * position;
	passNormal = normalize((transpose( inverse( viewMatrix * modelMatrix)) * vec4(normal, 0.0f)).xyz);

	if(type == 0){
		gl_Position = lightProjectionMatrix * lightViewMatrix * modelMatrix * position;
	}
	else if(type == 1 || type == 2){
		passPosition = (viewMatrix * modelMatrix * position).xyz;
		if(type == 2){
			passUV = texCoord;
		}
	}
	else if(type == 3){
		mat4 normalMatrix = transpose(inverse(viewMatrix * modelMatrix));

		vec3 N = normalize((normalMatrix * vec4(normal,0)).xyz);
		vec3 T = normalize((normalMatrix * vec4(tangent,0)).xyz);
		vec3 B = cross(T, N);
		mat3 TBN = transpose(mat3(T, B, N));

		vec3 position_cs = (viewMatrix * modelMatrix * position).xyz;
		vec3 eye_cs = position_cs;
		vec3 lightPosition_cs = (viewMatrix * light_pos).xyz;

		passLightVec = TBN * normalize(lightPosition_cs);
		passViewVec = TBN * eye_cs;
		passUV = texCoord;
	}
	else if(type == 5){
		passViewVec = (viewMatrix * modelMatrix * position).xyz;
		passLightVec = (viewMatrix * light_pos).xyz;
	}
	else if(type == 6){
		passPosition = position.xyz;
		mat4 vM = mat4(mat3(viewMatrix));
		gl_Position = projectionMatrix * vM * position;
	}
}