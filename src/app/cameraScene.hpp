#include "app/sceneManager.h"
#include "app/globalCamera.h"
#include "app/drawTool.hpp"
#include "renderer2D/logger.hpp"

class CamScene : public Scene
{
public:
	CamScene(const char* name, int orderPriority = 0)
		: m_sceneName{ name }, Scene(orderPriority)
	{ }

	void init() override final
	{
		LOG_INFO("{} - Initialising scene {}", __func__, m_sceneName.c_str());

		m_moveUp = false;
		m_moveDown = false;
		m_moveLeft = false;
		m_moveRight = false;

		m_previousTime = glfwGetTime();

		OnInit();
	}

	void update() override final
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

		OnUpdate();
	}

	void exit() override
	{
		LOG_INFO("{} - Exiting scene", __func__);

		m_moveUp = false;
		m_moveDown = false;
		m_moveLeft = false;
		m_moveRight = false;

		OnExit();
	}

	void onKeyDown(
		const R2D::KeyEvent& event
	) override final
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

		OnKeyDown(event);
	}

	void onKeyUp(
		const R2D::KeyEvent& event
	) override final
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

		OnKeyUp(event);
	}

	void onMouseScroll(
		const R2D::MouseEvent& event
	) override final
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

		OnMouseScroll(event);
	}

protected:
	virtual void OnInit() {};
	virtual void OnUpdate() {};
	virtual void OnExit() {};
	virtual void OnKeyDown(const R2D::KeyEvent& event) {};
	virtual void OnKeyUp(const R2D::KeyEvent& event) {};
	virtual void OnMouseScroll(const R2D::MouseEvent& event) {};

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

	std::string m_sceneName;
};