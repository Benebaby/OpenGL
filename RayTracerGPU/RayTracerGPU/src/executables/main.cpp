#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "../Tools/ShaderSet.h"
#include "Image.h"
#include "Camera.h"
#include "ScreenQuad.h"

const int width = 1600;
const int height = 900;

GLFWwindow* window;
Camera* cam = new Camera(width, height);
Image* im;

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cam->updateRadius(yoffset);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		cam->updateCamPos(xpos, ypos, true);
	else
		cam->updateCamPos(xpos, ypos, false);
}

void rayTraceImage()
{
	glm::vec3 from, xvec, yvec, zvec;
	cam->getView(&xvec, &yvec, &zvec, &from);
	float w = (float)im->m_width;
	float h = (float)im->m_height;

	float s = glm::tan(glm::radians(0.5f * 60.f));
	float s_ = s * w / h;

	glm::vec3 origin = from - zvec - s * yvec - s_ * xvec;
	glm::vec3 steprightvec = xvec * (2.f * s_ / w);
	glm::vec3 stepupvec = yvec * (2.f * s / h);

	for (unsigned int y = 0; y < im->m_height; y++) {
		for (unsigned int x = 0; x < im->m_width; x++) {
			glm::vec3 color = glm::vec3((float)x / 1600, (float)y/900, 1.0f);
			im->set(x, y, color);
		}
	}
	im->upload();
}

int main(void)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "RayTracerGPU", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwMakeContextCurrent(window);
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::cout << glGetString(GL_VERSION) << std::endl;

	const char* shaderfiles[2] = { SHADERS_PATH"/minimal.vert", SHADERS_PATH"/minimal.frag" };
	ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shaderfiles);

	im = new Image(1600, 900);
	ScreenQuad quad = ScreenQuad();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.UseProgram();
		rayTraceImage();
		quad.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}