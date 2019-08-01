#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 passNormal;
out vec3 passPosition;

void main(){
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
	passPosition = (viewMatrix * modelMatrix * position).xyz;
	passNormal = normalize((transpose( inverse( viewMatrix * modelMatrix)) * vec4(normal, 0.0f)).xyz);
}