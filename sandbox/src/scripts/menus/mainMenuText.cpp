/*! \file mainMenuText.cpp
*
* \brief A script for all the interactable text in the main menu
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/mainMenuText.h"
#include "scripts/layerControl.h"
#include "scripts/UI/startingMessage.h"
#include "loaders/sandboxLoader.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"

//! loadScene()
/*!
\param sceneName a const std::string& - The scene name
*/
void MainMenuText::loadScene(const std::string& sceneName)
{
	if (SceneManager::getScene(sceneName))
	{
		bool inspector = SceneManager::getActiveScene()->getInspectorEnabled();
		SceneManager::setActiveScene(sceneName, true);
		SceneManager::getActiveScene()->enableInspector(inspector);
	}
	else
	{
		SandboxLoader::load(sceneName, "assets/scenes/" + sceneName + "/");
		bool inspector = SceneManager::getActiveScene()->getInspectorEnabled();
		SceneManager::setActiveScene(sceneName, true);
		SceneManager::getActiveScene()->enableInspector(inspector);
	}
}

//! MainMenuText()
MainMenuText::MainMenuText()
{
	m_text = nullptr;
	setScriptName("MainMenuText");
}

//! ~MainMenuText()
MainMenuText::~MainMenuText()
{
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void MainMenuText::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
{
	bool containsMouse = getParent()->containsPoint(InputPoller::getMousePosition());

	if (m_text)
	{
		if (containsMouse && e.getButton() == Mouse::LEFTBUTTON)
		{
			m_text->setColour({ 1.f, 0.7f, 0.f, 1.f });		
		}
	}
}

//! onMouseRelease()
/*!
\param e a MouseReleasedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void MainMenuText::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	std::string entityName = getParent()->getName();
	bool containsMouse = getParent()->containsPoint(mousePosition);
	std::string sceneName = getParent()->getParentScene()->getName();
	Scene* scene = getParent()->getParentScene();

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("click")->getComponent<StudioSound>()->playSound();
		SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("menu_music")->getComponent<StudioSound>()->stopSound();

		if (entityName == "Level1")
		{
			loadScene("level1");
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
			static_cast<StartingMessage*>(SceneManager::getActiveScene()->getEntity("StartingMessageEnt1")->getComponent<NativeScript>())->play();
		}
		else if (getParent()->getName() == "MM_Settings")
		{
			LayerControl::hide("UI", scene);
			LayerControl::deactivate("UI", scene);
			LayerControl::show("Settings", scene);
			LayerControl::activate("Settings", scene);
			SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("elevator_music")->getComponent<StudioSound>()->playSound();
		}
		else if (getParent()->getName() == "Credits")
		{
			LayerControl::hide("UI", scene);
			LayerControl::deactivate("UI", scene);
			LayerControl::show("CreditsLayer", scene);
			LayerControl::activate("CreditsLayer", scene);
		}
		else if (getParent()->getName() == "MM_Quit")
		{
			if (WindowManager::getFocusedWindow())
				WindowManager::getFocusedWindow()->close();
		}
	}

	if (m_text)
	{
		if (!containsMouse && e.getButton() == Mouse::LEFTBUTTON)
		{
			m_text->setColour({ 1.f, 1.f, 1.f, 1.f });
			
		}
	}
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void MainMenuText::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	if(!m_text) m_text = getParent()->getComponent<Text>();
	bool containsMouse = getParent()->containsPoint(e.getPos());

	if (m_text)
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 0.5f, 0.85f, 1.f });
		else if(!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 1.0f, 1.f, 1.f });
	}
}