#include "Renderer.h"

#include <string>

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

unsigned int VBO;
unsigned int VAO;
unsigned int ShaderProgram;


const char* VertexShaderSource =
"#version 460 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos, 1.0f);\n"
"}\n\0";

const char* FragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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

	

	{
		//Compile shaders
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			fprintf(stderr, "Error: Vertex shader compilation failed %s\n", infoLog);

			return false;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL);

		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			fprintf(stderr, "Error: Fragment Shader compilation failed %s\n", infoLog);

			return false;
		}

		//Linking Shaders
		ShaderProgram = glCreateProgram();
		glAttachShader(ShaderProgram, vertexShader);
		glAttachShader(ShaderProgram, fragmentShader);
		glLinkProgram(ShaderProgram);

		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
			fprintf(stderr, "Error: Shader program linking failed %s\n", infoLog);

			return false;
		}
		glUseProgram(ShaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		
		//Setup vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
		glEnableVertexAttribArray(0);
	}

	
    return true;
}


void Renderer::Tick(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    std::string title = "RogueLike - " + std::to_string(static_cast<int>(1 / deltaTime)) + "fps";
    glfwSetWindowTitle(window, title.c_str());
    //@TODO design tick order for engine
	
	//Rendering
	glUseProgram(ShaderProgram);
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
}

void Renderer::Deinit()
{
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

bool Renderer::ShutdownRequested()
{
    return glfwWindowShouldClose(glfwGetCurrentContext());
}
