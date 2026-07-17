#include "sceneManager/sceneManager.h"
#include <imgui.h>

class TestScene : Scene
{
public:
	TestScene() : Scene(0)
	{ }

	void init() override
	{}

	void update() override
	{
		ImGui::ShowDemoWindow();
	}

	void exit()
	{}
};
TestScene _instance; // create a instance to register scene
