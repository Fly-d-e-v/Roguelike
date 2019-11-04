#include "Renderer.h"

#include "resources/ResourceManager.h"

#include "resources/resources/Shader.h"
#include "resources/loaders/ShaderLoader.h"

#include "Engine.h"
#include "core/config/Config.h"
#include "core/config/ConfigLoader.h"

#include "resources/loaders/TextureLoader.h"
#include "resources/resources/Texture.h"

#include <string>

#include "thirdparty/glad/glad.h"
#include "thirdparty/GLFW/glfw3.h"
#include "thirdparty/glm/glm.hpp"
#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_glfw.h"
#include "thirdparty/imgui/imgui_impl_opengl3.h"

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
    glfwSwapInterval(config._VSyncEnabled);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD \n");
		return false;
	}
	
	InitImgui(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//TODO: Assume we'll be using several shader programs, loading shaders and uniforms should probably be handled better... This solution is fine if we stick with a single shader.
    _shaderProgram = ResourceManager::Instance()->FetchResource<Shader>("Resources/Shaders/BasicVertex.shader");

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

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void Renderer::Tick(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();
	
    TickImgui(deltaTime);

    glClear(GL_COLOR_BUFFER_BIT);

    std::string title = "RogueLike - " + std::to_string(deltaTime * 1000.f) + "ms";
    glfwSetWindowTitle(window, title.c_str());

	//Rendering
	_shaderProgram->Use();
	glBindTexture(GL_TEXTURE_2D, _NielsTexture->_textureID);
	glBindVertexArray(_vao);

	//TODO: Should be handled by Entity, wake me up when we have an ECS
	_model_matrix = glm::rotate(_model_matrix, glm::radians(45.f * deltaTime), glm::vec3(0, 0, 1));
	glUniformMatrix4fv(_model_mat_uniform, 1, GL_FALSE, glm::value_ptr(_model_matrix));
	glUniformMatrix4fv(_view_mat_uniform, 1, GL_FALSE, glm::value_ptr(_view_matrix));
	
	if (_didResize) {
		Config& config = Engine::Instance()->GetConfig();
		glViewport(0, 0, config._ScreenWidth, config._ScreenHeight);
		_projection_matrix = glm::ortho(0.f, static_cast<float>(config._ScreenWidth), 0.f, static_cast<float>(config._ScreenHeight));
		glUniformMatrix4fv(_proj_mat_uniform, 1, GL_FALSE, glm::value_ptr(_projection_matrix));
		_didResize = false;
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(0);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

void Renderer::RegisterTool(std::shared_ptr<class Tool> tool) {
    _ImguiMenuItemEvent->Register(tool.get(), &Tool::MenuItemMethod);
    _ImguiToolRenderEvent->Register(tool.get(), &Tool::ToolMethod);
}

void Renderer::GLFWErrorCallback(int error, const char* description)
{
	error;
	fprintf(stderr, "Error: %s\n", description);
}

void Renderer::GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	window;
	Config& config = Engine::Instance()->GetConfig();
	config._ScreenWidth = width;
	config._ScreenHeight = height;
	ConfigLoader::SaveConfig(config);
	_didResize = true;
}

bool Renderer::LoadResources()
{
    _NielsTexture = ResourceManager::Instance()->FetchResource<Texture>("Resources/Textures/Niels.jpg");

	//TODO: REEEEEEEEEEEEE ECS
	Config& config = Engine::Instance()->GetConfig();
	_model_matrix = glm::translate(_model_matrix, glm::vec3(static_cast<float>(config._ScreenWidth) * 0.5f, static_cast<float>(config._ScreenHeight) * 0.5f, 0.f));
	_model_matrix = glm::scale(_model_matrix, glm::vec3(150.f, 150.f, 1.f));

	return true;
}

void Renderer::TickImgui(float) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Tools")) {

        _ImguiMenuItemEvent->Dispatch();
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    _ImguiToolRenderEvent->Dispatch();

    // Rendering
    ImGui::Render();

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

}

void Renderer::InitImgui (void* window) {
	//Setup Dear ImGui context
	_ImguiToolRenderEvent = std::make_shared<Event<>>();
	_ImguiMenuItemEvent = std::make_shared<Event<>>();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(window), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

bool Renderer::ShutdownRequested()
{
    return glfwWindowShouldClose(glfwGetCurrentContext());
}
