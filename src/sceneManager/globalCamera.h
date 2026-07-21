#pragma once

#include "renderer2D/inc/objects/camera2D.hpp"

struct GlobalCamera
{
	static void Init();
	static void CleanUp();

	static void UpdateUniformBuffer();
	static void BindUniformBuffer();

	static R2D::Camera2D& Get();
	static const R2D::Camera2D& GetConst();
};