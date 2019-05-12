#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "../Tools/ShaderSet.h"
#include "../Tools/TextureTools.h"
#include "Camera.h"
#include "BezierSpline.h"
#include "Cube.h"

const int width = 1600;
const int height = 900;

GLFWwindow* window;
Camera cam = Camera(width, height);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cam.radius -= (float)(yoffset * 0.1);
	cam.radius = glm::max(cam.radius, 0.000001f);
}

void KeyCallback(GLFWwindow *window, int key, int scanmode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		cam.Newpos = glm::vec2(xpos, ypos);
		float xAngle = (cam.Newpos.x - cam.Oldpos.x) / cam.m_width * 2 * cam.Pi;
		float yAngle = (cam.Newpos.y - cam.Oldpos.y) / cam.m_height * cam.Pi;
		cam.Angle -= glm::vec2(xAngle, yAngle);
		cam.Angle.y = glm::max(cam.Angle.y, 0.000001f);
		cam.Angle.y = glm::min(cam.Angle.y, cam.Pi);

	}
	cam.Oldpos = glm::vec2(xpos, ypos);
}

int main(void)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Normalmapping", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetCursorPosCallback(window, &MouseCallback);
	glfwSetScrollCallback(window, &ScrollCallback);
	glfwSetKeyCallback(window, &KeyCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::cout << glGetString(GL_VERSION) << std::endl;

	const char *normalshaderfiles[2] = { SHADERS_PATH"/normalmap.vert", SHADERS_PATH"/normalmap.frag" };
	ShaderSet normalshaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, normalshaderfiles);
	GLuint modelMatrixID = glGetUniformLocation(normalshaderProgram.getProgramID(), "modelMatrix");
	GLuint viewMatrixID = glGetUniformLocation(normalshaderProgram.getProgramID(), "viewMatrix");;
	GLuint projectionMatrixID = glGetUniformLocation(normalshaderProgram.getProgramID(), "projectionMatrix");
	GLuint lightID = glGetUniformLocation(normalshaderProgram.getProgramID(), "light_pos");
	GLuint viewID = glGetUniformLocation(normalshaderProgram.getProgramID(), "view_pos");
	GLuint tex1ID = glGetUniformLocation(normalshaderProgram.getProgramID(), "normalMap");
	GLuint tex2ID = glGetUniformLocation(normalshaderProgram.getProgramID(), "diffuseMap");
	GLuint tex3ID = glGetUniformLocation(normalshaderProgram.getProgramID(), "roughMap");
	GLuint tex4ID = glGetUniformLocation(normalshaderProgram.getProgramID(), "heightMap");

	const char *minimalshaderfiles[2] = { SHADERS_PATH"/minimal.vert", SHADERS_PATH"/minimal.frag" };
	ShaderSet minimalShaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, minimalshaderfiles);
	GLuint m_modelMatrixID = glGetUniformLocation(minimalShaderProgram.getProgramID(), "modelMatrix");
	GLuint m_viewMatrixID = glGetUniformLocation(minimalShaderProgram.getProgramID(), "viewMatrix");;
	GLuint m_projectionMatrixID = glGetUniformLocation(minimalShaderProgram.getProgramID(), "projectionMatrix");
	GLuint m_colorID = glGetUniformLocation(minimalShaderProgram.getProgramID(), "color");

	glm::mat4 modelMatrix(1.0f);
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(40.0f), (float)width / height, 0.01f, 100.0f);
	glm::vec4 light = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f);

	GLfloat vertices[] = { -1.0, 0.0, -1,  -1, 0.0, 1,  1, 0.0, -1,  1, 0.0, -1,  -1, 0.0, 1,  1, 0.0f, 1 };
	GLfloat normals[] = { 0.0, 1.0, 0.0,  0.0, 1.0, 0.0,  0.0, 1.0, 0.0,  0.0, 1.0, 0.0,  0.0, 1.0, 0.0,  0.0, 1.0, 0.0 };
	GLfloat texcoords[] = { 0.0, 0.0,  0.0, 2.0,  2.0, 0.0,  2.0, 0.0,  0.0, 2.0,  2.0, 2.0 };
	GLfloat tangent[] = { 1.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 0.0,  1.0, 0.0, 0.0 };

	GLuint vertexArrayHandle;
	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);

	GLuint vertexBufferHandle[4];
	glGenBuffers(4, vertexBufferHandle);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), &normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), &texcoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tangent), &tangent[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint t_normalmap = TextureTools::loadTexture(BUMPMAP_PATH "/cobblestone.png");
	GLuint t_basemap = TextureTools::loadTexture(BASEMAP_PATH "/cobblestone.png");
	GLuint t_roughmap = TextureTools::loadTexture(ROUGHMAP_PATH "/cobblestone.png");
	GLuint t_heightmap = TextureTools::loadTexture(HEIGHTMAP_PATH "/cobblestone.png");

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	normalshaderProgram.UseProgram();
	glUniform1i(tex1ID, 0);
	glUniform1i(tex2ID, 1);
	glUniform1i(tex3ID, 2);
	glUniform1i(tex4ID, 3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t_normalmap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_basemap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, t_roughmap);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, t_heightmap);
	glUniformMatrix4fv(modelMatrixID, 1, false, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(projectionMatrixID, 1, false, glm::value_ptr(projectionMatrix));

	minimalShaderProgram.UseProgram();
	glUniformMatrix4fv(m_modelMatrixID, 1, false, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(m_projectionMatrixID, 1, false, glm::value_ptr(projectionMatrix));
	glUniform3fv(m_colorID, 1, glm::value_ptr(glm::vec3(1.0)));

	BezierSpline* kurve = new BezierSpline(glm::vec3(-2.0, 1.0, 0.0), glm::vec3(-2.0, 1.0, 3.2), glm::vec3(3.2, 1.0, 3.2));
	kurve->extend(glm::vec3(3.2, 1.0, 0.0));
	kurve->extend(glm::vec3(3.2, 1.0, -3.2));
	kurve->extend(glm::vec3(-2.0, 1.0, -3.2));
	kurve->extend(glm::vec3(-2.0, 1.0, 0.0));
	kurve->finalize();

	Cube* lichtwürfel = new Cube();

	float t = 0.0;
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window))
	{
		t += 0.001;
		if (t > 1.0)
			t = 0.0;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cam.update();

		normalshaderProgram.UseProgram();
		glBindVertexArray(vertexArrayHandle);
		glUniformMatrix4fv(viewMatrixID, 1, false, glm::value_ptr(cam.getViewMatrix()));
		light = glm::vec4(kurve->deCasteljau(t),1.0f);
		glUniform4fv(lightID, 1, glm::value_ptr(light));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		minimalShaderProgram.UseProgram();
		modelMatrix = glm::mat4(1.0f);
		glUniformMatrix4fv(m_modelMatrixID, 1, false, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(m_viewMatrixID, 1, false, glm::value_ptr(cam.getViewMatrix()));
		kurve->draw();
		modelMatrix = glm::translate(modelMatrix, kurve->deCasteljau(t));
		glUniformMatrix4fv(m_modelMatrixID, 1, false, glm::value_ptr(modelMatrix));
		lichtwürfel->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete kurve;
	glfwTerminate();
	return 0;
}