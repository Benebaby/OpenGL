#ifndef BEZIERSPLINE_H_
#define BEZIERSPLINE_H_
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class BezierSpline {
public:
	BezierSpline(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	BezierSpline();
	virtual ~BezierSpline();
	void extend(glm::vec3 v);
	void finalize();
	void draw();
	glm::vec3 deCasteljau(float t);
	glm::vec3 interpolate(glm::vec3 a, glm::vec3 b, float t);
private:
	std::vector<glm::vec3> controlPoints;
	std::vector<glm::vec3> splinePoints;
	GLuint vertexArrayHandle;
	GLuint vertexBufferHandle;
};

#endif
