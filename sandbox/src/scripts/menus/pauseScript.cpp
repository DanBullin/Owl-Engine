/*! \file pauseScript.cpp
*
* \brief A script to show the pause menu
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/pauseScript.h"
#include "settings/settings.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/entities/entity.h"
#include "loaders/sandboxLoader.h"
#include "scripts/layerControl.h"
#include "scripts/gameTimer.h"

//! PauseScript()
PauseScript::PauseScript()
{
	setScriptName("PauseScript");
}

//! ~PauseScript()
PauseScript::~PauseScript()
{
}

//! onAttach()
void PauseScript::onAttach()
{
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void PauseScript::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	Scene* scene = getParent()->getParentScene();
	StudioSound* sound = nullptr;
	
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Pause).first)
	{
		if (LayerControl::isDisplayed("UI", getParent()->getParentScene()))
		{
			static_cast<GameTimer*>(getParent()->getParentScene()->getEntity("TimerEntity")->getComponent<NativeScript>())->stop();
			SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("pause_game")->getComponent<StudioSound>()->playSound();
			Layer* startingLayer = getParent()->getParentScene()->getLayerManager()->getLayer("StartingMsg");
			if (startingLayer)
			{
				if (startingLayer->getDisplayed() == true)
				{
					return;
				}
			}

			LayerControl::show("Pause", scene);
			LayerControl::activate("Pause", scene);
			SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(true);
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
			LayerControl::hide("UI", scene);
			SceneManager::getActiveScene()->setPhysicsPaused(true);
		}
	}
}