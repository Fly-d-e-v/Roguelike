#include "Renderer.h"

#include "resources/ResourceManager.h"

#include "resources/resources/Shader.h"
#include "resources/loaders/ShaderLoader.h"

#include "Engine.h"
#include "core/config/Config.h"
#include "core/config/ConfigLoader.h"
#include "ecs/EntityManager.h"

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
 uint8_t _indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

//TODO: Hack fest, should be contain in the camera
glm::mat4 _projection_matrix = glm::mat4(1.0f);
glm::mat4 _view_matrix = glm::mat4(1.0f);


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}
	
bool Renderer::Init() {
	_EntityManager = Engine::Instance()->GetEntityManager();
	
    if (!glfwInit()) 
        return false;

    Config& config = Engine::Instance()->GetConfig();

	glfwSetErrorCallback(GLFWErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(100, 100, "RogueLike", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, GLFWFramebufferSizeCallback);
    glfwSwapInterval(config._VSyncEnabled);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		printf("Failed to initialize GLAD \n");
		glfwTerminate();
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

void Renderer::Tick(float deltaTime)
{
	GLFWwindow* window = glfwGetCurrentContext();
	
    TickImgui(deltaTime);

    glClear(GL_COLOR_BUFFER_BIT);

	//Rendering
	_shaderProgram->Use();
	glBindVertexArray(_vao);

	if (_didResize) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		_projection_matrix = glm::ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height));
		glUniformMatrix4fv(_proj_mat_uniform, 1, GL_FALSE, glm::value_ptr(_projection_matrix));
		_didResize = false;
	}
	glUniformMatrix4fv(_view_mat_uniform, 1, GL_FALSE, glm::value_ptr(_view_matrix));
	
	//TODO: Should be handled by Entity, wake me up when we have an ECS
	auto render_components = _EntityManager->GetComponents<RenderComponent>();
	for (auto render_component : render_components)
	{
		//TODO: This hurts me physically, move to game loop
		render_component->model_matrix = glm::rotate(render_components[0]->model_matrix, glm::radians(45.f * deltaTime), glm::vec3(0, 0, 1));

		glBindTexture(GL_TEXTURE_2D, render_component->texture->_textureID);
		
		glUniformMatrix4fv(_model_mat_uniform, 1, GL_FALSE, glm::value_ptr(render_components[0]->model_matrix));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
	}
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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(glfwGetCurrentContext());
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

void Renderer::GLFWFramebufferSizeCallback(GLFWwindow* , int , int )
{
	_didResize = true;
}

bool Renderer::LoadResources() {

	//TODO: REEEEEEEEEEEEE ~~ECS~~ Game Init()
	
	auto test_sprite_render_component = RenderComponent();
	test_sprite_render_component.model_matrix = glm::translate(test_sprite_render_component.model_matrix, glm::vec3(350.f, 350.f, 0.f));
	test_sprite_render_component.model_matrix = glm::scale(test_sprite_render_component.model_matrix, glm::vec3(150.f, 150.f, 1.f));
	test_sprite_render_component.texture = ResourceManager::Instance()->FetchResource<Texture>("Resources/Textures/Niels.jpg");
	
	const auto test_sprite_entity = _EntityManager->CreateEntity();
	_EntityManager->AddComponent<RenderComponent>(test_sprite_render_component, test_sprite_entity);
	
	return true;
}

void Renderer::TickImgui(float deltaTime) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Tools")) {

        _ImguiMenuItemEvent->Dispatch();
        ImGui::EndMenu();
    }
	ImGui::AlignTextToFramePadding();
	ImGui::SetCursorPosX(ImGui::GetColumnWidth() - 100);
	ImGui::Text("FPS: %4.f", 1.f / deltaTime);
	ImGui::Text("MS: %.3f ", deltaTime * 1000.f);
		
    ImGui::EndMainMenuBar();

    _ImguiToolRenderEvent->Dispatch();

    ImGui::Render();

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
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
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

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
