#include "app/sceneManager.h"
#include "app/globalCamera.h"
#include "app/drawTool.hpp"
#include "renderer2D/logger.hpp"

#include <imgui.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

class TestScene : public Scene
{
public:
	TestScene()
		: Scene(0)
	{
	}

	void draw()
	{
		// World-space grid: makes camera movement and zoom easy to see.
		constexpr float gridExtent = 1000.0f;
		constexpr float gridSpacing = 100.0f;

		const glm::vec4 minorGridColour{
			0.18f, 0.18f, 0.22f, 1.0f
		};

		for (
			float coordinate = -gridExtent;
			coordinate <= gridExtent;
			coordinate += gridSpacing
			)
		{
			DrawTool::DrawLine(
				{ coordinate, -gridExtent },
				{ coordinate, gridExtent },
				minorGridColour,
				1.0f
			);

			DrawTool::DrawLine(
				{ -gridExtent, coordinate },
				{ gridExtent, coordinate },
				minorGridColour,
				1.0f
			);
		}

		// World axes.
		DrawTool::DrawLine(
			{ -gridExtent, 0.0f },
			{ gridExtent, 0.0f },
			{ 0.75f, 0.15f, 0.15f, 1.0f },
			2.0f
		);

		DrawTool::DrawLine(
			{ 0.0f, -gridExtent },
			{ 0.0f, gridExtent },
			{ 0.15f, 0.75f, 0.15f, 1.0f },
			2.0f
		);

		// Triangle vertices.
		const glm::vec2 pointA{ -200.0f, -140.0f };
		const glm::vec2 pointB{ 200.0f, -140.0f };
		const glm::vec2 pointC{ 0.0f,  220.0f };

		// Triangle edges with different widths and colours.
		DrawTool::DrawLine(
			pointA,
			pointB,
			{ 1.0f, 0.25f, 0.20f, 1.0f },
			4.0f
		);

		DrawTool::DrawLine(
			pointB,
			pointC,
			{ 0.20f, 1.0f, 0.35f, 1.0f },
			8.0f
		);

		DrawTool::DrawLine(
			pointC,
			pointA,
			{ 0.25f, 0.45f, 1.0f, 1.0f },
			12.0f
		);

		// Filled circles mark the vertices.
		DrawTool::DrawCircleFilled(
			pointA,
			20.0f,
			{ 1.0f, 0.25f, 0.20f, 1.0f }
		);

		DrawTool::DrawCircleFilled(
			pointB,
			28.0f,
			{ 0.20f, 1.0f, 0.35f, 1.0f }
		);

		DrawTool::DrawCircleFilled(
			pointC,
			36.0f,
			{ 0.25f, 0.45f, 1.0f, 1.0f }
		);

		// Wireframe circles around each vertex.
		DrawTool::DrawCircleWireframe(
			pointA,
			40.0f,
			{ 1.0f, 0.75f, 0.20f, 1.0f },
			2.0f
		);

		DrawTool::DrawCircleWireframe(
			pointB,
			52.0f,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			4.0f
		);

		DrawTool::DrawCircleWireframe(
			pointC,
			65.0f,
			{ 1.0f, 0.25f, 1.0f, 1.0f },
			7.0f
		);

		// Nested circles at the world origin.
		DrawTool::DrawCircleFilled(
			{ 0.0f, 0.0f },
			12.0f,
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		);

		DrawTool::DrawCircleWireframe(
			{ 0.0f, 0.0f },
			75.0f,
			{ 0.25f, 0.85f, 1.0f, 1.0f },
			3.0f
		);

		DrawTool::DrawCircleWireframe(
			{ 0.0f, 0.0f },
			110.0f,
			{ 1.0f, 0.65f, 0.15f, 1.0f },
			6.0f
		);

		const auto& camera = GlobalCamera::GetConst();

		ImGui::Begin("Draw Tool Test");
		ImGui::Text("WASD: move camera");
		ImGui::Text("Mouse wheel: zoom");
		ImGui::Separator();
		ImGui::Text(
			"Camera position: %.1f, %.1f",
			camera.getPosition().x,
			camera.getPosition().y
		);
		ImGui::Text("Camera zoom: %.2f", camera.getZoom());
		ImGui::End();

		ImGui::ShowDemoWindow();
	}

