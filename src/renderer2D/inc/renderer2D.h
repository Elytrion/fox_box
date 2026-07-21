#pragma once
#include "renderer2d/inc/utils/gl_commons.hpp"
#include <glm/glm.hpp>
#include <string>
#include <functional>

struct GLFWwindow;

namespace R2D
{
	class Buffer;
	class VertexArray;

	enum class MouseButton : uint8_t
	{
		LEFT = GLFW_MOUSE_BUTTON_1,
		RIGHT = GLFW_MOUSE_BUTTON_2,
		MIDDLE = GLFW_MOUSE_BUTTON_3,
		INVALID = 255,
	};
	enum class MouseAction : uint8_t
	{
		RELEASE = GLFW_RELEASE,
		PRESS = GLFW_PRESS,
		REPEAT = GLFW_REPEAT,
		INVALID = 255,
	};
	struct MouseEvent
	{
		MouseButton button{ MouseButton::INVALID };
		MouseAction action{ MouseAction::INVALID };
		glm::ivec2  cursorPos{ 0, 0 };
		glm::vec2   scroll{ 0.f, 0.f };
	};

	class Renderer2D
	{
	public:

		static void Init(uint32_t width, uint32_t height);

		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();
		static void PollGLEvents();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void ClearColour(float red = 0.1f, float green = 0.12f, float blue = 0.15f, float alpha = 1.f);
		static void ClearDepth(float value = 1.f);
		static void Clear();


		// Drawing function
		static void drawArraysEmpty(uint32_t vertexCount, uint32_t firstVertex = 0, DRAW_MODE mode = DRAW_MODE::TRIANGLES);
		static void drawArraysEmptyInstanced(
			uint32_t  vertexCount,
			uint32_t  firstInstance = 0,
			uint32_t  instanceCount = 0,
			DRAW_MODE mode = DRAW_MODE::TRIANGLES
		);
		static void drawArrays(uint32_t vertexCount, uint32_t firstVertex = 0, DRAW_MODE mode = DRAW_MODE::TRIANGLES);
		static void drawArraysInstanced(uint32_t vertexCount, uint32_t firstVertex = 0, uint32_t instanceCount = 0, DRAW_MODE mode = DRAW_MODE::TRIANGLES);

		static BufferHandle registerBuffer();

		static GLFWwindow* GetGLFWwindow();
		static bool GetShouldGLFWWindowClose();
		static void SignalGLFWWindowClose();
		static void SignalGLFWWindowFocus();

		static void SetWindowTitle(const std::string& title);

	private:
		static void KeyPressCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
		static void MouseButtonPressCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallback(GLFWwindow* window, double x, double y);
		static void ScrollCallback(GLFWwindow* window, double x, double y);
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);

		struct RenderContext;
		static RenderContext m_context;

		static std::string m_windowTitle;

		static BufferHandle m_bufferCounter;
	};
}