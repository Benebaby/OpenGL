#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <iostream>

#include "../Tools/ShaderSet.h"
#include "ScreenQuad.h"
#include "Camera.h"
#include "Image.h"
#include "Ray.h"
#include "Intersection.h"
#include "Sphere.h"
#include "Object.h"
#include "Scene.h"

const int width = 1600;
const int height = 900;

Camera* cam = new Camera(width, height);
glm::vec3 light = glm::vec3(5.0f, 5.0f, 0.0f);
Image* im;
Scene scene = Scene();
GLFWwindow* window;

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cam->updateRadius(yoffset);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		cam->updateCamPos(xpos, ypos, true);
	else
		cam->updateCamPos(xpos, ypos, false);
}

glm::vec3 shade(Intersection h)
{
	glm::vec3 lightVec = glm::normalize(light - h.getHitPosition());
	glm::vec3 normal = h.m_object->getNormal(h.m_ray, h.m_distance);
	glm::vec3 reflectedLightVec = glm::reflect(-lightVec, normal);
	glm::vec3 viewVec = glm::normalize(cam->getEye() - h.getHitPosition());
	float cosphi = glm::max(glm::dot(normal, lightVec), 0.0f);
	float cospsi_n = glm::pow(glm::max(glm::dot(viewVec, reflectedLightVec), 0.0f), 50.0f);
	return  0.2f * h.m_object->getColor() + (cosphi * h.m_object->getColor() + cospsi_n);
}

glm::vec3 trace(Ray ray, unsigned int level, unsigned int maxlevel)
{
	glm::vec3 color = glm::vec3(0.0f);
	Intersection hit = Intersection(ray, FLT_MAX, nullptr);
	Ray reflectedRay;
	if (scene.closestIntersection(&hit))
	{
		color = shade(hit);
		glm::vec3 lightDir = glm::normalize(light - hit.getHitPosition());
		Ray shadowray = Ray(hit.getHitPosition() + 0.001f * lightDir, lightDir);
		Intersection shadowhit = Intersection(shadowray, FLT_MAX, nullptr);
		if (scene.anyIntersection(&shadowhit))
			color = 0.2f * hit.m_object->getColor();
		if (level == maxlevel)
			return color;
		glm::vec3 normal = hit.m_object->getNormal(hit.m_ray, hit.m_distance);
		reflectedRay = Ray(hit.getHitPosition() + 0.001f * normal, glm::reflect(hit.m_ray.m_dir, normal));
	}
	else
	{
		return color;
	}

	return color + (1 / (float)maxlevel * trace(reflectedRay, level + 1, maxlevel));
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

	constexpr int antialiasingFactor = 1;
	constexpr int antialiasing = antialiasingFactor * antialiasingFactor;
	constexpr float antialiasingStep = 1.0f / antialiasingFactor;

	for (int y = 0; y < im->m_height; y++) {
		for (int x = 0; x < im->m_width; x++) {
			glm::vec3 color = glm::vec3(0.0f);
			for (int a = 0; a < antialiasing; a++) {
				float delta_x = a / antialiasingFactor * antialiasingStep + antialiasingStep / 2.0f; // aa(1) => 0.5
				float delta_y = a % antialiasingFactor * antialiasingStep + antialiasingStep / 2.0f;
				glm::vec3 dir = (origin + (x + delta_x) * steprightvec + (y + delta_y) * stepupvec - from);
				Ray ray = Ray(from, glm::normalize(dir));
				color += trace(ray, 0, 3);
			}
			im->set(x, y, color / (float)antialiasing);
		}
	}
	im->upload();
}

int main(void)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "RayTracer", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwSetCursorPosCallback(window, &MouseCallback);
	glfwSetScrollCallback(window, &ScrollCallback);
	glfwMakeContextCurrent(window);
	glewInit();

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	std::cout << glGetString(GL_VERSION) << std::endl;

	const char* shaderfiles[2] = { SHADERS_PATH"/minimal.vert", SHADERS_PATH"/minimal.frag" };
	ShaderSet shaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, shaderfiles);

	im = new Image(1600, 900);
	ScreenQuad quad = ScreenQuad();

	Object* s0 = new Sphere(glm::vec3(0.0f, 2.0f, 0.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	Object* s1 = new Sphere(glm::vec3(0.0f, -100.0f, 0.0f), 100.f, glm::vec3(0.0f, 0.0f, 1.0f));
	Object* s2 = new Sphere(glm::vec3(2.0f, 2.0f, 2.0f), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	scene.addObject(s0);
	scene.addObject(s1);
	scene.addObject(s2);

	glm::mat4 lightmatrix = glm::mat4(1.0f);
	float angle = 0.0f;
	double startTime = glfwGetTime();
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - startTime;
		startTime = glfwGetTime();
		//angle += 0.5 * deltaTime;
		lightmatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
		light = glm::vec3(lightmatrix * glm::vec4(5.0f, 5.0f, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.UseProgram();
		rayTraceImage();
		glBindTexture(GL_TEXTURE_2D, im->getID());
		quad.render();
		std::cout << deltaTime << "\n";

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}