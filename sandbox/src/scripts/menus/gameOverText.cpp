/*! \file pauseMenuText.cpp
*
* \brief A script for all the interactable text in the game over screen
*
* \author: Jamie Hall
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/gameOverText.h"
#include "loaders/sandboxLoader.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "scripts/layerControl.h"
#include "scripts/gameTimer.h"
#include "scripts/UI/startingMessage.h"

//! loadScene()
/*!
\param sceneName a const std::string& - The scene name
*/
void GameOverText::loadScene(const std::string& sceneName, const std::string& sceneFolderPath)
{
	bool inspector = getParent()->getParentScene()->getInspectorEnabled();
	if (SceneManager::getScene(sceneName))
	{
		SceneManager::setActiveScene(sceneName, true);
		SceneManager::getActiveScene()->enableInspector(inspector);
	}
	else
	{
		SandboxLoader::load(sceneName, sceneFolderPath);
		SceneManager::setActiveScene(sceneName, true);
		SceneManager::getActiveScene()->enableInspector(inspector);
	}
}


//! GameOverText()
GameOverText::GameOverText()
{
	m_text = nullptr;
	setScriptName("GameOverText");
}

//! ~GameOverText()
GameOverText::~GameOverText()
{
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void GameOverText::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
{
	bool containsMouse = getParent()->containsPoint(InputPoller::getMousePosition());

	if (m_text)
	{
		if (containsMouse && e.getButton() == Mouse::LEFTBUTTON)
			m_text->setColour({ 1.f, 0.7f, 0.f, 1.f });
	}
}

//! onMouseRelease()
/*!
\param e a MouseReleasedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void GameOverText::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	std::string entityName = getParent()->getName();
	bool containsMouse = getParent()->containsPoint(mousePosition);
	std::string sceneName = getParent()->getParentScene()->getName();
	Scene* scene = getParent()->getParentScene();

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("click")->getComponent<StudioSound>()->playSound();

		if (entityName == "GameOver_Retry")
		{
			if(scene->getName() == "level1")
				loadScene("level1-retry", "assets/scenes/level1/");
			else
				loadScene("level1", "assets/scenes/level1/");

			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
			static_cast<StartingMessage*>(SceneManager::getActiveScene()->getEntity("StartingMessageEnt1")->getComponent<NativeScript>())->play();
			return;
		}
		else if (entityName == "GameOver_Quit")
		{
			LayerControl::hide("GameOver", scene);
			LayerControl::deactivate("GameOver", scene);
			loadScene("mainMenu", "assets/scenes/mainMenu/");
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
			SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("menu_music")->getComponent<StudioSound>()->playSound();
		}
	}

	if (m_text)
	{
		if (!containsMouse && e.getButton() == Mouse::LEFTBUTTON)
			m_text->setColour({ 1.f, 1.f, 1.f, 1.f });
	}
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void GameOverText::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	if (!m_text) m_text = getParent()->getComponent<Text>();
	bool containsMouse = getParent()->containsPoint(e.getPos());

	if (m_text)
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 0.5f, 0.85f, 1.f });
		else if (!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 1.0f, 1.f, 1.f });
	}
}