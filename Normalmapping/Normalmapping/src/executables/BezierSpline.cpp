#include "BezierSpline.h"

BezierSpline::BezierSpline(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	controlPoints.push_back(a);
	controlPoints.push_back(b);
	controlPoints.push_back(c);
	vertexArrayHandle = -1;
	vertexBufferHandle = -1;
}

BezierSpline::BezierSpline() {
	vertexArrayHandle = -1;
	vertexBufferHandle = -1;
}

void BezierSpline::extend(glm::vec3 v) {
	controlPoints.push_back(v);
}

BezierSpline::~BezierSpline() {

}

glm::vec3 BezierSpline::deCasteljau(float t) {
	std::vector<glm::vec3> Ptemp = controlPoints;
	for (int n = Ptemp.size() - 2; n >= 0; n--) {
		for (int i = 0; i <= n; i++) {
			Ptemp[i] = interpolate(Ptemp[i], Ptemp[i + 1], t);
		}
	}
	return Ptemp[0];
}

glm::vec3 BezierSpline::interpolate(glm::vec3 a, glm::vec3 b, float t) {
	return (1 - t) * a + t * b;
}

void BezierSpline::finalize() {
	for (int i = 0; i < 200; i++)
		splinePoints.push_back(deCasteljau((float) (i / 200.0)));

	glGenBuffers(1, &vertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, splinePoints.size() * sizeof(glm::vec3), &splinePoints[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArrayHandle);

	glBindVertexArray(vertexArrayHandle);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BezierSpline::draw() {
	glBindVertexArray(vertexArrayHandle);
	glDrawArrays(GL_LINE_LOOP, 0, splinePoints.size());
}
