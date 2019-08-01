#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 PassPosition[];
in vec3 PassNormal[];
in mat3 PassTBN[];
in vec2 PassUV[];

out vec3 passPosition;
out vec3 passNormal;
out mat3 passTBN;
out vec2 passUV; 

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 10.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
} 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

void main() {    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
	passPosition = PassPosition[0];
	passNormal = PassNormal[0];
	passTBN = PassTBN[0];
	passUV = PassUV[0]; 
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    passPosition = PassPosition[1];
	passNormal = PassNormal[1];
	passTBN = PassTBN[1];
	passUV = PassUV[1]; 
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    passPosition = PassPosition[2];
	passNormal = PassNormal[2];
	passTBN = PassTBN[2];
	passUV = PassUV[2]; 
    EmitVertex();
    EndPrimitive();
}  