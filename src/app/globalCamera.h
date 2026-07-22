#pragma once

#include "renderer2D/inc/objects/camera2D.hpp"

struct GlobalCamera
{
	static void Init();
	static void CleanUp();

	static void UpdateUniformBuffer();
	static void BindUniformBuffer();

	static glm::vec2 ScreenToWorld(glm::vec2 screenPosition);
	static glm::vec2 WorldToScreen(glm::vec2 worldPosition);

	static R2D::Camera2D& Get();
	static const R2D::Camera2D& GetConst();
};
