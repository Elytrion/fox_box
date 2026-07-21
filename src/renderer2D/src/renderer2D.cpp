#include "renderer2d/inc/renderer2D.h"
#include "renderer2d/inc/objects/shaderProgram.hpp"
#include "renderer2d/inc/utils/shaderManager.hpp"
#include "renderer2d/inc/objects/varray.hpp"
#include "renderer2d/inc/objects/buffer.hpp"
#include "renderer2d/inc/utils/gl_logger.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace
{
	static void glfw_error_callback(int error, const char* desc)
	{
		std::fprintf(stderr, "GLFW Error %d: %s\n", error, desc);
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	static bool g_isMouseHeld{ false };
	static glm::ivec2 g_cursorPos{ 0, 0 };
}

namespace R2D
{
	struct Renderer2D::RenderContext
	{
		uint32_t height;
		uint32_t width;

		GLFWwindow* window;
	};
	Renderer2D::RenderContext Renderer2D::m_context;
	std::string Renderer2D::m_windowTitle{ "Renderer2D" };
	BufferHandle Renderer2D::m_bufferCounter{ 1 };

	void Renderer2D::Init(uint32_t width, uint32_t height)
	{
		glfwSetErrorCallback(glfw_error_callback);

		// Init glfw
		if (!glfwInit())
		{
			std::exit(EXIT_FAILURE);
		}

		// Create window
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		m_context.window = glfwCreateWindow(width, height, m_windowTitle.c_str(), nullptr, nullptr);
		if (!m_context.window)
		{
			glfwTerminate();
			std::exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(m_context.window);

		glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::fprintf(stderr, "Failed to initialize GLAD\n");
			std::exit(EXIT_FAILURE);
		}

		int cwidth, cheight;
		glfwGetWindowSize(m_context.window, &cwidth, &cheight);
		m_context.width = static_cast<uint32_t>(cwidth);
		m_context.height = static_cast<uint32_t>(cheight);
		glViewport(0, 0, m_context.width, m_context.height);


		glfwSetKeyCallback(m_context.window, KeyPressCallback);
		glfwSetMouseButtonCallback(m_context.window, MouseButtonPressCallback);
		glfwSetCursorPosCallback(m_context.window, CursorPosCallback);
		glfwSetScrollCallback(m_context.window, ScrollCallback);
		glfwSetFramebufferSizeCallback(m_context.window, framebuffer_size_callback);
		glfwSetWindowSizeCallback(m_context.window, WindowSizeCallback);
	}

	void Renderer2D::Shutdown()
	{
		m_bufferCounter = 0;

		if (m_context.window)
		{
			glfwDestroyWindow(m_context.window);
		}

		glfwTerminate();
	}

	void Renderer2D::BeginFrame()
	{
		SetViewport(0, 0, m_context.width, m_context.height);
		ClearColour();
		ClearDepth();
		Clear();
	}

	void Renderer2D::EndFrame()
	{
		glFinish();
		glfwSwapBuffers(m_context.window);
	}

	void Renderer2D::PollGLEvents()
	{
		glfwPollEvents();
	}


	// Drawing function
	void Renderer2D::drawArraysEmpty(uint32_t vertexCount, uint32_t firstVertex, DRAW_MODE mode)
	{
		glDrawArrays(
			static_cast<GLenum>(mode),
			firstVertex,
			vertexCount
		);
		LOG_GL_ERRORS("DRAW ARRAYS EMPTY");
	}
	void Renderer2D::drawArraysEmptyInstanced(
		uint32_t  vertexCount,
		uint32_t  firstInstance,
		uint32_t  instanceCount,
		DRAW_MODE mode
	)
	{
		glDrawArraysInstancedBaseInstance(
			static_cast<GLenum>(mode),
			0,
			vertexCount,
			instanceCount,
			firstInstance
		);
		LOG_GL_ERRORS("DRAW ARRAYS EMPTY INSTANCED");
	}
	void Renderer2D::drawArrays(uint32_t vertexCount, uint32_t firstVertex, DRAW_MODE mode)
	{
		glDrawArrays
		(
			static_cast<GLenum>(mode),
			firstVertex,
			vertexCount
		);
		LOG_GL_ERRORS("DRAW ARRAYS");
	}
	void Renderer2D::drawArraysInstanced(uint32_t vertexCount, uint32_t firstVertex, uint32_t instanceCount, DRAW_MODE mode)
	{
		glDrawArraysInstanced
		(
			static_cast<GLenum>(mode),
			firstVertex,
			vertexCount,
			instanceCount
		);
		LOG_GL_ERRORS("DRAW ARRAYS INSTANCED");
	}

	BufferHandle Renderer2D::registerBuffer()
	{
		return m_bufferCounter++;
	}

	void Renderer2D::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void Renderer2D::ClearColour(float red, float green, float blue, float alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	void Renderer2D::ClearDepth(float value)
	{
		glClearDepthf(value);
	}

	void Renderer2D::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	GLFWwindow* Renderer2D::GetGLFWwindow()
	{
		return m_context.window;
	}

	bool Renderer2D::GetShouldGLFWWindowClose()
	{
		return glfwWindowShouldClose(m_context.window);
	}

	void Renderer2D::SignalGLFWWindowClose()
	{
		glfwSetWindowShouldClose(m_context.window, true);
	}

	void Renderer2D::SignalGLFWWindowFocus()
	{
		glfwFocusWindow(m_context.window);
		glfwRequestWindowAttention(m_context.window);
	}

	void Renderer2D::SetWindowTitle(const std::string& title)
	{
		m_windowTitle = title;
		glfwSetWindowTitle(m_context.window, m_windowTitle.c_str());
	}

	void Renderer2D::KeyPressCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			// call all key down events
		}
		else if (action == GLFW_RELEASE)
		{
			// call all key up events
		}
	}

	void Renderer2D::MouseButtonPressCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_RELEASE && g_isMouseHeld)
		{
			g_isMouseHeld = false;
		}
		else if (action == GLFW_PRESS)
		{
			g_isMouseHeld = true;
		}

		auto const evt = MouseEvent{ static_cast<MouseButton>(button), static_cast<MouseAction>(action), g_cursorPos };

		// call all mouse button events
	}

	void Renderer2D::CursorPosCallback(GLFWwindow* window, double x, double y)
	{
		MouseEvent evt;
		if (g_isMouseHeld)
		{
			evt.action = MouseAction::REPEAT;
		}
		evt.cursorPos.x = x;
		evt.cursorPos.y = y;
		g_cursorPos.x = x;
		g_cursorPos.y = y;

		// call all mouse move events
	}

	void Renderer2D::ScrollCallback(GLFWwindow* window, double x, double y)
	{
		// call all mouse scroll events
	}

	void Renderer2D::WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		m_context.width = width;
		m_context.height = height;

		// call all window resize events
	}
}