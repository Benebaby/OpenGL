#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "../Tools/ShaderSet.h"
#include "Texture.h"
#include "Camera.h"
#include "ScreenQuad.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Box.h"
#include "Scene.h"
#include "Material.h"

#define WIDTH 1600
#define HEIGHT 900
#define FOV 45.

GLFWwindow * window;
Camera* cam = new Camera(WIDTH, HEIGHT, FOV);
ShaderSet* rayTracerProgram;
ShaderSet* showImageProgram;
ScreenQuad* quad;
Texture* image;
Texture* test;
Scene* scene;
GLuint loc_from, loc_xvec, loc_yvec, loc_zvec, loc_lightPos, loc_Eye, loc_image_size, loc_image, loc_FOV;
glm::vec4 light = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cam->updateRadius(yoffset);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		cam->updateCamPos(xpos, ypos, true);
	else
		cam->updateCamPos(xpos, ypos, false);
}

void resizeCallback(GLFWwindow* window, int w, int h)
{
	image = new Texture(w, h, 4);
	glViewport(0, 0, w, h);
}

void initWindow() {
	glfwInit();
	window = glfwCreateWindow(WIDTH, HEIGHT, "RayTracerGPU", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwSetCursorPosCallback(window, &MouseCallback);
	glfwSetScrollCallback(window, &ScrollCallback);
	glfwSetWindowSizeCallback(window, &resizeCallback);
	glfwMakeContextCurrent(window);
	glewInit();
}

void initOpenGL() {
	const char* showImageFiles[2] = { SHADERS_PATH"showImage.vert", SHADERS_PATH"showImage.frag" };
	showImageProgram = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, showImageFiles);

	const char* rayTracerFiles[2] = { SHADERS_PATH "rayTracer.comp" };
	rayTracerProgram = new ShaderSet(COMPUTE_SHADER_BIT, rayTracerFiles);

	rayTracerProgram->UseProgram();
	loc_from = glGetUniformLocation(rayTracerProgram->getProgramID(), "from");
	loc_xvec = glGetUniformLocation(rayTracerProgram->getProgramID(), "xvec");
	loc_yvec = glGetUniformLocation(rayTracerProgram->getProgramID(), "yvec");
	loc_zvec = glGetUniformLocation(rayTracerProgram->getProgramID(), "zvec");
	loc_lightPos = glGetUniformLocation(rayTracerProgram->getProgramID(), "lightPos");
	loc_Eye = glGetUniformLocation(rayTracerProgram->getProgramID(), "eye");
	loc_FOV = glGetUniformLocation(rayTracerProgram->getProgramID(), "fov");
	loc_image_size = glGetUniformLocation(rayTracerProgram->getProgramID(), "image_size");
	loc_image = glGetUniformLocation(rayTracerProgram->getProgramID(), "image");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::cout << "OpenGL-Version: " << glGetString(GL_VERSION) << std::endl;
	scene = new Scene(rayTracerProgram->getProgramID());
	quad = new ScreenQuad();
	image = new Texture(WIDTH, HEIGHT, 4);
	GLint data = 0;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &data);
	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE: " << data;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &data);
	std::cout << " * " << data;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &data);
	std::cout << " * " << data << std::endl;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &data);
	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_COUNT: " << data;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &data);
	std::cout << " * " << data;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &data);
	std::cout << " * " << data << std::endl;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &data);
	std::cout << "GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: " << data << std::endl;
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &data);
	std::cout << "GL_MAX_COMPUTE_SHARED_MEMORY_SIZE: " << data / 1024 << "KB" << std::endl;
}

