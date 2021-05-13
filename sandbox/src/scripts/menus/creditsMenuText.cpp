/*! \file creditsMenuText.cpp
*
* \brief A script for all the interactable text in the credits menu
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/creditsMenuText.h"
#include "loaders/sandboxLoader.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "scripts/layerControl.h"
#include "scripts/gameTimer.h"

//! loadScene()
/*!
\param sceneName a const std::string& - The scene name
*/
void CreditsMenuText::loadScene(const std::string& sceneName)
{
	bool inspector = getParent()->getParentScene()->getInspectorEnabled();
	if (SceneManager::getScene(sceneName))
	{
		SceneManager::setActiveScene(sceneName, true);
		SceneManager::getActiveScene()->enableInspector(inspector);
	}
	else
	{
		SandboxLoader::load(sceneName, "assets/scenes/" + sceneName + "/");
		SceneManager::setActiveScene(sceneName, true);
		SceneManager::getActiveScene()->enableInspector(inspector);
	}
}

//! CreditsMenuText()
CreditsMenuText::CreditsMenuText()
{
	m_text = nullptr;
	setScriptName("CreditsMenuText");
}

//! ~CreditsMenuText()
CreditsMenuText::~CreditsMenuText()
{
}

//! onMousePress()
/*!
\param e a MousePressedEvent& - A mouse released event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void CreditsMenuText::onMousePress(MousePressedEvent& e, const float timestep, const float totalTime)
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
void CreditsMenuText::onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	std::string entityName = getParent()->getName();
	bool containsMouse = getParent()->containsPoint(mousePosition);
	std::string sceneName = getParent()->getParentScene()->getName();
	Scene* scene = getParent()->getParentScene();

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("click")->getComponent<StudioSound>()->playSound();

		if (entityName == "Credits_Quit")
		{
			LayerControl::hide("CreditsLayer", scene);
			LayerControl::deactivate("CreditsLayer", scene);

			LayerControl::show("UI", scene);
			LayerControl::activate("UI", scene);

			if (getParent()->getParentScene()->getName() != "mainMenu")
			{
				loadScene("mainMenu");
			}

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
void CreditsMenuText::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
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