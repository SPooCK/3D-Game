#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// SOIL2
#include "SOIL2/SOIL2.h"
// Ohter includes
#include "Shader.h"

// Window demensions
const GLint WIDTH = 800, HEIGHT = 600;

// The MIAN function, from here we start the application and run the game loop
int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "3D Game", nullptr, nullptr);

	int screenWidht, screenHeight;
	glfwGetFramebufferSize(window, &screenWidht, &screenHeight);

	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidht, screenHeight);

	Shader ourShader("resources/shaders/core.vs", "resources/shaders/core-frag.vs");

	GLfloat vertices[] = {
		// Position				// Color
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,  // bottom left
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f, // bottom right
		0.0f, 0.5, 0.0f,		0.0f, 0.0f, 1.0f // middle top
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//Game Loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activated (key pressed, mouse move etc.)
		glfwPollEvents();

		// Render and CLear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render OpenGL
		ourShader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Sawp the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLWF, clearing any resources allocated by GLFW
	glfwTerminate();
	return EXIT_SUCCESS;
}