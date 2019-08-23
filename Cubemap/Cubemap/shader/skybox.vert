#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 passTexCoord;

void main(){
	passTexCoord = position.xyz;
	mat4 vM = viewMatrix;
	gl_Position = projectionMatrix * vM * position;
}