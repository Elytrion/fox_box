#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace R2D
{
	class Camera2D
	{
	public:
		void setViewport(uint32_t width, uint32_t height)
		{
			m_viewport = { width, height };
		}

		void setPosition(glm::vec2 position)
		{
			m_position = position;
		}

		void pan(glm::vec2 delta)
		{
			m_position += delta;
		}

		void setZoom(float zoom)
		{
			m_zoom = glm::max(zoom, 0.01f);
		}

		void zoom(float amount)
		{
			setZoom(m_zoom * amount);
		}

		glm::mat4 getViewMatrix() const
		{
			return glm::translate(glm::mat4(1.f), glm::vec3(-m_position, 0.f));
		}

		glm::mat4 getProjectionMatrix() const
		{
			const float halfWidth = static_cast<float>(m_viewport.x) * 0.5f / m_zoom;
			const float halfHeight = static_cast<float>(m_viewport.y) * 0.5f / m_zoom;

			return glm::ortho(
				-halfWidth,
				halfWidth,
				-halfHeight,
				halfHeight,
				-1.f,
				1.f
			);
		}

		glm::mat4 getViewProjectionMatrix() const
		{
			return getProjectionMatrix() * getViewMatrix();
		}

		glm::vec2 screenDeltaToWorldDelta(glm::vec2 screenDelta) const
		{
			return screenDelta / m_zoom;
		}

		glm::vec2 getPosition() const { return m_position; }
		float getZoom() const { return m_zoom; }

	private:
		glm::vec2 m_position{ 0.f, 0.f };
		glm::uvec2 m_viewport{ 1280, 720 };
		float m_zoom{ 1.f };
	};
}