void rayTraceImage()
{
	rayTracerProgram->UseProgram();
	glm::vec3 from, xvec, yvec, zvec;
	cam->getView(&xvec, &yvec, &zvec, &from);
	glm::vec3 eye = cam->getEye();
	glUniform3f(loc_from, from.x, from.y, from.z);
	glUniform3f(loc_xvec, xvec.x, xvec.y, xvec.z);
	glUniform3f(loc_yvec, yvec.x, yvec.y, yvec.z);
	glUniform3f(loc_zvec, zvec.x, zvec.y, zvec.z);
	glUniform4f(loc_lightPos, light.x, light.y, light.z, light.w);
	glUniform3f(loc_Eye, eye.x, eye.y, eye.z);
	glUniform1f(loc_FOV, cam->getFov());
	scene->render();
	glUniform2i(loc_image_size, image->getWidth(), image->getHeight());
	glBindImageTexture(0, image->getTextureID(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	glUniform1i(loc_image, 0);
	glDispatchCompute(image->getWidth() / 32 + 1, image->getHeight() / 32 + 1, 1);
}

void showImage() {
	showImageProgram->UseProgram();
	glActiveTexture(GL_TEXTURE0);
	image->bind2D();
	quad->render();
	image->unbind2D();
}

int main(void)
{
	initWindow();
	initOpenGL();

	scene->addSphere(Sphere(glm::vec3(-9.0f, 1.0f, 9.0f), 1.0f, 0));
	scene->addSphere(Sphere(glm::vec3(4.0f, 0.5f, 3.0f), 0.5f,glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), 0));
	scene->addTriangle(Triangle(glm::vec3(-4.0f, 4.0f, 1.0f), glm::vec3(-4.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, -4.0f), 3));
	scene->addTriangle(Triangle(glm::vec3(1.0f, 4.0f, -4.0f), glm::vec3(-4.0f, 4.0f, 1.0f), glm::vec3(1.0f, 0.0f, -4.0f), 3));
	scene->addTriangle(Triangle(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(2.0f, 0.0f, 3.0f), glm::vec3(1.0f, 2.0f, 4.0f), 5));
	scene->addBox(Box(glm::vec3(-10.0f, -1.0f, -10.0f), glm::vec3(10.0f, 0.0f, 10.0f), 1));
	scene->addBox(Box(glm::vec3(2.5f, 0.0f, -1.0f), glm::vec3(3.5f, 1.0f, 1.0f), 4));
	scene->addTexture("billiard15.png");
	scene->addTexture("checker.png");
	Material Pool = Material(0, glm::vec3(1.0f), 50.0f);
	Material Checker = Material(1, glm::vec3(1.0f), 50.0f);
	Material DarkRed = Material(glm::vec4(0.3f, 0.0f, 0.0f, 1.0f), glm::vec3(1.0f), 50.0f);
	Material Black = Material(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec3(1.0f), 50.0f);
	Material Blue = Material(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(1.0f), 50.0f);
	Material Green = Material(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(1.0f), 50.0f);
	scene->addMaterial(Pool);
	scene->addMaterial(Checker);
	scene->addMaterial(DarkRed);
	scene->addMaterial(Black);
	scene->addMaterial(Blue);
	scene->addMaterial(Green);

	float degrees = 0.0f;
	float distance = 0.0f;
	float distancePerSecond = 3.0f;
	float allDistance = 0.0f;
	int clockwise = 1;
	float pi = 3.141592f;
	glm::vec3 movedirection = glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f));
	glm::vec3 rotateaxis = glm::cross(movedirection, glm::vec3(0.0f, 1.0f, 0.0f));

	double starttime = glfwGetTime();
	double time = glfwGetTime();
	double deltaTime;

	glfwSwapInterval(0);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();

		distance = deltaTime * distancePerSecond;
		degrees += deltaTime * 360.0f / (pi * 2 * 1.0f) * distancePerSecond;
		allDistance += distance;

		if (degrees > 360.0f)
			degrees -= 360.0f;

		if (allDistance >= 25.4f) {
			allDistance -= 25.4f;
			movedirection *= -1.0f;
			clockwise *= -1;
			std::cout << glfwGetTime() - starttime << std::endl;
		}

		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(degrees * clockwise), rotateaxis);
		glm::vec3 translation = distance * movedirection;
		glm::mat4 modelMatrix;
		modelMatrix[0] = rotation[0];
		modelMatrix[1] = rotation[1];
		modelMatrix[2] = rotation[2];
		modelMatrix[0][3] = translation.x;
		modelMatrix[1][3] = translation.y;
		modelMatrix[2][3] = translation.z;
		modelMatrix[3][3] = 1.0f;


		scene->updateSphere(0, modelMatrix);

		rayTraceImage();
		showImage();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}