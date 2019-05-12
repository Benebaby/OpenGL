#ifndef __SHADERSET_H
#define __SHADERSET_H

#include "Defs.h"

class ShaderSet
{
public:
	ShaderSet();
	ShaderSet(GLuint shader_mask, const char** ShaderNames);
	~ShaderSet();

	void GenerateShaderProgramm(GLuint shader_mask, const char** ShaderNames);
	void UseProgram();
	GLuint getProgramID();

private:
	void checkShader(GLuint shaderID);
	void checkProgram(GLuint programID);
	void loadShaderSource(GLint shaderID, const char* fileName);

	GLuint m_shader_mask;

protected:
	GLuint m_ProgramID;
};
#endif /*__SHADERSET_H*/
