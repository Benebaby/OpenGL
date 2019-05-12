#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 light_pos;

out vec2 passTexCoords;
out vec3 passLightVec;
out vec3 passViewVec;

void main(){
	
	mat4 normalMatrix = transpose(inverse(viewMatrix * modelMatrix));
	vec3 N = normalize((normalMatrix * vec4(normal,0)).xyz);
	vec3 T = normalize((normalMatrix * vec4(tangent,0)).xyz);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));
	
	vec3 position_cs = (viewMatrix * modelMatrix * position).xyz;
	vec3 eye_cs = position_cs;
	vec3 lightPosition_cs = (viewMatrix * light_pos).xyz;

	passLightVec = TBN * normalize(lightPosition_cs);
	passViewVec = TBN * eye_cs;
	passTexCoords = texCoords;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}