#include "app.hpp"

#include <cstdio>
#include <stdexcept>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "app/sceneManager.h"
#include "app/globalCamera.h"
#include "app/drawTool.hpp"

App::App()
{
    R2D::Renderer2D::Init(1280, 720);
    R2D::Renderer2D::SetWindowTitle("Fox Box - 2D Physics Engine");
    initImGUI();
    GlobalCamera::Init();
    DrawTool::Init();
    SceneManager::getInstance()->setCurrentScene(0);
    SceneManager::getInstance()->setupInputCallbacks();
}
App::~App()
{
    SceneManager::getInstance()->cleanupInputCallbacks();
    SceneManager::getInstance()->cleanup();
    DrawTool::Cleanup();
    GlobalCamera::CleanUp();
    shutdownImGUI();
    R2D::Renderer2D::Shutdown();
}

void App::run()
{
    R2D::Renderer2D::BeginFrame();
    R2D::Renderer2D::PollGLEvents();
    beginFrameImGUI();

    const glm::uvec2 viewportSize = R2D::Renderer2D::GetViewportSize();
    GlobalCamera::Get().setViewport(viewportSize.x, viewportSize.y);

    GlobalCamera::UpdateUniformBuffer();
    GlobalCamera::BindUniformBuffer();
    {
        // render things here
        SceneManager::getInstance()->updateCurrentScene();
    }

    endFrameImGUI();
    R2D::Renderer2D::EndFrame();
}

bool App::isRunning() const
{
    return !R2D::Renderer2D::GetShouldGLFWWindowClose();
}

void App::initImGUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(R2D::Renderer2D::GetGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void App::shutdownImGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void App::beginFrameImGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void App::endFrameImGUI()
{
    ImGui::Render();

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// main function
int main(int argc, char** argv) {
    App app;
    while (app.isRunning()) 
        app.run();
    return 0;
}
