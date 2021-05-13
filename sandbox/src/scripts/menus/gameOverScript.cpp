/*! \file gameOverScript.cpp
*
* \brief A script to show the game over display
*
* \author: Jamie Hall
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/gameOverScript.h"
#include "settings/settings.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/entities/entity.h"
#include "loaders/sandboxLoader.h"
#include "scripts/layerControl.h"
#include "scripts/gameTimer.h"

//! GameOverScript()
GameOverScript::GameOverScript()
{
	setScriptName("GameOverScript");
}

//! ~PauseScript()
GameOverScript::~GameOverScript()
{
}

//! onAttach()
void GameOverScript::onAttach()
{
}

void GameOverScript::activateGameOver()
{
	Scene* scene = getParent()->getParentScene();
	StudioSound* sound = nullptr;

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

	LayerControl::show("GameOver", scene);
	LayerControl::activate("GameOver", scene);
	SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(true);
	SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("player_death")->getComponent<StudioSound>()->playSound();
	WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
	LayerControl::hide("UI", scene);
	SceneManager::getActiveScene()->setPhysicsPaused(true);
}
