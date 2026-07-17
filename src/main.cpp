#include <cstdio>
#include <stdexcept>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "renderer2D/inc/renderer2D.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>



int main()
{
    R2D::Renderer2D::Init(1280, 720);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(R2D::Renderer2D::GetGLFWwindow() , true);
    ImGui_ImplOpenGL3_Init("#version 460");


    while (!R2D::Renderer2D::GetShouldGLFWWindowClose())
    {
        R2D::Renderer2D::BeginFrame();

        R2D::Renderer2D::PollGLEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            // render things here
            // Test render imgui demo
            ImGui::ShowDemoWindow();
        }

        ImGui::Render();

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        R2D::Renderer2D::EndFrame();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    R2D::Renderer2D::Shutdown();
    return 0;
}
