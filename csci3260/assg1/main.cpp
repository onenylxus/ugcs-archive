/*
Type your name and student ID here
	- Name: Ng Wing Yin
	- Student ID: 1155127101
*/

// Include
#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>

// Namespace
using namespace std;

////////////////////////////////////////////////////////////////

// Program ID
GLint programID;

// Control variables
float time;
bool gate;
float sunRotX, sunRotY, sunRotZ;
float earthScale, earthDist;
float roll;

// Reset variables
void reset() {
	time = 0.0f;
	gate = true;
	sunRotX = sunRotY = sunRotZ = 0.0f;
	earthScale = 1.0f;
	earthDist = 4.0f;
	roll = 0.0f;
}

// OpenGL information
void get_OpenGL_info() {
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	cout << "OpenGL company: " << name << endl;
	cout << "Renderer name: " << renderer << endl;
	cout << "OpenGL version: " << glversion << endl;
}

// Check status
bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType) {
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE) {
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

// Check shader status
bool checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

// Check program status
bool checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

// Read shader code
string readShaderCode(const char* fileName) {
	ifstream meInput(fileName);
	if (!meInput.good()) {
		cout << "File failed to load ... " << fileName << endl;
		exit(1);
	}
	return string(istreambuf_iterator<char>(meInput), istreambuf_iterator<char>());
}

// Install shaders
void installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID)) { return; }
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID)) { return; }
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glUseProgram(programID);
}

////////////////////////////////////////////////////////////////

// Buffer ID
GLuint vao[4], vboPos[4], vboCol[4], ibo[3];

