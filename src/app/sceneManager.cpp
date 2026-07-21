#include "sceneManager.h"
#include <algorithm>

Scene::Scene(int orderPriority) : m_sceneOrderPriority{ orderPriority }
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
	sortScenes();
}

void SceneManager::unregisterScene(Scene* scene)
{
	if (!m_scenes.size())
		return;

	auto it = std::find(m_scenes.begin(), m_scenes.end(), scene);
	if (it != m_scenes.end()) {
		m_scenes.erase(it);
	}

	sortScenes();
}

void SceneManager::setCurrentScene(int index)
{
	if (!m_scenes.size())
		return;

	if (index < 0 || static_cast<std::size_t>(index) >= m_scenes.size())
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

void SceneManager::changeSceneOrderPriority(Scene* scene, int order)
{
	scene->m_sceneOrderPriority = order;
	sortScenes();
}

Scene* SceneManager::getCurrentScene()
{
	if (
		m_currentSceneIndex < 0 ||
		static_cast<std::size_t>(m_currentSceneIndex) >= m_scenes.size()
		)
	{
		return nullptr;
	}
	
	return m_scenes[m_currentSceneIndex];
}

void SceneManager::sortScenes()
{	
	Scene* currentScene = nullptr;
	
	if (m_currentSceneIndex >= 0 && static_cast<std::size_t>(m_currentSceneIndex) < m_scenes.size())
	{
		currentScene = m_scenes[m_currentSceneIndex];
	}
	
	std::stable_sort(
		m_scenes.begin(),
		m_scenes.end(),
		[](const Scene* lhs, const Scene* rhs)
		{
			return lhs->m_sceneOrderPriority < rhs->m_sceneOrderPriority;
		});
	
	if (currentScene) // reset m_currentSceneIndex to new ordering
	{
		const auto it = std::find(
			m_scenes.begin(),
			m_scenes.end(),
			currentScene);

		if (it != m_scenes.end())
		{
			m_currentSceneIndex = static_cast<int>(std::distance(m_scenes.begin(), it));
		}
	}
}

void SceneManager::setupInputCallbacks()
{
	m_keyDownCallbackHandle =
		R2D::Renderer2D::AddKeyDownCallback(
			[this](const R2D::KeyEvent& event)
			{
				if (Scene* scene = getCurrentScene())
					scene->onKeyDown(event);
			}
		);

	m_keyUpCallbackHandle =
		R2D::Renderer2D::AddKeyUpCallback(
			[this](const R2D::KeyEvent& event)
			{
				if (Scene* scene = getCurrentScene())
					scene->onKeyUp(event);
			}
		);

	m_mouseButtonCallbackHandle =
		R2D::Renderer2D::AddMouseButtonCallback(
			[this](const R2D::MouseEvent& event)
			{
				if (Scene* scene = getCurrentScene())
					scene->onMouseButton(event);
			}
		);

	m_mouseMoveCallbackHandle =
		R2D::Renderer2D::AddMouseMoveCallback(
			[this](const R2D::MouseEvent& event)
			{
				if (Scene* scene = getCurrentScene())
					scene->onMouseMove(event);
			}
		);

	m_mouseScrollCallbackHandle =
		R2D::Renderer2D::AddMouseScrollCallback(
			[this](const R2D::MouseEvent& event)
			{
				if (Scene* scene = getCurrentScene())
					scene->onMouseScroll(event);
			}
		);
}

void SceneManager::cleanupInputCallbacks()
{
	R2D::Renderer2D::RemoveCallback(m_keyDownCallbackHandle);
	R2D::Renderer2D::RemoveCallback(m_keyUpCallbackHandle);
	R2D::Renderer2D::RemoveCallback(m_mouseButtonCallbackHandle);
	R2D::Renderer2D::RemoveCallback(m_mouseMoveCallbackHandle);
	R2D::Renderer2D::RemoveCallback(m_mouseScrollCallbackHandle);

	m_keyDownCallbackHandle = 0;
	m_keyUpCallbackHandle = 0;
	m_mouseButtonCallbackHandle = 0;
	m_mouseMoveCallbackHandle = 0;
	m_mouseScrollCallbackHandle = 0;
}
