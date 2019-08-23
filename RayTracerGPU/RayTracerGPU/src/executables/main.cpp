#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "../Tools/ShaderSet.h"

const int width = 1600;
const int height = 900;

GLFWwindow* window;

int main(void)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Test", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwMakeContextCurrent(window);
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::cout << glGetString(GL_VERSION) << std::endl;

	const char *shaderfiles[2] = {SHADERS_PATH"/minimal.vert", SHADERS_PATH"/minimal.frag" };
	ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shaderfiles);

	GLfloat vertices[] = { -0.4, -0.4,  -0.4, 0.4,  0.4, -0.4 };

	GLuint vertexArrayHandle;
	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);

	GLuint vertexBufferHandle;
	glGenBuffers(1, &vertexBufferHandle);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shaderProgram.UseProgram();
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}