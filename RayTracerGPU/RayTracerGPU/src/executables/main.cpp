#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "../Tools/ShaderSet.h"
#include "Texture.h"
#include "Camera.h"
#include "ScreenQuad.h"
#include "Sphere.h"

#define WIDTH 1600
#define HEIGHT 900
#define FOV 60.

GLFWwindow * window;
Camera* cam = new Camera(WIDTH, HEIGHT, FOV);
ShaderSet* rayTracerProgram;
ShaderSet* showImageProgram;
ScreenQuad* quad;
Texture* image;
GLuint loc_from, loc_xvec, loc_yvec, loc_zvec, loc_lightPos, loc_image_size, loc_image, ssbo;
glm::vec4 light = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f);
std::vector<Sphere> scene;

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
	const char* showImageFiles[2] = { SHADERS_PATH"/showImage.vert", SHADERS_PATH"/showImage.frag" };
	showImageProgram = new ShaderSet(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, showImageFiles);

	const char* rayTracerFiles[2] = { SHADERS_PATH "/rayTracer.comp" };
	rayTracerProgram = new ShaderSet(COMPUTE_SHADER_BIT, rayTracerFiles);

	rayTracerProgram->UseProgram();
	loc_from = glGetUniformLocation(rayTracerProgram->getProgramID(), "from");
	loc_xvec = glGetUniformLocation(rayTracerProgram->getProgramID(), "xvec");
	loc_yvec = glGetUniformLocation(rayTracerProgram->getProgramID(), "yvec");
	loc_zvec = glGetUniformLocation(rayTracerProgram->getProgramID(), "zvec");
	loc_lightPos = glGetUniformLocation(rayTracerProgram->getProgramID(), "lightPos");
	loc_image_size = glGetUniformLocation(rayTracerProgram->getProgramID(), "image_size");
	loc_image = glGetUniformLocation(rayTracerProgram->getProgramID(), "image");
	glGenBuffers(1, &ssbo);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::cout << "OpenGL-Version: " << glGetString(GL_VERSION) << std::endl;
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
	std::cout << "GL_MAX_COMPUTE_SHARED_MEMORY_SIZE: " << data/1024 <<"KB"<< std::endl;
}

void rayTraceImage()
{
	rayTracerProgram->UseProgram();
	glm::vec3 from, xvec, yvec, zvec;
	cam->getView(&xvec, &yvec, &zvec, &from);
	glUniform3f(loc_from, from.x, from.y, from.z);
	glUniform3f(loc_xvec, xvec.x, xvec.y, xvec.z);
	glUniform3f(loc_yvec, yvec.x, yvec.y, yvec.z);
	glUniform3f(loc_zvec, zvec.x, zvec.y, zvec.z);
	glUniform4f(loc_lightPos, light.x, light.y, light.z, light.w);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, scene.size() * sizeof(Sphere), scene.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo);

	glUniform2i(loc_image_size, image->getWidth(), image->getHeight());
	glBindImageTexture(0, image->getTextureID(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	glUniform1i(loc_image, 0);

	glDispatchCompute(image->getWidth() / 32, image->getHeight() / 32, 1);
}

void showImage() {
	showImageProgram->UseProgram();
	image->bind2D();
	quad->render();
}

int main(void)
{
	initWindow();
	initOpenGL();

	quad = new ScreenQuad();
	image = new Texture(WIDTH, HEIGHT, 4);

	Sphere s0 = Sphere(glm::vec3(0.0f, -101.0f, 0.0f), 100.f);
	Sphere s1 = Sphere(glm::vec3(-0.5f, 0.0f, -0.5f), 1.0f);
	Sphere s2 = Sphere(glm::vec3(0.5f, -.75f, 0.5f), 0.25f);
	Sphere s3 = Sphere(glm::vec3(0.5f, -0.25f, 0.5f), 0.25f);
	Sphere s4 = Sphere(glm::vec3(0.5f, 0.25f, 0.5f), 0.25f);
	Sphere s5 = Sphere(glm::vec3(0.5f, 0.75f, 0.5f), 0.25f);
	scene.push_back(s0);
	scene.push_back(s1);
	scene.push_back(s2);
	scene.push_back(s3);
	scene.push_back(s4);
	scene.push_back(s5);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		rayTraceImage();
		showImage();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}