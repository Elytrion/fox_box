#pragma once

#include "glm/glm.hpp"
#include <vector>

struct DrawTool
{
	static void Init();
	static void Cleanup();

	static void DrawLine(
		glm::vec2 const& start,
		glm::vec2 const& end,
		glm::vec4 const& colour = glm::vec4(1.f, 1.f, 1.f, 1.f),
		float            width = 1.f
	);

	static void DrawCircleFilled(
		glm::vec2 const& position,
		float            radius,
		glm::vec4 const& colour = glm::vec4(1.f, 1.f, 1.f, 1.f)
	);

	static void DrawCircleWireframe(
		glm::vec2 const& position,
		float            radius,
		glm::vec4 const& colour = glm::vec4(1.f, 1.f, 1.f, 1.f),
		float            line_width = 1.f
	);

};
