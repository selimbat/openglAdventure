#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\mat4x4.hpp>

// Window dimentions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = glm::pi<float>() / 180.0f;

GLuint VAO, VBO, IBO, shaderProgram, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 3.0f;
float triIncrement = 0.005f;


// Vertex shader
static const char* vShader = "									\n\
#version 330     												\n\
layout (location = 0) in vec3 pos;								\n\
out vec4 vCol;													\n\
uniform mat4 model;												\n\
void main()														\n\
{																\n\
	gl_Position = model * vec4(0.4 * pos.xy, pos.z , 1.0);		\n\
	vCol = vec4(clamp(pos, 0.0, 1.0), 1.0);						\n\
}";

// Fragment shader
static const char* fShader = "									\n\
#version 330     												\n\
in vec4 vCol;													\n\
out vec4 color;													\n\
void main()														\n\
{																\n\
	color = vCol;												\n\
}";


void CreateTriangle()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("ERROR: Compiling the %d shader failed! '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(program, shader);

}

void CompileShaders()
{
	shaderProgram = glCreateProgram();

	if (!shaderProgram)
	{
		printf("ERROR: Creating shader program failed!\n");
		return;
	}

	AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
	AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("ERROR: Linking shader program failed! '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("ERROR: Validating program failed! '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderProgram, "model");
}

int main()
{
	// Initialize GLFW
	if (!glfwInit()) 
	{
		printf("ERROR: GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}
	
	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Setting profile to core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("ERROR: GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set Context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("ERROR: GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get user input events
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glm::mat4 model(1.0f);
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::rotate(model, triOffset, glm::vec3(1.0, 1.0, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}