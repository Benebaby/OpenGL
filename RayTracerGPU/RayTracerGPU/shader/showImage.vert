#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 texCoords;

out vec2 passTexCoords;

void main(){
	passTexCoords = texCoords;
    gl_Position = Position;
}