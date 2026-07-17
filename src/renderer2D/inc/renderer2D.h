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

	class Renderer2D
	{
	public:
		using KeyCallbackFnType = std::function<void(int, int, int)>;
		using MouseButtonCallbackFnType = std::function<void(int, int, int)>;
		using CursorPosCallbackFnType = std::function<void(float, float)>;
		using ScrollCallbackFnType = std::function<void(double, double)>;
		using WindowSizeCallbackFnType = std::function<void(int, int)>;
		using WindowCloseCallbackFnType = std::function<void()>;

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

	private:
		struct RenderContext;
		static RenderContext m_context;

		static std::string m_windowTitle;


		static BufferHandle m_bufferCounter;


	};
}