// Send data to OpenGL (todo)
void sendDataToOpenGL() {
	// Sun
	static const GLfloat sunPos[] = {
		+0.0000f, -0.5257f, +0.8507f,
		+0.8507f, +0.0000f, +0.5257f,
		+0.8507f, +0.0000f, -0.5257f,
		-0.8507f, +0.0000f, -0.5257f,
		-0.8507f, +0.0000f, +0.5257f,
		-0.5257f, +0.8507f, +0.0000f,
		+0.5257f, +0.8507f, +0.0000f,
		+0.5257f, -0.8507f, +0.0000f,
		-0.5257f, -0.8507f, +0.0000f,
		+0.0000f, -0.5257f, -0.8507f,
		+0.0000f, +0.5257f, -0.8507f,
		+0.0000f, +0.5257f, +0.8507f
	};

	static const GLfloat sunCol[] = {
		1.0f, 0.2f, 0.0f,
		0.9f, 0.5f, 0.0f,
		0.8f, 0.7f, 0.0f,
		1.0f, 0.6f, 0.0f,
		0.9f, 0.4f, 0.0f,
		1.0f, 0.6f, 0.0f,
		0.8f, 1.0f, 0.0f,
		1.0f, 0.5f, 0.0f,
		0.7f, 0.2f, 0.0f,
		0.8f, 0.8f, 0.0f,
		1.0f, 0.7f, 0.0f,
		0.9f, 0.3f, 0.0f
	};

	static const GLushort sunInd[] = {
		1, 2, 6,
		1, 7, 2,
		3, 4, 5,
		4, 3, 8,
		6, 5, 11,
		5, 6, 10,
		9, 10, 2,
		10, 9, 3,
		7, 8, 9,
		8, 7, 0,
		11, 0, 1,
		0, 11, 4,
		6, 2, 10,
		1, 6, 11,
		3, 5, 10,
		5, 4, 11,
		2, 7, 9,
		7, 1, 0,
		3, 9, 8,
		4, 8, 0
	};

	// Earth
	static const GLfloat earthPos[] = {
		-0.3f, -0.3f, -0.3f,
		-0.3f, -0.3f, +0.3f,
		-0.3f, +0.3f, -0.3f,
		-0.3f, +0.3f, +0.3f,
		+0.3f, -0.3f, -0.3f,
		+0.3f, -0.3f, +0.3f,
		+0.3f, +0.3f, -0.3f,
		+0.3f, +0.3f, +0.3f
	};

	static const GLfloat earthCol[] = {
		0.0f, 0.2f, 1.0f,
		0.0f, 0.3f, 1.0f,
		0.0f, 0.1f, 0.9f,
		0.0f, 0.8f, 1.0f,
		0.0f, 0.6f, 1.0f,
		0.0f, 0.4f, 1.0f,
		0.0f, 0.4f, 0.8f,
		0.0f, 0.5f, 1.0f
	};

	static const GLushort earthInd[] = {
		0, 4, 2,
		2, 4, 6,
		4, 5, 6,
		6, 5, 7,
		5, 1, 7,
		7, 1, 3,
		1, 0, 3,
		3, 0, 2,
		2, 6, 3,
		3, 6, 7,
		1, 5, 0,
		0, 5, 4
	};

	// UFO
	static const GLfloat ufoPos[] = {
		0.0f, +0.3f, 0.0f,
		0.0f, -0.3f, 0.3f,
		0.3f, -0.3f, -0.6f,
		-0.3f, -0.3f, -0.6f
	};

	static const GLfloat ufoCol[] = {
		0.2f, 0.5f, 0.2f,
		0.4f, 0.5f, 0.2f,
		0.3f, 0.5f, 0.2f,
		0.2f, 0.6f, 0.3f
	};

	static const GLushort ufoInd[] = {
		2, 0, 3,
		0, 2, 1,
		3, 1, 2,
		1, 3, 0
	};

	// Ship
	static const GLfloat shipPos[] = {
		0.0f, 0.0f, 0.1f,
		0.05f, 0.0f, -0.1f,
		-0.05f, 0.0f, -0.1f
	};

	static const GLfloat shipCol[] = {
		0.8f, 0.8f, 0.8f,
		0.6f, 0.6f, 0.6f,
		0.6f, 0.6f, 0.6f
	};

	// Generate buffers
	glGenVertexArrays(4, vao);
	glGenBuffers(4, vboPos);
	glGenBuffers(4, vboCol);
	glGenBuffers(3, ibo);

	glBindVertexArray(vao[0]); // Sun
	
	glBindBuffer(GL_ARRAY_BUFFER, vboPos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sunPos), sunPos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboCol[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sunCol), sunCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sunInd), sunInd, GL_STATIC_DRAW);

	glBindVertexArray(vao[1]); // Earth

	glBindBuffer(GL_ARRAY_BUFFER, vboPos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(earthPos), earthPos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, vboCol);
	glBindBuffer(GL_ARRAY_BUFFER, vboCol[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(earthCol), earthCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(earthInd), earthInd, GL_STATIC_DRAW);

	glBindVertexArray(vao[2]); // UFO

	glBindBuffer(GL_ARRAY_BUFFER, vboPos[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ufoPos), ufoPos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, vboCol);
	glBindBuffer(GL_ARRAY_BUFFER, vboCol[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ufoCol), ufoCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ufoInd), ufoInd, GL_STATIC_DRAW);

	glBindVertexArray(vao[3]); // Ship

	glBindBuffer(GL_ARRAY_BUFFER, vboPos[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shipPos), shipPos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, vboCol);
	glBindBuffer(GL_ARRAY_BUFFER, vboCol[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shipCol), shipCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0); // Unbind
}

