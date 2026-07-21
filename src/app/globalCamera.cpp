#include "globalCamera.h"
#include "renderer2D/inc/objects/buffer.hpp"
#include "renderer2D/inc/utils/bufferLayout.hpp"

/* 
	NOTE: camera uniform to use in shader!
	layout(std140, binding = 0) uniform CameraData
	{
		mat4 u_viewProjection;
		mat4 u_view;
		mat4 u_projection;
		vec4 u_positionZoom;
	};
*/
namespace
{
	constexpr uint32_t CAMERA_UBO_BINDING = 0;

	struct CameraUniform
	{
		glm::mat4 viewProjection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 projection{ 1.f };
		glm::vec4 positionZoom{ 0.f, 0.f, 0.f, 1.f };
	};

	static R2D::Camera2D g_camera;
	static R2D::Buffer g_cameraUniformBuffer;
}

using namespace R2D;

void GlobalCamera::Init()
{
	CameraUniform data{};
	g_cameraUniformBuffer.alloc(R2D::BUFFER_USAGE::DYNAMIC_DRAW, sizeof(CameraUniform), &data);
}
void GlobalCamera::CleanUp()
{
	g_cameraUniformBuffer.destroy();
}

void GlobalCamera::UpdateUniformBuffer()
{
	CameraUniform data{};

	data.view = g_camera.getViewMatrix();
	data.projection = g_camera.getProjectionMatrix();
	data.viewProjection =
		g_camera.getViewProjectionMatrix();

	data.positionZoom = glm::vec4(
		g_camera.getPosition(),
		0.0f,
		g_camera.getZoom()
	);

	g_cameraUniformBuffer.update(
		&data,
		sizeof(CameraUniform)
	);
	
}
void GlobalCamera::BindUniformBuffer()
{
	g_cameraUniformBuffer.bindAsUniform(CAMERA_UBO_BINDING);
}

R2D::Camera2D& GlobalCamera::Get()
{
	return g_camera;
}
const R2D::Camera2D& GlobalCamera::GetConst()
{
	return g_camera;
}