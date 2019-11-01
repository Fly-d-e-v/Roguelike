#include "Renderer.h"

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float vertices[] = {
	//Positions				//Colors			//UVs
	 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left 
};/
unsigned int indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

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

	glfwSetErrorCallback(GLFWErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    GLFWwindow* window = glfwCreateWindow(500, 500, "RogueLike", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, GLFWFramebufferSizeCallback);
	
	if (!window) {
		glfwTerminate();
		return false;
	}
    
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD \n");
		return false;
	}

	glViewport(0, 0, 500, 500);

    glfwSwapInterval(1);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	_shaderProgram.Load("Resources/Shaders/BasicVertex.glsl", "Resources/Shaders/BasicFragment.glsl");

	
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

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
    glfwPollEvents();

    std::string title = "RogueLike - " + std::to_string(static_cast<int>(1.f / deltaTime)) + "fps";
    glfwSetWindowTitle(window, title.c_str());
    //@TODO design tick order for engine
		
	//Rendering
	_shaderProgram.Use();
	glBindTexture(GL_TEXTURE_2D, _textureNiels);
	glBindVertexArray(_vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
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
	glViewport(0, 0, width, height);
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

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			printf("Failure loading texture Niels\n");
			return false;
		}
		stbi_image_free(textureData);
	}

	return true;
}

bool Renderer::ShutdownRequested()
{
    return glfwWindowShouldClose(glfwGetCurrentContext());
}
