#version 330 core

in vec3 passTexCoord;
uniform samplerCube cubemap;

out vec4 fragmentColor;

void main(){
	fragmentColor = texture(cubemap, passTexCoord);
}