// Paint GL (todo)
void paintGL(void) {
	// Matrices
	glm::mat4 initMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f) * glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	glm::mat4 resultMatrix;

	GLint loc = glGetUniformLocation(programID, "modelTransformMatrix");
	
	// Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Clear
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Time update
	if (gate) { time += 0.1f; }
	float ufoDist = earthScale * 0.3f + 0.5f;

	// Sun
	glBindVertexArray(vao[0]);
	resultMatrix = initMatrix;
	resultMatrix *= glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0, 0, 1));
	resultMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -15));
	resultMatrix *= glm::rotate(glm::mat4(1.0f), sunRotX - time / 1800, glm::vec3(1, 0, 0));
	resultMatrix *= glm::rotate(glm::mat4(1.0f), sunRotY + time / 3600, glm::vec3(0, 1, 0));
	resultMatrix *= glm::rotate(glm::mat4(1.0f), sunRotZ - time / 2700, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(loc, 1, GL_FALSE, &resultMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_SHORT, nullptr);

	// Earth
	glBindVertexArray(vao[1]);
	resultMatrix = initMatrix;
	resultMatrix *= glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0, 0, 1));
	resultMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(earthDist * cos(time / 360), cos(time / 180) / 5, -15 + earthDist * sin(time / 360)));
	resultMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(earthScale, earthScale, earthScale));
	resultMatrix *= glm::rotate(glm::mat4(1.0f), time / 180, glm::vec3(0, 1, 0));
	resultMatrix *= glm::rotate(glm::mat4(1.0f), time / 900, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(loc, 1, GL_FALSE, &resultMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

	// UFO
	glBindVertexArray(vao[2]);
	resultMatrix = initMatrix;
	resultMatrix *= glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0, 0, 1));
	resultMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(earthDist * cos(time / 360) + ufoDist * cos(time / 90), cos(time / 180) / 5, -15 + earthDist * sin(time / 360) + ufoDist * sin(time / 90)));
	resultMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 0.6f));
	resultMatrix *= glm::rotate(glm::mat4(1.0f), time / 180, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(loc, 1, GL_FALSE, &resultMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, nullptr);

	// Ship
	glBindVertexArray(vao[3]);
	resultMatrix = glm::mat4(1.0f);
	resultMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(sin(time / 1800) / 3, -0.6 - sin(time / 360) / 8, 0));
	resultMatrix *= glm::rotate(glm::mat4(1.0f), cos(time / 360) / 4, glm::vec3(1, 0, 0));
	resultMatrix *= glm::rotate(glm::mat4(1.0f), sin(time / 1800) / 3, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(loc, 1, GL_FALSE, &resultMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 9);

	glBindVertexArray(0);
}

// Key callback (todo)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
	if (key == GLFW_KEY_R && action == GLFW_PRESS) { reset(); }
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { gate = !gate; }

	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) { sunRotX -= 0.1f; }
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) { sunRotX += 0.1f; }
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) { sunRotY -= 0.1f; }
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) { sunRotY += 0.1f; }
	if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) { sunRotZ -= 0.1f; }
	if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) { sunRotZ += 0.1f; }

	if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)) { roll -= 0.01f; }
	if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT)) { roll += 0.01f; }
	if (key == GLFW_KEY_COMMA && (action == GLFW_PRESS || action == GLFW_REPEAT)) { earthDist = max(earthDist - 0.1f, 2.0f); }
	if (key == GLFW_KEY_PERIOD && (action == GLFW_PRESS || action == GLFW_REPEAT)) { earthDist = min(earthDist + 0.1f, 6.0f); }
	if (key == GLFW_KEY_MINUS && (action == GLFW_PRESS || action == GLFW_REPEAT)) { earthScale = max(earthScale - 0.1f, 0.5f); }
	if (key == GLFW_KEY_EQUAL && (action == GLFW_PRESS || action == GLFW_REPEAT)) { earthScale = min(earthScale + 0.1f, 2.0f); }
}

// Frame buffer size callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Initialize GL (run once, todo)
void initializedGL(void) {
	sendDataToOpenGL();
	installShaders();
	reset();
}

////////////////////////////////////////////////////////////////

// Main function
int main(int argc, char* argv[]) {
	GLFWwindow* window;

	// Initialize the glfw
	if (!glfwInit()) {
		cout << "Failed to initialize GLFW" << endl;
		return -1;
	}

	// glfw: configure; necessary for MAC
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(800, 600, "Assignment 1", NULL, NULL);
	if (!window) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	// Initialize the glew
	if (GLEW_OK != glewInit()) {
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}
	get_OpenGL_info();
	initializedGL();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		paintGL();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
