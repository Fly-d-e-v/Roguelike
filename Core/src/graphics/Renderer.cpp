#include "Renderer.h"
#include "Shader.h"

#include "Engine.h"
#include "core/config/Config.h"

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "thirdparty/glad/glad.h"
#include "thirdparty/GLFW/glfw3.h"
#include "thirdparty/glm/glm.hpp"
#pragma warning(push, 0)        
//Some includes with unfixable warnings
#include "thirdparty/glm/gtc/type_ptr.hpp"
#pragma warning(pop)

bool Renderer::_didResize = true;

float _vertices[] = {
	//Positions				//Colors			//UVs
	 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left 
};
unsigned int _indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

//TODO: Hack fest, should be contain in the camera
unsigned int _width = 640, _height = 360;
glm::mat4 _projection_matrix = glm::mat4(1.0f);
glm::mat4 _view_matrix = glm::mat4(1.0f);
glm::mat4 _model_matrix = glm::mat4(1.0f);


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}
	
bool Renderer::Init() {

    if (!glfwInit()) {
        return false;
    }

   Config& config = Engine::Instance()->GetConfig();

	glfwSetErrorCallback(GLFWErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    GLFWwindow* window = glfwCreateWindow(config._ScreenWidth, config._ScreenHeight, "RogueLike", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, GLFWFramebufferSizeCallback);
    glfwSwapInterval(0); //Turns off V-Sync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD \n");
		return false;
	}
	
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//TODO: Assume we'll be using several shader programs, loading shaders and uniforms should probably be handled better... This solution is fine if we stick with a single shader.
	_shaderProgram = std::make_shared<Shader>();
	_shaderProgram->Load("Resources/Shaders/BasicVertex.glsl", "Resources/Shaders/BasicFragment.glsl");

	_model_mat_uniform = glGetUniformLocation(_shaderProgram->ID, "model");
	_view_mat_uniform = glGetUniformLocation(_shaderProgram->ID, "view");
	_proj_mat_uniform = glGetUniformLocation(_shaderProgram->ID, "projection");

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), &_vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), &_indices, GL_STATIC_DRAW);

	//Setup vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	if (!LoadResources()) {
		printf("Error loading resources\n");
		return false;
	}

    return true;
}


void Renderer::Tick(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();
    glClear(GL_COLOR_BUFFER_BIT);

    std::string title = "RogueLike - " + std::to_string(deltaTime * 1000.f) + "ms";
    glfwSetWindowTitle(window, title.c_str());

	//Rendering
	_shaderProgram->Use();
	glBindTexture(GL_TEXTURE_2D, _textureNiels);
	glBindVertexArray(_vao);

	//TODO: Should be handled by Entity, wake me up when we have an ECS
	_model_matrix = glm::rotate(_model_matrix, glm::radians(45.f * deltaTime), glm::vec3(0, 0, 1));
	glUniformMatrix4fv(_model_mat_uniform, 1, GL_FALSE, glm::value_ptr(_model_matrix));
	glUniformMatrix4fv(_view_mat_uniform, 1, GL_FALSE, glm::value_ptr(_view_matrix));
	
	if (_didResize) {
		glViewport(0, 0, _width, _height);
		_projection_matrix = glm::ortho(0.f, static_cast<float>(_width), 0.f, static_cast<float>(_height));
		glUniformMatrix4fv(_proj_mat_uniform, 1, GL_FALSE, glm::value_ptr(_projection_matrix));
		_didResize = false;
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Renderer::Deinit()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);

    glfwTerminate();
}

void Renderer::GLFWErrorCallback(int error, const char* description)
{
	error;
	fprintf(stderr, "Error: %s\n", description);
}

void Renderer::GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	window;
	_width = width;
	_height = height;
	_didResize = true;
}

bool Renderer::LoadResources()
{
	stbi_set_flip_vertically_on_load(true);
	{//Niels Texture
		int width, height, nrChannels;
		unsigned char* textureData = stbi_load("Resources/Textures/Niels.jpg", &width, &height, &nrChannels, 0);

		if (textureData) {
			glGenTextures(1, &_textureNiels);
			glBindTexture(GL_TEXTURE_2D, _textureNiels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		}
		else {
			printf("Failure loading texture Niels\n");
			return false;
		}
		stbi_image_free(textureData);
	}
	_model_matrix = glm::translate(_model_matrix, glm::vec3(static_cast<float>(_width) * 0.5f, static_cast<float>(_height) * 0.5f, 0.f));
	_model_matrix = glm::scale(_model_matrix, glm::vec3(150.f, 150.f, 1.f));

	return true;
}

bool Renderer::ShutdownRequested()
{
    return glfwWindowShouldClose(glfwGetCurrentContext());
}
