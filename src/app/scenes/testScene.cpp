#include "app/cameraScene.hpp"

#include <imgui.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

class TestScene : public CamScene
{
public:
	TestScene()
		: CamScene("Test Scene", 0)
	{
	}

	void OnUpdate() override
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
};

TestScene _instance;