#version 330 core
out vec4 fragmentColor;
  
in vec2 passTexCoords;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, passTexCoords).r;
    fragmentColor = vec4(vec3(depthValue), 1.0);
}  