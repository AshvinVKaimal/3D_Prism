#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <random>
#include "include/glm/glm/glm.hpp"
#include "include/glm/glm/gtc/matrix_transform.hpp"
#include "include/glm/glm/gtc/type_ptr.hpp"

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

// Vertex shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 color;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	color = aColor;\n"
"}\0";

// Fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 color;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(color, 1.0f);\n"
"}\0";

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);		// camera position
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);	// camera front vector
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);		// camera up vector

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f; // time of last frame

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

glm::vec3 objectCenter = glm::vec3(0.0f, 0.0f, 0.5f);	// coordinates of object center

bool isPrism = true;				// flag to switch between prism and pyramid

void processInput(GLFWwindow* window)
{
	// close the window when the escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = 2.5f * deltaTime;
	const float objSpeed = 5.f * deltaTime;

	glm::vec3 dir = glm::normalize(objectCenter - cameraPos);				// camera direction
	glm::vec3 cameraRight = glm::normalize(glm::cross(dir, cameraUp));		// camera right vector

    // move the camera forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * dir;
    // move the camera backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * dir;
    // move the camera left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraRight * cameraSpeed;
    // move the camera right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraRight * cameraSpeed;
    // move the camera up
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += glm::cross(cameraRight, dir) * cameraSpeed;
    // move the camera down
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= glm::cross(cameraRight, dir) * cameraSpeed;

	// move the object forward
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		model = glm::translate(model, objSpeed * glm::vec3(0.0f, 0.0f, 0.1f));
	// move the object backward
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		model = glm::translate(model, objSpeed * glm::vec3(0.0f, 0.0f, -0.1f));
	// move the object left
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		model = glm::translate(model, objSpeed * glm::vec3(-0.1f, 0.0f, 0.0f));
	// move the object right
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		model = glm::translate(model, objSpeed * glm::vec3(0.1f, 0.0f, 0.0f));
	// move the object up
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		model = glm::translate(model, objSpeed * glm::vec3(0.0f, 0.1f, 0.0f));
	// move the object down
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		model = glm::translate(model, objSpeed * glm::vec3(0.0f, -0.1f, 0.0f));

	// rotate object around the z-axis
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		model = glm::rotate(model, glm::radians(0.5f), glm::vec3(0.0f, 0.0f, 1.0f));

	static bool keyReleased = true;
	
	// switch between prism and pyramid
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && keyReleased)
	{
		isPrism = !isPrism;
		keyReleased = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)
	{
		keyReleased = true;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// set the viewport to match the new window dimensions
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <number_of_vertices>" << std::endl;
		return -1;
	}

	int n = atoi(argv[1]);
	if (n < 3)
	{
		std::cout << "Number of vertices must be at least 3" << std::endl;
		return -1;
	}

	// initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// create window object
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// make the window's context the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load all OpenGL function pointers with GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// enable depth testing
	glEnable(GL_DEPTH_TEST);


	
	// vertices coordinates
	GLfloat* vertices = new GLfloat[12 * n];
	for (int i = 0; i < n; i++)
	{
		// base vertices
		vertices[6 * i] = cos(2 * M_PI * i / n) / 2;
		vertices[6 * i + 1] = sin(2 * M_PI * i / n) / 2;
		vertices[6 * i + 2] = 0.0f;
		// base colors
		vertices[6 * i + 3] = ((float) rand() / (RAND_MAX));
		vertices[6 * i + 4] = ((float) rand() / (RAND_MAX));
		vertices[6 * i + 5] = ((float) rand() / (RAND_MAX));

		// top vertices
		vertices[6 * (n + i)] = cos(2 * M_PI * i / n) / 2;
		vertices[6 * (n + i) + 1] = sin(2 * M_PI * i / n) / 2;
		vertices[6 * (n + i) + 2] = 1.0f;
		// top colors
		vertices[6 * (n + i) + 3] = vertices[6 * i + 3];
		vertices[6 * (n + i) + 4] = vertices[6 * i + 4];
		vertices[6 * (n + i) + 5] = vertices[6 * i + 5];
	}

	// indices for the vertices
	GLuint* indices = new GLuint[6 * n];
	for (int i = 0; i < n; i++)
	{
		indices[6 * i] = i;
		indices[6 * i + 1] = (i + 1) % n;
		indices[6 * i + 2] = n + i;

		indices[6 * i + 3] = n + i;
		indices[6 * i + 4] = (i + 1) % n;
		indices[6 * i + 5] = n + (i + 1) % n;
	}

	// create vertex shader object and compile the shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// create fragment shader object and compile the shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// create shader program and link the shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// create reference containers for the vertex array object, vertex buffer object and element buffer object
	GLuint VAO, VBO, EBO;

	// generate the vertex array object, vertex buffer object and element buffer object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind the vertex array object, vertex buffer object and element buffer object
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * n * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * n * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	// GL_STREAM_DRAW - data is set once and used by the GPU at most a few times
	// GL_STATIC_DRAW - data is set once and used many times
	// GL_DYNAMIC_DRAW - data is changed a lot and used many times

	// configure the vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// safety measure to avoid accidentally modifying the objects
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currFrame = static_cast<float>(glfwGetTime());
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		// input
		processInput(window);

	// rendering commands here

		// clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// transform the camera
		view = glm::lookAt(cameraPos, objectCenter, cameraUp);

		if (!isPrism)
		{
			for (int i = 0; i < n; i++)
			{
				vertices[6 * (n + i)] = 0.0f;
				vertices[6 * (n + i) + 1] = 0.0f;
			}
			glBufferData(GL_ARRAY_BUFFER, 12 * n * sizeof(float), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * n * sizeof(unsigned int), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}
		else
		{
			for (int i = 0; i < n; i++)
			{
				vertices[6 * (n + i)] = cos(2 * M_PI * i / n) / 2;
				vertices[6 * (n + i) + 1] = sin(2 * M_PI * i / n) / 2;
			}
			glBufferData(GL_ARRAY_BUFFER, 12 * n * sizeof(float), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * n * sizeof(unsigned int), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		// use the shader program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// draw the object
		glDrawArrays(GL_TRIANGLE_FAN, 0, n);
		glDrawArrays(GL_TRIANGLE_FAN, n, n);
		glDrawElements(GL_TRIANGLES, 6 * n, GL_UNSIGNED_INT, 0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// delete all created objects
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// clear all GLFW resources
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}