#include <iostream>
#include <cmath>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// SOIL2
#include "SOIL2/SOIL2.h"
// GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
// irrKlang
#include <irrKlang.h>
// Ohter includes
#include <list> // Using list arrays
#include <string.h>
#include "Shader.h"
#include "Camera.h"
GLfloat RandomFloat(GLfloat a, GLfloat b);
#include "Box.h"


using namespace glm;
using namespace irrklang;
using namespace std;

// Window demensions
const GLint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
bool CollisionCheck(vec3 A, vec3 B, GLfloat radius);
vec3 ReverseMovement(vec3 Pos);
vec3 CubeMovement(vec3 Pos);
void BindMapsDiSpe(GLuint diffuseMap, GLuint specularMap, const char* PathD, const char* PathS);
void CancelMove();

// Camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;

// Light attributes
vec3 lightPos(1.2f, 1.0f, -2.0f);
bool drawDir = true;
bool drawPoint = true;
bool drawSpot = false;

// Deltatime
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

// The MIAN function, from here we start the application and run the game loop
int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Setup Full Screen mode
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "3D Game", monitor, nullptr);
	//GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "3D Game", nullptr, nullptr);

	if (nullptr == window) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit()) {
		cout << "Failed to initialise GLEW" << endl;
		
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Sound Engine
	ISoundEngine* engine = createIrrKlangDevice();
	ISound* music = engine->play2D("res/media/getout.ogg", true);

	// Build and compile our shader program
	Shader lightingShader("res/shaders/lighting.vs", "res/shaders/lighting.frag");
	Shader lampShader("res/shaders/lamp.vs", "res/shaders/lamp.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions            // Normals              // Texture Coords
		-0.25f, -0.25f, -0.25f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.25f, -0.25f, -0.25f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.25f,  0.25f, -0.25f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.25f,  0.25f, -0.25f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.25f,  0.25f, -0.25f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.25f, -0.25f, -0.25f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.25f, -0.25f,  0.25f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.25f, -0.25f,  0.25f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.25f,  0.25f,  0.25f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.25f,  0.25f,  0.25f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.25f,  0.25f,  0.25f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.25f, -0.25f,  0.25f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.25f,  0.25f,  0.25f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.25f,  0.25f, -0.25f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.25f, -0.25f, -0.25f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.25f, -0.25f, -0.25f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.25f, -0.25f,  0.25f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.25f,  0.25f,  0.25f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.25f,  0.25f,  0.25f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.25f,  0.25f, -0.25f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.25f, -0.25f, -0.25f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.25f, -0.25f, -0.25f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.25f, -0.25f,  0.25f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.25f,  0.25f,  0.25f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.25f, -0.25f, -0.25f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.25f, -0.25f, -0.25f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.25f, -0.25f,  0.25f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.25f, -0.25f,  0.25f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.25f, -0.25f,  0.25f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.25f, -0.25f, -0.25f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.25f,  0.25f, -0.25f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.25f,  0.25f, -0.25f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.25f,  0.25f,  0.25f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.25f,  0.25f,  0.25f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.25f,  0.25f,  0.25f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.25f,  0.25f, -0.25f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	GLfloat mapVertices[] = {
		// Positions            // Normals              // Texture Coords
		-20.0f, -20.0f, -20.0f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		20.0f, -20.0f, -20.0f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		20.0f, 20.0f, -20.0f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		20.0f,  20.0f, -20.0f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-20.0f,  20.0f, -20.0f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-20.0f, -20.0f, -20.0f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-20.0f, -20.0f, 20.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		20.0f, -20.0f,  20.0f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		20.0f,  20.0f,  20.0f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		20.0f, 20.0f, 20.0f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-20.0f,  20.0f,  20.0f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-20.0f, -20.0f,  20.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-20.0f,  20.0f, 20.0f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-20.0f, 20.0f, -20.0f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-20.0f, -20.0f, -20.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-20.0f, -20.0f, -20.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-20.0f, -20.0f,  20.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-20.0f,  20.0f,  20.0f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		20.0f, 20.0f,  20.0f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		20.0f, 20.0f, -20.0f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		20.0f, -20.0f, -20.0f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		20.0f, -20.0f, -20.0f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		20.0f, -20.0f,  20.0f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		20.0f,  20.0f,  20.0f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-20.0f, -20.0f, -20.0f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		20.0f, -20.0f, -20.0f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		20.0f, -20.0f,  20.0f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		20.0f, -20.0f,  20.0f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-20.0f, -20.0f,  20.0f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-20.0f, -20.0f, -20.0f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-20.0f,  20.0f, -20.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		20.0f,  20.0f, -20.0f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		20.0f,  20.0f,  20.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		20.0f,  20.0f,  20.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-20.0f,  20.0f,  20.0f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-20.0f,  20.0f, -20.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	// Position map boxe
	Box* Map = new Box();

	// Position all boxes
	list<Box*> Boxes;
	for (GLint i = 0; i < 100; i++) {
		Box* nBox = new Box();
		GLfloat x = RandomFloat(-15.0f, 15.0f);
		GLfloat y = RandomFloat(-15.0f, 15.0f);
		GLfloat z = RandomFloat(-15.0f, 15.0f);
		nBox->pos = vec3(x, y, z);
		Boxes.push_back(nBox);
	}

	// Positions of the point lights
	list<Box*> Lights;
	for (GLint i = 0; i < 50; i++) {
		Box* nBox = new Box();
		GLfloat x = RandomFloat(-15.0f, 15.0f);
		GLfloat y = RandomFloat(-15.0f, 15.0f);
		GLfloat z = RandomFloat(-15.0f, 15.0f);
		nBox->pos = vec3(x, y, z);
		Lights.push_back(nBox);
	}

	// Set the Map VAO (and VBO)
	GLuint VBO, mapVAO;
	glGenVertexArrays(1, &mapVAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set the vertex buffer(s) and attribute pointer(s)
	glBindVertexArray(mapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mapVertices), mapVertices, GL_STATIC_DRAW);

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Normal Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	// First, set the container's VAO (and VBO)
	GLuint boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set the vertex buffer(s) and attribute pointer(s)
	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
	glEnableVertexAttribArray(0);

	// Normal Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	// Then, we set the light's VAO (VBO stays the same)
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set the vertex buffer(s) and attribute pointer(s)
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

	// Load textures
	GLuint diffuseMap, specularMap, emissionMap, diffuseMapM, specularMapM;
	glGenTextures(1, &diffuseMap); glGenTextures(1, &diffuseMapM);
	glGenTextures(1, &specularMap); glGenTextures(1, &specularMapM);
	glGenTextures(1, &emissionMap);

	BindMapsDiSpe(diffuseMap, specularMap, "res/img/box.png", "res/img/box_spec.png");
	BindMapsDiSpe(diffuseMapM, specularMapM, "res/img/metal.png", "res/img/metal_spec.png");

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	mat4 projection = perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	GLfloat End = glfwGetTime() + 2.0f;
	//Game Loop
	while (!glfwWindowShouldClose(window)) {
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		float lDiff = drawDir ? 1.0f : 0;
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), lDiff, lDiff, lDiff);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		int L = 0; lDiff = drawPoint ? 0.8f : 0;
		for (list<Box*>::iterator it = Lights.begin(); it != Lights.end(); it++) {
			// Point Lights
			glBindVertexArray(lightVAO);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + to_string(L) + "].position").c_str()), (*it)->pos.x, (*it)->pos.y, (*it)->pos.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + to_string(L) + "].ambient").c_str()), 0.05f, 0.05f, 0.05f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + to_string(L) + "].diffuse").c_str()), lDiff, lDiff, lDiff);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + to_string(L) + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + to_string(L) + "].constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + to_string(L) + "].linear").c_str()), 0.09f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + to_string(L) + "].quadratic").c_str()), 0.032f);
			L++;
		}

		lDiff = drawSpot ? 1.0f : 0;
		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), lDiff, lDiff, lDiff);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), cos(radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), cos(radians(15.0f)));

		// Create camera transformations
		mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		// Bind Map diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMapM);

		// Bind Map specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMapM);

		// Draw Map container with the same VAO and VBO information
		glBindVertexArray(mapVAO);
		mat4 model = mat4();
		model = translate(model, Map->pos);

		Map->angle = (GLfloat)glfwGetTime() * -0.05f;
		GLfloat angle = Map->angle;
		model = rotate(model, angle, vec3(1.0f, 0.3f, 0.5f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// Bind Box diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		// Bind Box specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// Draw box containers with the same VAO and VBO information
		glBindVertexArray(boxVAO);
		for (list<Box*>::iterator it = Boxes.begin(); it != Boxes.end(); it++) {
			model = mat4();
			if ((*it)->CanJump() && CollisionCheck(camera.GetPosition(), (*it)->pos, 1.75f)) {
				(*it)->Jump(); (*it)->pCatch = false;
				engine->play2D("res/media/beep.wav");
			} else if (CollisionCheck(camera.GetPosition(), (*it)->pos, 0.75f)) {
				if (currentFrame >= End) {
					engine->play2D("res/media/drag.flac");
					End = currentFrame + 2.0f;
				}
				(*it)->pos = ReverseMovement((*it)->pos);
				(*it)->angle += 0.005f; (*it)->pCatch = true;
				for (list<Box*>::iterator it2 = Boxes.begin(); it2 != Boxes.end(); it2++) {
					if ((*it2) != (*it) && CollisionCheck((*it)->pos, (*it2)->pos, 0.75f)) {
						(*it2)->pos = CubeMovement((*it2)->pos);
						(*it2)->angle += 0.005f;
					}
				}
			} else if ((*it)->pCatch) {
				(*it)->pCatch = false;
			}

			model = translate(model, (*it)->pos);

			angle = (*it)->angle;
			model = rotate(model, angle, vec3(1.0f, 0.3f, 0.5f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
		
		// Draw the light object (using light's vertex attributes)
		// We now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightVAO);
		for (list<Box*>::iterator it = Lights.begin(); it != Lights.end(); it++) {
			model = mat4();
			model = translate(model, (*it)->pos);
			model = scale(model, vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	engine->drop();
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLWF, clearing any resources allocated by GLFW
	glfwTerminate();
	return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void DoMovement() {
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

vec3 ReverseMovement(vec3 Pos) {
	GLfloat velocity = 3.0f * deltaTime;
	vec3 front = camera.GetFront();
	vec3 right = camera.GetRight();

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		Pos += front * velocity;
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
		Pos -= front * velocity;
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
		Pos -= right * velocity;
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
		Pos += right * velocity;
	}

	return Pos;
}

void CancelMove() {
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		keys[GLFW_KEY_W] = false; keys[GLFW_KEY_UP] = false;
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		keys[GLFW_KEY_S] = false; keys[GLFW_KEY_DOWN] = false;
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		keys[GLFW_KEY_A] = false; keys[GLFW_KEY_LEFT] = false;
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		keys[GLFW_KEY_D] = false; keys[GLFW_KEY_RIGHT] = false;
	}
}

vec3 CubeMovement(vec3 Pos) {
	GLfloat velocity = 3.0f * deltaTime;
	vec3 front = camera.GetFront();
	vec3 right = camera.GetRight();

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		Pos += front * velocity;
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		Pos -= front * velocity;
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		Pos -= right * velocity;
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		Pos += right * velocity;
	}

	return Pos;
}

void OnKeyPress() {
	if (keys[GLFW_KEY_R]) {
		drawDir = !drawDir;
	} else if (keys[GLFW_KEY_F]) {
		drawSpot = !drawSpot;
	} else if (keys[GLFW_KEY_T]) {
		drawPoint = !drawPoint;
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (GLFW_PRESS == action) {
			keys[key] = true;
			OnKeyPress();
		} else if (GLFW_RELEASE == action) {
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

bool CollisionCheck(vec3 A, vec3 B, GLfloat radius) {
	GLfloat dx = A.x - B.x;
	GLfloat dy = A.y - B.y;
	GLfloat dz = A.z - B.z;
	GLfloat distance = sqrt(dx*dx + dy*dy + dz*dz);

	return (distance <= radius);
}


void BindMapsDiSpe(GLuint diffuseMap, GLuint specularMap, const char* PathD, const char* PathS) {
	int texWidth, texHeight;
	unsigned char *image;

	// Diffuse Map
	image = SOIL_load_image(PathD, &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	// Specular Map
	image = SOIL_load_image(PathS, &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}