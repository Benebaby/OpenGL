#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 PassPosition;
out vec3 PassNormal;
out mat3 PassTBN;
out vec2 PassUV;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

	mat4 normalMatrix = transpose(inverse(viewMatrix * modelMatrix));
	vec3 T = normalize((normalMatrix * vec4(tangent, 0.0)).xyz);
	vec3 N = normalize((normalMatrix * vec4(normal, 0.0)).xyz);
	vec3 B = cross(N, T);
	PassTBN = mat3(T, B, N);
	PassPosition = (viewMatrix * modelMatrix * position).xyz;
	PassNormal = (normalMatrix * vec4(normal, 0.0)).xyz;
	PassUV = texCoords;
}