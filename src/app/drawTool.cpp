#include "drawTool.hpp"
#include "shaders/drawToolShaderSrc.hpp"
#include "renderer2D/inc/objects/shaderProgram.hpp"
#include "renderer2D/inc/utils/shaderManager.hpp"
#include "renderer2D/inc/objects/varray.hpp"
#include "renderer2D/inc/renderer2D.h"

namespace
{
	R2D::ShaderProgram g_CircleShader;
	R2D::ShaderProgram g_LineShader;

	R2D::VertexArray g_DefaultVAO;

	R2D::Buffer g_QuadBuffer;
	R2D::Buffer g_PositionBuffer;
	R2D::Buffer g_ColourBuffer;

	static void UpdateSharedInstanceBuffers(
		const glm::vec2& position,
		const glm::vec4& colour
	)
	{
		g_PositionBuffer.updateElement(
			&position,
			1
		);

		g_ColourBuffer.updateElement(
			&colour,
			1
		);
	}
}

void DrawTool::Init()
{
	g_CircleShader.setProgram(R2D::ShaderManager::createShaderProgram(&circle_vs, &circle_fs));
	g_LineShader.setProgram(R2D::ShaderManager::createShaderProgram(&line_vs, &line_fs));

	const glm::vec2 quadVertices[] =
	{
		{-1.0f, -1.0f},
		{ 1.0f, -1.0f},
		{ 1.0f,  1.0f},

		{-1.0f, -1.0f},
		{ 1.0f,  1.0f},
		{-1.0f,  1.0f}
	};

	g_QuadBuffer.alloc(
		R2D::BUFFER_USAGE::STATIC_DRAW,
		6,
		R2D::LAYOUTS::VEC2LAYOUT,
		quadVertices
	);

	g_PositionBuffer.alloc(
		R2D::BUFFER_USAGE::DYNAMIC_DRAW,
		1,
		R2D::LAYOUTS::VEC2LAYOUT
	);

	g_ColourBuffer.alloc(
		R2D::BUFFER_USAGE::DYNAMIC_DRAW,
		1,
		R2D::LAYOUTS::VEC4LAYOUT
	);


	g_DefaultVAO
		.setVertexBuffers(
			{
				&g_QuadBuffer
			}
		)
		.setInstanceBuffers(
			{
				&g_PositionBuffer,
				&g_ColourBuffer
			}
		)
		.build();

}

void DrawTool::Cleanup()
{
	g_DefaultVAO.reset();

	g_QuadBuffer.destroy();
	g_PositionBuffer.destroy();
	g_ColourBuffer.destroy();

	g_CircleShader.destroy();
	g_LineShader.destroy();
}

void DrawTool::DrawLine(
	glm::vec2 const& start,
	glm::vec2 const& end,
	glm::vec4 const& colour,
	float            width
)
{
	const glm::vec2 delta = end - start;
	const float length = glm::length(delta);

	if (length <= 0.0001f || width <= 0.0f)
	{
		return;
	}

	const glm::vec2 centre = (start + end) * 0.5f;
	const glm::vec2 direction = delta / length;
	const float halfLength = length * 0.5f;

	g_PositionBuffer.updateElement(
		&centre,
		1
	);

	g_ColourBuffer.updateElement(
		&colour,
		1
	);

	g_LineShader.setUniform(
		"u_direction",
		R2D::UNIFORM_TYPE::VEC2,
		&direction
	);

	g_LineShader.setUniform(
		"u_halfLength",
		R2D::UNIFORM_TYPE::FLOAT,
		&halfLength
	);

	g_LineShader.setUniform(
		"u_width",
		R2D::UNIFORM_TYPE::FLOAT,
		&width
	);

	g_LineShader.use();
	g_DefaultVAO.bind();

	R2D::Renderer2D::drawArraysInstanced(
		6,
		0,
		1,
		R2D::DRAW_MODE::TRIANGLES
	);	
}

void DrawTool::DrawCircleFilled(
	glm::vec2 const& position,
	float            radius,
	glm::vec4 const& colour
)
{
	if (radius <= 0.0f)
		return;

	UpdateSharedInstanceBuffers(position, colour);

	const int wireframe = 0;
	const float lineWidth = 1.0f;

	g_CircleShader.setUniform(
		"u_radius",
		R2D::UNIFORM_TYPE::FLOAT,
		&radius
	);

	g_CircleShader.setUniform(
		"u_wireframe",
		R2D::UNIFORM_TYPE::INT,
		&wireframe
	);

	g_CircleShader.setUniform(
		"u_lineWidth",
		R2D::UNIFORM_TYPE::FLOAT,
		&lineWidth
	);

	g_CircleShader.use();
	g_DefaultVAO.bind();

	R2D::Renderer2D::drawArraysInstanced(
		6,
		0,
		1,
		R2D::DRAW_MODE::TRIANGLES
	);
}

void DrawTool::DrawCircleWireframe(
	glm::vec2 const& position,
	float            radius,
	glm::vec4 const& colour,
	float            line_width
)
{
	if (radius <= 0.0f)
		return;

	UpdateSharedInstanceBuffers(position, colour);

	const int wireframe = 1;
	const float lineWidth = glm::max(line_width, 1.0f);

	g_CircleShader.setUniform(
		"u_radius",
		R2D::UNIFORM_TYPE::FLOAT,
		&radius
	);

	g_CircleShader.setUniform(
		"u_wireframe",
		R2D::UNIFORM_TYPE::INT,
		&wireframe
	);

	g_CircleShader.setUniform(
		"u_lineWidth",
		R2D::UNIFORM_TYPE::FLOAT,
		&lineWidth
	);

	g_CircleShader.use();
	g_DefaultVAO.bind();

	R2D::Renderer2D::drawArraysInstanced(
		6,
		0,
		1,
		R2D::DRAW_MODE::TRIANGLES
	);
}

