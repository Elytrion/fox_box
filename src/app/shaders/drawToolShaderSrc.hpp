#pragma once

static const char* line_vs = R"(
#version 460 core

layout(location = 0) in vec2 a_localPosition;
layout(location = 1) in vec2 a_position;
layout(location = 2) in vec4 a_colour;

layout(std140, binding = 0) uniform CameraData
{
	mat4 u_viewProjection;
	mat4 u_view;
	mat4 u_projection;
	vec4 u_positionZoom;
};

uniform vec2 u_direction;
uniform float u_halfLength;
uniform float u_width;

layout(location = 0) out vec2 v_localPosition;
layout(location = 1) out vec4 v_colour;

void main()
{
	vec2 normal = vec2(
		-u_direction.y,
		 u_direction.x
	);

	// Treat line width as screen pixels.
	float zoom = max(u_positionZoom.w, 0.0001);
	float halfWidthWorld = 0.5 * u_width / zoom;

	vec2 worldPosition =
		a_position
		+ u_direction * a_localPosition.x * u_halfLength
		+ normal * a_localPosition.y * halfWidthWorld;

	v_localPosition = a_localPosition;
	v_colour = a_colour;

	gl_Position =
		u_viewProjection * vec4(worldPosition, 0.0, 1.0);
}
)";

static const char* line_fs = R"(
#version 460 core

layout(location = 0) in vec2 v_localPosition;
layout(location = 1) in vec4 v_colour;

layout(location = 0) out vec4 o_colour;

void main()
{
	o_colour = v_colour;
}
)";

static const char* circle_vs = R"(
#version 460 core

layout(location = 0) in vec2 a_localPosition;
layout(location = 1) in vec2 a_position;
layout(location = 2) in vec4 a_colour;

layout(std140, binding = 0) uniform CameraData
{
	mat4 u_viewProjection;
	mat4 u_view;
	mat4 u_projection;
	vec4 u_positionZoom;
};

uniform float u_radius;

layout(location = 0) out vec2 v_localPosition;
layout(location = 1) out vec4 v_colour;

void main()
{
	v_localPosition = a_localPosition;
	v_colour = a_colour;

	vec2 worldPosition =
		a_position + a_localPosition * u_radius;

	gl_Position =
		u_viewProjection * vec4(worldPosition, 0.0, 1.0);
}
)";

static const char* circle_fs = R"(
#version 460 core

layout(location = 0) in vec2 v_localPosition;
layout(location = 1) in vec4 v_colour;

layout(location = 0) out vec4 o_colour;

uniform int u_wireframe;
uniform float u_lineWidth;

void main()
{
	float distanceFromCentre = length(v_localPosition);
	float pixelWidth = fwidth(distanceFromCentre);

	float outerCoverage = 1.0 - smoothstep(
		1.0 - pixelWidth,
		1.0,
		distanceFromCentre
	);

	float coverage = outerCoverage;

	if (u_wireframe != 0)
	{
		float innerRadius =
			1.0 - max(u_lineWidth, 1.0) * pixelWidth;

		float innerCoverage = smoothstep(
			innerRadius - pixelWidth,
			innerRadius + pixelWidth,
			distanceFromCentre
		);

		coverage *= innerCoverage;
	}

	if (coverage <= 0.0)
	{
		discard;
	}

	o_colour = vec4(
		v_colour.rgb,
		v_colour.a * coverage
	);
}
)";

