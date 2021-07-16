/*
Student Information
Student ID: 1155127101
Student Name: Ng Wing Yin
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

////////////////////////////////////////////////////////////////

// Dimensions
const int WIDTH = 800, HEIGHT = 600;

// Structs
struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Model {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

// Shader
Shader shader, shadow;

// Camera
namespace Camera {
	glm::vec3 pos(15.0f, -1.0f, 0.0f);
	glm::vec3 target(0.0f, -1.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	float zoom = 45.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;
}

// Lights
namespace Lights {
	glm::vec3 dlDirection(5.0f, -12.0f, 5.0f);
	float dlIntensity = 2.0f;
	glm::vec3 dlAmbient(0.1f, 0.1f, 0.1f);
	glm::vec3 dlDiffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 dlSpecular(0.3f, 0.3f, 0.3f);

	glm::vec3 pl0Position(2.0f, -0.5f, 0.0f);
	float pl0Constant = 1.0f;
	float pl0Linear = 0.1f;
	float pl0Quadratic = 0.025f;
	glm::vec3 pl0Ambient(1.0f, 0.0f, 0.0f);
	glm::vec3 pl0Diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 pl0Specular(0.3f, 0.3f, 0.3f);

	glm::vec3 pl1Position(0.0f, -0.5f, 2.0f);
	float pl1Constant = 1.0f;
	float pl1Linear = 0.1f;
	float pl1Quadratic = 0.025f;
	glm::vec3 pl1Ambient(0.0f, 1.0f, 0.0f);
	glm::vec3 pl1Diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 pl1Specular(0.3f, 0.3f, 0.3f);

	glm::vec3 pl2Position(-2.0f, -0.5f, 0.0f);
	float pl2Constant = 1.0f;
	float pl2Linear = 0.1f;
	float pl2Quadratic = 0.025f;
	glm::vec3 pl2Ambient(0.0f, 0.0f, 1.0f);
	glm::vec3 pl2Diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 pl2Specular(0.3f, 0.3f, 0.3f);

	glm::vec3 pl3Position(0.0f, -0.5f, -2.0f);
	float pl3Constant = 1.0f;
	float pl3Linear = 0.1f;
	float pl3Quadratic = 0.025f;
	glm::vec3 pl3Ambient(1.0f, 1.0f, 0.0f);
	glm::vec3 pl3Diffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 pl3Specular(0.3f, 0.3f, 0.3f);

	glm::vec3 slPosition(0.0f, 0.0f, 0.0f);
	glm::vec3 slDirection(-1.0f, 0.0f, 0.0f);
	float slCutoff = glm::cos(glm::radians(12.5f));
	float slOuterCutoff = glm::cos(glm::radians(15.0f));
	float slConstant = 1.0f;
	float slLinear = 0.25f;
	float slQuadratic = 0.1f;
	glm::vec3 slAmbient(0.2f, 0.2f, 0.2f);
	glm::vec3 slDiffuse(1.0f, 1.0f, 1.0f);
	glm::vec3 slSpecular(0.3f, 0.3f, 0.3f);
}

// Controls
namespace Controls {
	// Textures
	int dolphinTxtr = 0;
	int seaTxtr = 0;

	// Mouse
	float mousePosX = WIDTH / 2;
	float mousePosY = HEIGHT / 2;
	bool isLeftClick = false;
}

// Dolphin
namespace Dolphin {
	glm::vec3 position(0, -1, 0);
	float direction = 0.0f;
}

// Time
float time = 0.0f;
float oldTime = 0.0f;

// Dolphin model and texture
Model dolphinModel;
Texture dolphinTexture[2];
GLuint dolphinVAO, dolphinVBO, dolphinEBO;

// Sea model and texture
Model seaModel;
Texture seaTexture[2];
GLuint seaVAO, seaVBO, seaEBO;

// Depth map
GLuint depthMapFBO, depthMap;

// Namespace
using namespace std;

////////////////////////////////////////////////////////////////

// Load OBJ to model
Model loadOBJ(const char* objPath) {
	struct V {
		// struct for identify if a vertex has showed up
		unsigned int index_position, index_uv, index_normal;
		bool operator == (const V& v) const {
			return index_position == v.index_position && index_uv == v.index_uv && index_normal == v.index_normal;
		}
		bool operator < (const V& v) const {
			return (index_position < v.index_position) ||
				(index_position == v.index_position && index_uv < v.index_uv) ||
				(index_position == v.index_position && index_uv == v.index_uv && index_normal < v.index_normal);
		}
	};

	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::map<V, unsigned int> temp_vertices;

	Model model;
	unsigned int num_vertices = 0;
	std::cout << "\nLoading OBJ file " << objPath << "..." << std::endl;

	// Check for Error
	std::ifstream file;
	file.open(objPath);
	if (file.fail()) {
		std::cerr << "Impossible to open the file! Do you use the right path? See Tutorial 6 for details" << std::endl;
		exit(1);
	}

	while (!file.eof()) {
		// process the object file
		char lineHeader[128];
		file >> lineHeader;

		if (strcmp(lineHeader, "v") == 0) {
			// geometric vertices
			glm::vec3 position;
			file >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			// texture coordinates
			glm::vec2 uv;
			file >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			// vertex normals
			glm::vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			// Face elements
			V vertices[3];
			for (int i = 0; i < 3; i++) {
				char ch;
				file >> vertices[i].index_position >> ch >> vertices[i].index_uv >> ch >> vertices[i].index_normal;
			}

			// Check if there are more than three vertices in one face.
			std::string redundency;
			std::getline(file, redundency);
			if (redundency.length() >= 5) {
				std::cerr << "There may exist some errors while load the obj file. Error content: [" << redundency << " ]" << std::endl;
				std::cerr << "Please note that we only support the faces drawing with triangles. There are more than three vertices in one face." << std::endl;
				std::cerr << "Your obj file can't be read properly by our simple parser :-( Try exporting with other options." << std::endl;
				exit(1);
			}

			for (int i = 0; i < 3; i++) {
				if (temp_vertices.find(vertices[i]) == temp_vertices.end()) {
					// the vertex never shows before
					Vertex vertex;
					vertex.position = temp_positions[vertices[i].index_position - 1];
					vertex.uv = temp_uvs[vertices[i].index_uv - 1];
					vertex.normal = temp_normals[vertices[i].index_normal - 1];

					model.vertices.push_back(vertex);
					model.indices.push_back(num_vertices);
					temp_vertices[vertices[i]] = num_vertices;
					num_vertices += 1;
				}
				else {
					// reuse the existing vertex
					unsigned int index = temp_vertices[vertices[i]];
					model.indices.push_back(index);
				}
			} // for
		} // else if
		else {
			// it's not a vertex, texture coordinate, normal or face
			char stupidBuffer[1024];
			file.getline(stupidBuffer, 1024);
		}
	}
	file.close();
	std::cout << "There are " << num_vertices << " vertices in the obj file.\n" << std::endl;
	return model;
}

// Get OpenGL info
void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

// Send data to OpenGL
void sendDataToOpenGL() {
	// Dolphin VAO and VBO
	dolphinModel = loadOBJ("resources/dolphin/dolphin.obj");
	glGenVertexArrays(1, &dolphinVAO);
	glBindVertexArray(dolphinVAO);
	glGenBuffers(1, &dolphinVBO);
	glBindBuffer(GL_ARRAY_BUFFER, dolphinVBO);
	glBufferData(GL_ARRAY_BUFFER, dolphinModel.vertices.size() * sizeof(Vertex), &dolphinModel.vertices[0], GL_STATIC_DRAW);

	// Dolphin Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Dolphin EBO
	glGenBuffers(1, &dolphinEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dolphinEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dolphinModel.indices.size() * sizeof(unsigned int), &dolphinModel.indices[0], GL_STATIC_DRAW);

	// Sea VAO and VBO
	seaModel = loadOBJ("resources/sea/sea.obj");
	glGenVertexArrays(1, &seaVAO);
	glBindVertexArray(seaVAO);
	glGenBuffers(1, &seaVBO);
	glBindBuffer(GL_ARRAY_BUFFER, seaVBO);
	glBufferData(GL_ARRAY_BUFFER, seaModel.vertices.size() * sizeof(Vertex), &seaModel.vertices[0], GL_STATIC_DRAW);

	// Sea Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Sea EBO
	glGenBuffers(1, &seaEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, seaEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, seaModel.indices.size() * sizeof(unsigned int), &seaModel.indices[0], GL_STATIC_DRAW);


	// Clear buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Depth texture for shadows
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "Frame buffer is not complete" << endl;
	}

	//Load textures
	dolphinTexture[0].setupTexture("resources/dolphin/dolphin_01.jpg");
	dolphinTexture[1].setupTexture("resources/dolphin/dolphin_02.jpg");
	seaTexture[0].setupTexture("resources/sea/sea_01.jpg");
	seaTexture[1].setupTexture("resources/sea/sea_02.jpg");
}

// Init function
void initializedGL(void) {
	// OpenGL setup
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	// Shader setup
	shader.setupShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
	shadow.setupShader("VertexShadowCode.glsl", "FragmentShadowCode.glsl");

	// Depth test and cull face
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

// Paint function
void paintGL(void) {
	// Update time
	float newTime = glfwGetTime();
	time += newTime - oldTime;
	oldTime = newTime;

	// Matrices
	glm::mat4 projMatrix = glm::perspective(glm::radians(Camera::zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	glm::mat4 biasMatrix(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);
	glm::mat4 depthMatrix = glm::ortho(-10.0f, 15.0f, -10.0f, 15.0f, -20.0f, 20.0f) * glm::lookAt(-Lights::dlDirection, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthBiasMatrix = biasMatrix * depthMatrix;
	glm::mat4 viewMatrix = glm::lookAt(Camera::pos, Camera::target, Camera::up);
	glm::mat4 mtMatrix;

	// Vectors
	glm::vec3 strobe0(int(int(glm::floor(time * 4)) % 4 == 0));
	glm::vec3 strobe1(int(int(glm::floor(time * 4)) % 4 == 1));
	glm::vec3 strobe2(int(int(glm::floor(time * 4)) % 4 == 2));
	glm::vec3 strobe3(int(int(glm::floor(time * 4)) % 4 == 3));

	//////////////////////////////////////////////////////////////

	// Clear
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render depth map
	glViewport(0, 0, 1024, 1024);
	shadow.use();

	shadow.setMat4("depthMatrix", depthMatrix);

	// Set transformation matrix for dolphin
	mtMatrix = glm::translate(glm::mat4(1.0f), Dolphin::position);
	mtMatrix *= glm::rotate(glm::mat4(1.0f), Dolphin::direction, glm::vec3(0, 1, 0));
	mtMatrix *= glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1, 0, 0));
	mtMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.02f, 0.02f));
	shadow.setMat4("modelTransformMatrix", mtMatrix);

	// Bind texture for dolphin
	dolphinTexture[Controls::dolphinTxtr].bind(0);
	glBindVertexArray(dolphinVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dolphinEBO);
	glDrawElements(GL_TRIANGLES, GLsizei(dolphinModel.indices.size()), GL_UNSIGNED_INT, 0);

	// Set transformation matrix for sea
	mtMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 0));
	mtMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10));
	shadow.setMat4("modelTransformMatrix", mtMatrix);

	// Bind texture for sea
	seaTexture[Controls::seaTxtr].bind(0);
	glBindVertexArray(seaVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, seaEBO);
	glDrawElements(GL_TRIANGLES, GLsizei(seaModel.indices.size()), GL_UNSIGNED_INT, 0);

	// Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//////////////////////////////////////////////////////////////

	// Clear
	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.4f, 0.8f, 0.9f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.use();

	// Set lighting information
	Lights::slPosition = Camera::pos;
	Lights::slDirection = glm::normalize(Camera::target - Camera::pos);
	shader.setVec3("viewPos", Camera::pos);
	shader.setMat4("depthBiasMatrix", depthBiasMatrix);

	shader.setVec3("dirLight.direction", Lights::dlDirection);
	shader.setFloat("dirLight.intensity", Lights::dlIntensity);
	shader.setVec3("dirLight.ambient", Lights::dlAmbient);
	shader.setVec3("dirLight.diffuse", Lights::dlDiffuse);
	shader.setVec3("dirLight.specular", Lights::dlSpecular);

	shader.setVec3("pointLight[0].position", Lights::pl0Position);
	shader.setFloat("pointLight[0].constant", Lights::pl0Constant);
	shader.setFloat("pointLight[0].linear", Lights::pl0Linear);
	shader.setFloat("pointLight[0].quadratic", Lights::pl0Quadratic);
	shader.setVec3("pointLight[0].ambient", Lights::pl0Ambient * strobe0);
	shader.setVec3("pointLight[0].diffuse", Lights::pl0Diffuse);
	shader.setVec3("pointLight[0].specular", Lights::pl0Specular);

	shader.setVec3("pointLight[1].position", Lights::pl1Position);
	shader.setFloat("pointLight[1].constant", Lights::pl1Constant);
	shader.setFloat("pointLight[1].linear", Lights::pl1Linear);
	shader.setFloat("pointLight[1].quadratic", Lights::pl1Quadratic);
	shader.setVec3("pointLight[1].ambient", Lights::pl1Ambient * strobe1);
	shader.setVec3("pointLight[1].diffuse", Lights::pl1Diffuse);
	shader.setVec3("pointLight[1].specular", Lights::pl1Specular);

	shader.setVec3("pointLight[2].position", Lights::pl2Position);
	shader.setFloat("pointLight[2].constant", Lights::pl2Constant);
	shader.setFloat("pointLight[2].linear", Lights::pl2Linear);
	shader.setFloat("pointLight[2].quadratic", Lights::pl2Quadratic);
	shader.setVec3("pointLight[2].ambient", Lights::pl2Ambient * strobe2);
	shader.setVec3("pointLight[2].diffuse", Lights::pl2Diffuse);
	shader.setVec3("pointLight[2].specular", Lights::pl2Specular);

	shader.setVec3("pointLight[3].position", Lights::pl3Position);
	shader.setFloat("pointLight[3].constant", Lights::pl3Constant);
	shader.setFloat("pointLight[3].linear", Lights::pl3Linear);
	shader.setFloat("pointLight[3].quadratic", Lights::pl3Quadratic);
	shader.setVec3("pointLight[3].ambient", Lights::pl3Ambient * strobe3);
	shader.setVec3("pointLight[3].diffuse", Lights::pl3Diffuse);
	shader.setVec3("pointLight[3].specular", Lights::pl3Specular);

	shader.setVec3("spotLight.position", Lights::slPosition);
	shader.setVec3("spotLight.direction", Lights::slDirection);
	shader.setFloat("spotLight.cutoff", Lights::slCutoff);
	shader.setFloat("spotLight.outerCutoff", Lights::slOuterCutoff);
	shader.setFloat("spotLight.constant", Lights::slConstant);
	shader.setFloat("spotLight.linear", Lights::slLinear);
	shader.setFloat("spotLight.quadratic", Lights::slQuadratic);
	shader.setVec3("spotLight.ambient", Lights::slAmbient);
	shader.setVec3("spotLight.diffuse", Lights::slDiffuse);
	shader.setVec3("spotLight.specular", Lights::slSpecular);

	shader.setInt("material.txtr", 0);
	shader.setFloat("material.shininess", 32.0f);

	shader.setMat4("projectionMatrix", projMatrix);
	shader.setMat4("viewMatrix", viewMatrix);

	// Set transformation matrix for dolphin
	mtMatrix = glm::translate(glm::mat4(1.0f), Dolphin::position);
	mtMatrix *= glm::rotate(glm::mat4(1.0f), Dolphin::direction, glm::vec3(0, 1, 0));
	mtMatrix *= glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1, 0, 0));
	mtMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.02f, 0.02f));
	shader.setMat4("modelTransformMatrix", mtMatrix);

	// Bind texture for dolphin
	dolphinTexture[Controls::dolphinTxtr].bind(0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shader.setInt("depthMap", 1);

	glBindVertexArray(dolphinVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dolphinEBO);
	glDrawElements(GL_TRIANGLES, GLsizei(dolphinModel.indices.size()), GL_UNSIGNED_INT, 0);

	// Set transformation matrix for sea
	mtMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 5, 0));
	mtMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10));
	shader.setMat4("modelTransformMatrix", mtMatrix);

	// Bind texture for sea
	seaTexture[Controls::seaTxtr].bind(0);
	glBindVertexArray(seaVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, seaEBO);
	glDrawElements(GL_TRIANGLES, GLsizei(seaModel.indices.size()), GL_UNSIGNED_INT, 0);
}

////////////////////////////////////////////////////////////////

// Frame buffer callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) { Controls::isLeftClick = action == GLFW_PRESS; }
}

// Cursor position callback
void cursor_position_callback(GLFWwindow* window, double x, double y) {
	float xOffset = float(x) - Controls::mousePosX;
	float yOffset = Controls::mousePosY - float(y);
	Controls::mousePosX = float(x);
	Controls::mousePosY = float(y);

	if (Controls::isLeftClick) {
		Camera::yaw += xOffset * 0.1f;
		Camera::pitch += yOffset * 0.1f;

		if (Camera::pitch > 89.0f) {
			Camera::pitch = 89.0f;
		}
		else if (Camera::pitch < 0.0f) {
			Camera::pitch = 0.0f;
		}
	}

	float radius = glm::distance(Camera::pos, Camera::target);
	Camera::pos = glm::vec3(glm::cos(glm::radians(Camera::pitch)) * glm::cos(glm::radians(Camera::yaw)) * radius, -1 + glm::sin(glm::radians(Camera::pitch)) * radius, glm::cos(glm::radians(Camera::pitch)) * glm::sin(glm::radians(Camera::yaw)) * radius);
}

// Mouse scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Camera::zoom -= (float)yoffset;
	if (Camera::zoom < 1.0f)
		Camera::zoom = 1.0f;
	if (Camera::zoom > 45.0f)
		Camera::zoom = 45.0f;
}

// Keyboard key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Texture control
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) { Controls::dolphinTxtr = 0; }
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) { Controls::dolphinTxtr = 1; }
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) { Controls::seaTxtr = 0; }
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) { Controls::seaTxtr = 1; }

	// Dolphin movement
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) { Dolphin::position += glm::vec3(glm::sin(Dolphin::direction) / 20.0f, 0.0f, glm::cos(Dolphin::direction) / 20.0f); }
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) { Dolphin::position -= glm::vec3(glm::sin(Dolphin::direction) / 20.0f, 0.0f, glm::cos(Dolphin::direction) / 20.0f); }
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) { Dolphin::direction += 0.02f; }
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) { Dolphin::direction -= 0.02f; }

	// Directional light
	if (key == GLFW_KEY_W && action == GLFW_REPEAT) { Lights::dlIntensity = glm::min(Lights::dlIntensity + 0.05f, 4.0f); }
	if (key == GLFW_KEY_S && action == GLFW_REPEAT) { Lights::dlIntensity = glm::max(Lights::dlIntensity - 0.05f, 0.0f); }
}

////////////////////////////////////////////////////////////////

// Main function
int main(int argc, char* argv[]) {
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	initializedGL();

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
