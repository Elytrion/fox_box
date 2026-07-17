#pragma once
#include <vector>

class Scene
{
public:
	Scene();
	~Scene();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;
};

class SceneManager
{
public:
	static SceneManager* getInstance()
	{
		if (!m_instance)
			m_instance = new SceneManager();
		return m_instance;
	}

	void registerScene(Scene* scene);
	void unregisterScene(Scene* scene);
	void setNextScene();
	void setCurrentScene(int index);
	void updateCurrentScene();
	void cleanup()
	{
		for (auto scene : m_scenes)
		{
			if (!scene)
				continue;
			scene->exit();
		}
		m_scenes.clear();
	}

private:
	static SceneManager* m_instance;
	int m_currentSceneIndex{ 0 };
	std::vector<Scene*> m_scenes{};

};
