#version 330 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 Tangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 light_pos;

out vec2 passTexCoord;
out vec3 passLightVec;
out vec3 passViewVec;

void main(){
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * Position;

	mat4 normalMatrix = transpose(inverse(viewMatrix * modelMatrix));
	vec3 N = normalize((normalMatrix * vec4(Normal,0)).xyz);
	vec3 T = normalize((normalMatrix * vec4(Tangent,0)).xyz);
	vec3 B = cross(T, N);
	mat3 TBN = transpose(mat3(T, B, N));

	vec3 position_cs = (viewMatrix * modelMatrix * Position).xyz;
	vec3 lightPosition_cs = (viewMatrix * light_pos).xyz;

	passLightVec = TBN * normalize(lightPosition_cs - position_cs);
	passViewVec = TBN * position_cs;
	passTexCoord = texCoord;
}