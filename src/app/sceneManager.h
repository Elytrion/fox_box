#pragma once
#include <vector>
#include "renderer2D/inc/renderer2D.h"

class Scene
{
public:
	Scene(int orderPriority = -1);
	~Scene();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;



private:
	friend class SceneManager;
	int m_sceneOrderPriority{ -1 };
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
	void changeSceneOrderPriority(Scene* scene, int order);
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
	void sortScenes();
	static SceneManager* m_instance;
	int m_currentSceneIndex{ 0 };
	std::vector<Scene*> m_scenes{};
};