	void init() override
	{
		LOG_INFO("{} - Initialising scene", __func__);

		m_moveUp = false;
		m_moveDown = false;
		m_moveLeft = false;
		m_moveRight = false;

		m_previousTime = glfwGetTime();

		m_onKeyUpHdl = R2D::Renderer2D::AddKeyUpCallback([this](const R2D::KeyEvent& event) { onKeyUp(event); });
		m_onKeyDownHdl = R2D::Renderer2D::AddKeyDownCallback([this](const R2D::KeyEvent& event) { onKeyDown(event); });
		m_onMouseScrollHdl = R2D::Renderer2D::AddMouseScrollCallback([this](const R2D::MouseEvent& event) { onMouseScroll(event); });
	}

	void update() override
	{
		const double currentTime = glfwGetTime();

		const float deltaTime = static_cast<float>(
			currentTime - m_previousTime
			);

		m_previousTime = currentTime;

		const float clampedDeltaTime =
			glm::min(deltaTime, 0.1f);

		glm::vec2 movement(0.0f);

		if (m_moveLeft)
		{
			movement.x -= 1.0f;
		}

		if (m_moveRight)
		{
			movement.x += 1.0f;
		}

		if (m_moveDown)
		{
			movement.y -= 1.0f;
		}

		if (m_moveUp)
		{
			movement.y += 1.0f;
		}

		const float movementLength =
			glm::length(movement);

		if (movementLength > 0.0f)
		{
			movement /= movementLength;

			auto& camera = GlobalCamera::Get();

			camera.pan(
				movement *
				m_cameraMoveSpeed *
				clampedDeltaTime /
				camera.getZoom()
			);
		}

		draw();
	}

	void exit() override
	{
		LOG_INFO("{} - Exiting scene", __func__);

		m_moveUp = false;
		m_moveDown = false;
		m_moveLeft = false;
		m_moveRight = false;

		R2D::Renderer2D::RemoveCallback(m_onKeyUpHdl);
		R2D::Renderer2D::RemoveCallback(m_onKeyDownHdl);
		R2D::Renderer2D::RemoveCallback(m_onMouseScrollHdl);
	}

	void onKeyDown(
		const R2D::KeyEvent& event
	)
	{
		switch (event.key)
		{
		case GLFW_KEY_W:
			m_moveUp = true;
			break;

		case GLFW_KEY_S:
			m_moveDown = true;
			break;

		case GLFW_KEY_A:
			m_moveLeft = true;
			break;

		case GLFW_KEY_D:
			m_moveRight = true;
			break;

		default:
			break;
		}
	}

	void onKeyUp(
		const R2D::KeyEvent& event
	)
	{
		switch (event.key)
		{
		case GLFW_KEY_W:
			m_moveUp = false;
			break;

		case GLFW_KEY_S:
			m_moveDown = false;
			break;

		case GLFW_KEY_A:
			m_moveLeft = false;
			break;

		case GLFW_KEY_D:
			m_moveRight = false;
			break;

		default:
			break;
		}
	}

	void onMouseScroll(
		const R2D::MouseEvent& event
	)
	{
		auto& camera = GlobalCamera::Get();

		// Multiplicative zoom feels consistent across the entire range.
		const float zoomMultiplier = std::pow(
			m_zoomStep,
			event.scroll.y
		);

		const float newZoom = glm::clamp(
			camera.getZoom() * zoomMultiplier,
			m_minZoom,
			m_maxZoom
		);

		camera.setZoom(newZoom);
	}

private:
	bool m_moveUp{ false };
	bool m_moveDown{ false };
	bool m_moveLeft{ false };
	bool m_moveRight{ false };

	double m_previousTime{ 0.0 };

	// World units per second.
	float m_cameraMoveSpeed{ 400.0f };

	float m_minZoom{ 0.1f };
	float m_maxZoom{ 10.0f };
	float m_zoomStep{ 1.15f };

	R2D::EventHandle m_onKeyUpHdl;
	R2D::EventHandle m_onKeyDownHdl;
	R2D::EventHandle m_onMouseScrollHdl;
};

TestScene _instance;