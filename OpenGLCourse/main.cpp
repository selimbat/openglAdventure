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

#include "CommonValues.h"
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Input.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

const float toRadians = glm::pi<float>() / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture girafeTexture;
Texture leopardTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];

GLfloat deltatime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";

void ComputeAverageNormals(unsigned int* indices,
						   unsigned int indicesCount,
						   GLfloat* vertices,
						   unsigned int verticesCount,
						   unsigned int vLength,
						   unsigned int normalOffset)
{
	for (size_t i = 0; i < indicesCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1 = glm::vec3(vertices[in1	 ] - vertices[in0	 ],
								 vertices[in1 + 1] - vertices[in0 + 1],
								 vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2 = glm::vec3(vertices[in2	 ] - vertices[in0	 ],
								 vertices[in2 + 1] - vertices[in0 + 1],
								 vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		unsigned nOffset = i * vLength + normalOffset;
		glm::vec3 normal = glm::vec3(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		normal = glm::normalize(normal);
		vertices[nOffset] = normal.x; vertices[nOffset + 1] = normal.y; vertices[nOffset + 2] = normal.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x	   y	  z		   u	   v	 n_x   n_y   n_z
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f,	 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 2.0f,    0.5f, 0.0f,	 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,    1.0f, 0.0f,	 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,     0.5f, 1.0f,	 0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[]{
		//	x	   y	  z		u	   v	  n_x   n_y   n_z
		-20.0f, 0.0f, -20.0f, -20.0f, -20.0f, 0.0f, -1.0f, 0.0f,
		20.0f, 0.0f, -20.0f,  20.0f, -20.0f,  0.0f, -1.0f, 0.0f,
		-20.0f, 0.0f, 20.0f,  -20.0f, 20.0f,  0.0f, -1.0f, 0.0f,
		20.0f, 0.0f, 20.0f,   -20.0f, -20.0f, 0.0f, -1.0f, 0.0f
	};

	ComputeAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* floor = new Mesh();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(floor);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();
	Input input = Input(mainWindow.GetGLFWWindow());

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f, &input);

	girafeTexture = Texture((char*)"Textures/girafe.jpg");
	girafeTexture.LoadTexture();
	leopardTexture = Texture((char*)"Textures/leopard.jpg");
	leopardTexture.LoadTexture();

	shinyMaterial = Material(4.0f, 64);
	dullMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(glm::vec3(0.0f), 0.3f,
								 0.8f, glm::vec3(-2.0f, -1.0f, -1.0f));

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(glm::vec3(1.0f, 1.0f, 1.0f),
								0.2f, 1.0f,
								glm::vec3(-4.0f, 0.0f, -6.0f),
								LightAttenuationModel(0.3f, 0.1f, 0.02f));
	pointLightCount++;
	pointLights[1] = PointLight(glm::vec3(1.0f, 1.0f, 1.0f),
								0.4f, 0.8f,
								glm::vec3(4.0f, -1.0f, -3.0f),
								LightAttenuationModel(0.3f, 0.1f, 0.02f));
	pointLightCount++;


	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(),
											0.1f,
											100.0f);

	GLuint uniformModel = 0,
		   uniformProjection = 0,
		   uniformView = 0,
		   uniformEyePosition = 0,
		   uniformSpecularIntensity = 0,
		   uniformShininess = 0;

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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		glUniform3fv(uniformEyePosition, 1, glm::value_ptr(camera.GetPosition()));

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.9f, -7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		girafeTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.9f, -7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		leopardTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		girafeTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		glUseProgram(0);

		mainWindow.SwapBuffers();
	}

	return 0;
}