#version 330 core

out vec4 fragmentColor;
uniform sampler2D Image;

in vec2 passTexCoords;

void main(){
	fragmentColor = vec4(texture(Image, passTexCoords).rgb, 1.0);
}