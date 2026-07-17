#include "sceneManager.h"

Scene::Scene()
{
	SceneManager::getInstance()->registerScene(this);
}
Scene::~Scene()
{
	SceneManager::getInstance()->unregisterScene(this);
}

SceneManager* SceneManager::m_instance = nullptr;

void SceneManager::registerScene(Scene* scene)
{
	m_scenes.push_back(scene);
}

void SceneManager::unregisterScene(Scene* scene)
{
	if (!m_scenes.size())
		return;

	auto it = std::find(m_scenes.begin(), m_scenes.end(), scene);
	if (it != m_scenes.end()) {
		m_scenes.erase(it);
	}
}

void SceneManager::setCurrentScene(int index)
{
	if (!m_scenes.size())
		return;

	Scene* currScene = m_scenes[m_currentSceneIndex];
	if (currScene)
		currScene->exit();
	m_currentSceneIndex = index;
	Scene* newScene = m_scenes[m_currentSceneIndex];
	if (newScene)
		newScene->init();
}
void SceneManager::setNextScene()
{
	if (!m_scenes.size())
		return;

	int nxtIndex = (m_currentSceneIndex + 1) % m_scenes.size();
	setCurrentScene(nxtIndex);
}
void SceneManager::updateCurrentScene()
{
	if (!m_scenes.size())
		return;

	Scene* currScene = m_scenes[m_currentSceneIndex];
	if (!currScene)
	{
		return;
	}
	currScene->update();
}