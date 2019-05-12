#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/ext.hpp>
#include <vector>

#include "../Tools/ShaderSet.h"
#include "Camera.h"
#include "Object.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

int width = 1600;
int height = 900;

GLFWwindow* window;
glm::mat4 projectionMatrix;
Camera cam = Camera(width, height);
bool wantChassis = true;

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cam.radius -= (float)(yoffset * 0.1);
	cam.radius = glm::max(cam.radius, 0.000001f);
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

void resizeCallback(GLFWwindow *window, int w, int h) {
	width = w;
	height = h;
	projectionMatrix = glm::perspective(3.141f * 40.0f / 180.0f, width / (float)height, 0.1f, 500.0f);
	cam.m_width = w;
	cam.m_height = h;
	glViewport(0, 0, width, height);
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
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (wantChassis)
			wantChassis = false;
		else
			wantChassis = true;
	}
}

int main(void)
{ 
	glfwInit();
	window = glfwCreateWindow(width, height, "Audi_R8_V10_Plus", NULL, NULL);
	glfwSetWindowPos(window, 150, 90);
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetCursorPosCallback(window, &MouseCallback);
	glfwSetScrollCallback(window, &ScrollCallback);
	glfwSetKeyCallback(window, &KeyCallback);
	glfwSetWindowSizeCallback(window, &resizeCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::cout << glGetString(GL_VERSION) << std::endl;

	glm::mat4 modelMatrixR1;
	glm::mat4 modelMatrixR2;
	glm::mat4 modelMatrixR3;
	glm::mat4 modelMatrixR4;
	glm::mat4 audiMatrix = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-2, -0.03, 0)), glm::radians(-90.0f), glm::vec3(0, 1, 0));
	glm::mat4 cubiMatrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 2.0f, -2.0f)), glm::vec3(0.01));
	glm::mat4 myriamMatrix = glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, 0.0f, -1.8f)), glm::vec3(1.13)), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rolfMatrix = glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-3.8f, 0.0f, -1.8f)), glm::vec3(1.13)), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 1000.0f);
	glm::vec4 light = glm::vec4(-5.0f, 8.0f, -10.0f, 0.0f);

	glm::mat4 lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 30.0f);
	glm::mat4 lightViewMatrix = glm::lookAt(glm::vec3(light.x, light.y, light.z), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	const char *principledshaderfiles[2] = { SHADERS_PATH"principled.vert", SHADERS_PATH"principled.frag" };
	ShaderSet principledShaderProgram(VERTEX_SHADER_BIT | FRAGMENT_SHADER_BIT, principledshaderfiles);
	GLuint projectionMatrixID = glGetUniformLocation(principledShaderProgram.getProgramID(), "projectionMatrix");
	GLuint viewMatrixID = glGetUniformLocation(principledShaderProgram.getProgramID(), "viewMatrix");
	GLuint lightProjectionMatrixID = glGetUniformLocation(principledShaderProgram.getProgramID(), "lightProjectionMatrix");
	GLuint lightViewMatrixID = glGetUniformLocation(principledShaderProgram.getProgramID(), "lightViewMatrix");
	GLuint lightID = glGetUniformLocation(principledShaderProgram.getProgramID(), "light_pos");
	GLuint shaderID = principledShaderProgram.getProgramID();

	Mesh r8_chassis_new = Mesh("r8_chassis_new.obj");
	Mesh r8_felge = Mesh("felge.obj");
	Mesh r8_reifen = Mesh("reifen.obj");
	Mesh ground = Mesh("plane.obj");
	Mesh würfel = Mesh("cube.obj");
	Mesh woman = Mesh("myriam.obj");

	Texture reifenBasemap("tire/albedo.png");
	Texture reifenRoughmap("tire/rough.png");
	Texture reifenNormalmap("tire/normal.png");
	Texture cobblestoneBasemap("cobblestone/albedo.png");
	Texture cobblestoneRoughmap("cobblestone/rough.png");
	Texture cobblestoneNormalmap("cobblestone/normal.png");
	Texture myriamBasemap("myriam/albedo.jpg");
	Texture myriamNormalmap("myriam/normal.jpg");
	Texture myriamRoughmap("myriam/rough.jpg");
	Texture reflectionMap("reflection/rightC.jpg", "reflection/leftC.jpg", "reflection/topC.jpg", "reflection/bottomC.jpg", "reflection/frontC.jpg", "reflection/backC.jpg");
	Texture skybox("skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg");

	//1=Phong 2=TexPhong 3=PBR 4=Normalmap 5=Reflective 6=skybox
	Material red = Material(1, shaderID, glm::vec4(0.6f, 0.0f, 0.0f, 1.0f), glm::vec3(1.0f), glm::vec3(0.3), 20.0f);
	Material clothes = Material(3, shaderID, myriamBasemap, myriamRoughmap, myriamNormalmap, glm::vec3(0.3), 20.0f);
	Material cobble = Material(3, shaderID, cobblestoneBasemap, cobblestoneRoughmap, cobblestoneNormalmap, glm::vec3(0.3), 20.0f);
	Material profil = Material(3, shaderID, reifenBasemap, reifenRoughmap, reifenNormalmap, glm::vec3(0.3), 20.0f);
	Material sky = Material(6, shaderID, skybox);
	Material metalblack = Material(5, shaderID, reflectionMap, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(1.0f), 1, glm::vec3(0.3f), 100.0f);
	Material metalcyan = Material(5, shaderID, reflectionMap, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(1.0f), 1, glm::vec3(0.3f), 100.0f);
	Material nomaterial = Material(0, shaderID);
	
	Object felge = Object(r8_felge, shaderID, metalcyan);
	Object audi = Object(r8_chassis_new, shaderID, metalblack);
	Object reifen = Object(r8_reifen, shaderID, profil);
	Object boden = Object(ground, shaderID, cobble);
	Object box = Object(würfel, shaderID, sky);
	Object myriam = Object(woman, shaderID, clothes);

	Object shaderFelge = Object(r8_felge, shaderID, nomaterial);
	Object shaderAudi = Object(r8_chassis_new, shaderID, nomaterial);
	Object shaderReifen = Object(r8_reifen, shaderID, nomaterial);
	Object shaderBoden = Object(ground, shaderID, nomaterial);
	Object shaderMyriam = Object(woman, shaderID, nomaterial);

	Texture depthMap(4096, 4096);
	GLuint tex5ID = glGetUniformLocation(shaderID, "depthMap");

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.getTexture(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float angle = 0.0f;
	float rpm = 10.0f;
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(0);
	while (!glfwWindowShouldClose(window))
	{
		angle = (float)glfwGetTime() * 3.141592f * rpm;
		cam.update();
		modelMatrixR1 = glm::scale(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-1.51f, 0.4f, 0.95f)), glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)), glm::radians(angle * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.1f, 1.0f, 1.0f));
		modelMatrixR2 = glm::scale(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-1.51f, 0.4f, -0.95f)), glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)), glm::radians(-angle * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.1f, 1.0f, 1.0f));
		modelMatrixR3 = glm::scale(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(1.63f, 0.4f, 0.95f)), glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)), glm::radians(angle * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.2f, 1.0f, 1.0f));
		modelMatrixR4 = glm::scale(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(1.63f, 0.4f, -0.95f)), glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0)), glm::radians(-angle * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.2f, 1.0f, 1.0f));

		principledShaderProgram.UseProgram();
		glUniformMatrix4fv(lightViewMatrixID, 1, false, glm::value_ptr(lightViewMatrix));
		glUniformMatrix4fv(lightProjectionMatrixID, 1, false, glm::value_ptr(lightProjectionMatrix));
		glViewport(0, 0, 4096, 4096);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		shaderFelge.setModelMatrix(modelMatrixR1);
		shaderFelge.render();
		shaderFelge.setModelMatrix(modelMatrixR2);
		shaderFelge.render();
		shaderFelge.setModelMatrix(modelMatrixR3);
		shaderFelge.render();
		shaderFelge.setModelMatrix(modelMatrixR4);
		shaderFelge.render();
		shaderReifen.setModelMatrix(modelMatrixR1);
		shaderReifen.render();
		shaderReifen.setModelMatrix(modelMatrixR2);
		shaderReifen.render();
		shaderReifen.setModelMatrix(modelMatrixR3);
		shaderReifen.render();
		shaderReifen.setModelMatrix(modelMatrixR4);
		shaderReifen.render();
		shaderBoden.render();
		shaderMyriam.setModelMatrix(myriamMatrix);
		shaderMyriam.render();

		if (wantChassis) {
			shaderAudi.setModelMatrix(audiMatrix);
			shaderAudi.render();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		principledShaderProgram.UseProgram();
		glViewport(0, 0, width, height);
		glUniform4fv(lightID, 1, glm::value_ptr(light));
		glUniformMatrix4fv(viewMatrixID, 1, false, glm::value_ptr(cam.getViewMatrix()));
		glUniformMatrix4fv(projectionMatrixID, 1, false, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(lightViewMatrixID, 1, false, glm::value_ptr(lightViewMatrix));
		glUniformMatrix4fv(lightProjectionMatrixID, 1, false, glm::value_ptr(lightProjectionMatrix));
		glUniform1i(tex5ID, 4);
		glActiveTexture(GL_TEXTURE4);
		depthMap.bind2D();

		felge.setModelMatrix(modelMatrixR1);
		felge.render();
		felge.setModelMatrix(modelMatrixR2);
		felge.render();
		felge.setModelMatrix(modelMatrixR3);
		felge.render();
		felge.setModelMatrix(modelMatrixR4);
		felge.render();
		reifen.setModelMatrix(modelMatrixR1);
		reifen.render();
		reifen.setModelMatrix(modelMatrixR2);
		reifen.render();
		reifen.setModelMatrix(modelMatrixR3);
		reifen.render();
		reifen.setModelMatrix(modelMatrixR4);
		reifen.render();
		boden.render();
		box.render();
		myriam.setModelMatrix(myriamMatrix);
		myriam.render();
		if (wantChassis) {
			audi.setModelMatrix(audiMatrix);
			audi.render();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}