#include "sceneManager/sceneManager.h"
#include "renderer2D/logger.hpp"
#include <imgui.h>

class TestScene : Scene
{
public:
	TestScene() : Scene(0)
	{ }

	void init() override
	{
		LOG_INFO("{} - Initialising scene", __func__);
	}

	void update() override
	{
		ImGui::ShowDemoWindow();
	}

	void exit()
	{
		LOG_INFO("{} - Exiting scene", __func__);
	}
};
TestScene _instance; // create a instance to register scene
