/*! \file levelCompleteText.cpp
*
* \brief A script for all the interactable text in the level complete UI
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/levelCompleteText.h"
#include "scripts/layerControl.h"
#include "scripts/UI/startingMessage.h"
#include "loaders/sandboxLoader.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "scripts/gameTimer.h"

//! loadScene()
/*!
\param sceneName a const std::string& - The scene name
*/
void LevelCompleteText::loadScene(const std::string& sceneName)
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

LevelCompleteText::LevelCompleteText()
{
	m_text = nullptr;
	setScriptName("LevelCompleteText");
}

LevelCompleteText::~LevelCompleteText()
{
}

void LevelCompleteText::onAttach()
{
}

void LevelCompleteText::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_text) m_text = getParent()->getComponent<Text>();
}

void LevelCompleteText::onMousePress(MousePressedEvent & e, const float timestep, const float totalTime)
{
	bool containsMouse = getParent()->containsPoint(InputPoller::getMousePosition());

	if (m_text && getParent()->getName() != "LevelTimer")
	{
		if (containsMouse && e.getButton() == Mouse::LEFTBUTTON)
		{
			m_text->setColour({ 1.f, 0.7f, 0.f, 1.f });
		}
	}
}

void LevelCompleteText::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	bool containsMouse = getParent()->containsPoint(mousePosition);

	if (m_text && getParent()->getName() != "LevelTimer")
	{
		if (containsMouse && e.getButton() == Mouse::LEFTBUTTON)
		{
			m_text->setColour({ 1.f, 1.f, 1.f, 1.f });
			if (getParent()->getName() == "LevelQuit")
			{
				LayerControl::hide("LevelCompleteLayer", getParent()->getParentScene());
				LayerControl::deactivate("LevelCompleteLayer", getParent()->getParentScene());
				LayerControl::show("CreditsLayer", getParent()->getParentScene());
				LayerControl::activate("CreditsLayer", getParent()->getParentScene());
				WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
			}
		}
	}
}

void LevelCompleteText::onMouseMoved(MouseMovedEvent & e, const float timestep, const float totalTime)
{
	if (!m_text) m_text = getParent()->getComponent<Text>();
	bool containsMouse = getParent()->containsPoint(e.getPos());

	if (m_text && getParent()->getName() != "LevelTimer")
	{
		if (containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 0.5f, 0.85f, 1.f });
		else if (!containsMouse && !InputPoller::isMousePressed(Mouse::LEFTBUTTON))
			m_text->setColour({ 1.f, 1.0f, 1.f, 1.f });
	}
}

void LevelCompleteText::setTimeText()
{
	float time = static_cast<GameTimer*>(getParent()->getParentScene()->getEntity("TimerEntity")->getComponent<NativeScript>())->getTime();
	std::string timeStr = std::to_string(time);
	timeStr = timeStr.substr(0, timeStr.find('.') + 2);
	m_text->setText("You completed the level in: " + timeStr + " seconds.");
}