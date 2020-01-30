#define STB_IMAGE_IMPLEMENTATION


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtx\string_cast.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Texture.h"

const float toRadians = glm::pi<float>() / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture girafeTexture;
Texture leopardTexture;

GLfloat deltatime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x	   y	  z		 u	   v
		-1.0f, -1.2f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
		1.0f, -1.3f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	Input input = Input(mainWindow.GetGLFWWindow());

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f, &input);

	girafeTexture = Texture((char*)"Textures/girafe.jpg");
	girafeTexture.LoadTexture();
	leopardTexture = Texture((char*)"Textures/leopard.jpg");
	leopardTexture.LoadTexture();

	printf(glm::to_string(camera.GetViewMatrix()).c_str());
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(),
											0.1f,
											100.0f);

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

	// Loop until window closed
	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltatime = now - lastTime;
		lastTime = now;

		// Get user input events
		glfwPollEvents();
		camera.Update(deltatime);
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.5, 0.45f, -3.0f));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		girafeTexture.UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5, -0.45f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		leopardTexture.UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}