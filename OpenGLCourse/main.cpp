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
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "main.h"

const float toRadians = glm::pi<float>() / 180.0f;

Window mainWindow;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Mesh* floorMesh = new Mesh();
Model falconMillenium = Model();
Model bunny = Model();
Model dragon = Model();
Model utahTeapot = Model();

Texture girafeTexture;
Texture leopardTexture;
Texture placeholderTexture;
Texture grayTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLuint uniformModel = 0,
	   uniformProjection = 0,
	   uniformView = 0,
	   uniformEyePosition = 0,
	   uniformSpecularIntensity = 0,
	   uniformShininess = 0,
	   uniformOmniLightPos = 0,
	   uniformFarPlane = 0;


GLfloat deltatime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
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

	floorMesh->CreateMesh(floorVertices, floorIndices, 32, 6);
}

void AddLights()
{
	mainLight = DirectionalLight(glm::vec3(1.0f), 0.2f,
							     0.5f, glm::vec3(-2.0f, -1.0f, -1.0f), 2048, 2048);

	pointLights[0] = PointLight(glm::vec3(1.0f, 0.0f, 0.0f),
								0.8f, 0.8f,
								glm::vec3(-2.0f, 0.0f, 0.0f),
								LightAttenuationModel(1.0f, 0.1f, 1.0f),
								1024, 1024, 0.01f, 100.0f);
	pointLightCount++;
	pointLights[1] = PointLight(glm::vec3(0.0f, 1.0f, 0.0f),
								0.8f, 0.8f,
								glm::vec3(1.0f, 0.0f, -2.0f),
								LightAttenuationModel(1.0f, 0.1f, 1.0f),
								1024, 1024, 0.01f, 100.0f);
	pointLightCount++;
	pointLights[2] = PointLight(glm::vec3(0.0f, 0.0f, 1.0f),
								0.8f, 0.8f,
								glm::vec3(0.0f, 0.0f, 1.0f),
								LightAttenuationModel(1.0f, 0.1f, 1.0f),
								1024, 1024, 0.01f, 100.0f);
	pointLightCount++;

	spotLights[0] = SpotLight(glm::vec3(1.0f),
							  0.6f, 0.8f,
							  glm::vec3(1.0f, 0.0f, 1.0f),
							  glm::vec3(-1.0f, -1.0f, -1.0f),
							  20.0f,
							  LightAttenuationModel(1.0f, 0.1f, 1.0f),
							  1024, 1024, 0.01f, 100.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(glm::vec3(0.7f, 1.0f, 1.0f),
							  0.2f, 0.8f,
							  glm::vec3(0.0f, -0.6f, 0.5f),
							  glm::vec3(1.0f, -1.0f, 0.0f),
							  45.0f,
							  LightAttenuationModel(0.1f, 0.1f, 0.02f),
							  1024, 1024, 0.01f, 100.0f);
	spotLightCount++;
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFile("Shaders/directional_shadow_map.vert",
										   "Shaders/directional_shadow_map.frag");
	
	omniShadowShader = Shader();
	omniShadowShader.CreateFromFile("Shaders/omni_shadow_map.vert",
									"Shaders/omni_shadow_map.frag",
									"Shaders/omni_shadow_map.geom");
									
}

void RenderScene()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.5f));
	model = glm::scale(model, glm::vec3(0.05f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	utahTeapot.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.005f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	utahTeapot.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	placeholderTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	floorMesh->RenderMesh();
	/*
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	grayTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	dragon.RenderModel();
	*/
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.3f, -2.1f, -1.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	grayTexture.UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	bunny.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();
	ShadowMap* shadowMap = light->GetShadowMap();
	glViewport(0, 0, shadowMap->GetShadowWidth(), shadowMap->GetShadowHeight());
	shadowMap->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetPointLightPositionLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3fv(uniformOmniLightPos, 1, glm::value_ptr(light->GetPosition()));
	glUniform1f(uniformFarPlane, light->GetFarPlane());

	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glViewport(0, 0, 1366, 768);

	// Clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3fv(uniformEyePosition, 1, glm::value_ptr(camera.GetPosition()));

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	shaderList[0].Validate();

	RenderScene();
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();
	Input input = Input(mainWindow.GetGLFWWindow());

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f, &input);

	//falconMillenium.LoadModel("Models/millenium-falcon.obj");
	//dragon.LoadModel("Models/dragon.obj");
	bunny.LoadModel("Models/bunny.obj");
	utahTeapot.LoadModel("Models/utah-teapot.obj");

	girafeTexture = Texture((char*)"Textures/girafe.jpg");
	girafeTexture.LoadTexture();
	leopardTexture = Texture((char*)"Textures/leopard.jpg");
	leopardTexture.LoadTexture();
	placeholderTexture = Texture((char*)"Textures/white_square.png");
	placeholderTexture.LoadTexture();
	grayTexture = Texture((char*)"Textures/gray.jpg");
	grayTexture.LoadTexture();

	shinyMaterial = Material(0.5f, 32);
	dullMaterial = Material(0.3f, 1);

	AddLights();
	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
											mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(),
											0.1f,
											100.0f);


	// Loop until window closed
	while (!mainWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltatime = now - lastTime;
		lastTime = now;

		// Get user input events
		glfwPollEvents();
		camera.Update(deltatime);
		spotLights[0].SetAt(3.0f * glm::vec3(sin(now), 0.0f, cos(now)), glm::vec3(-sin(now), -0.5f, -cos(now)));

		DirectionalShadowMapPass(&mainLight);
		
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}
		
		RenderPass(projection, camera.GetViewMatrix());


		glUseProgram(0);

		mainWindow.SwapBuffers();
	}
	return 0;
}
