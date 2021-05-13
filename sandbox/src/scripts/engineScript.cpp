/*! \file engineScript.h
*
* \brief A script to use some engine features
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/engineScript.h"
#include "settings/settings.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/file/sceneSaver.h"

//! EngineScript()
EngineScript::EngineScript()
{
	setScriptName("EngineScript");
}

//! ~OwlEngine()
EngineScript::~EngineScript()
{
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void EngineScript::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
#ifndef NG_DEPLOY
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::SaveScene).first) // Save File
		SceneSaver::save(SceneManager::getActiveScene());

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::EnableInspector).first) // Toggle Inspector
	{
		SceneManager::getActiveScene()->enableInspector(!SceneManager::getActiveScene()->getInspectorEnabled());
		if (!SceneManager::getActiveScene()->getInspectorEnabled()) SceneManager::getActiveScene()->setViewPos({ 0.f, 0.f });
	}

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::ToggleCursorInput).first) // Toggle Cursor input
	{
		switch (WindowManager::getFocusedWindow()->getProperties().getCursorInputMode())
		{
		case CursorInputMode::Visible:
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Disabled);
			break;
		case CursorInputMode::Hidden:
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
			break;
		case CursorInputMode::Disabled:
			WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
			break;
		}
	}

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::ToggleColliders).first && InputPoller::isKeyPressed(Settings::getKeys(PlayerConfig::ToggleColliders).second))
	{
		ResourceManager::setConfigValue(Config::ShowColliders, !ResourceManager::getConfigValue(Config::ShowColliders));
	}

#endif

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::FullscreenWindow).first)
		WindowManager::getFocusedWindow()->setFullscreen(!WindowManager::getFocusedWindow()->getProperties().getFullScreen()); // Toggle Fullscreen

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MinimiseWindow).first)
		WindowManager::getFocusedWindow()->setMinimised(!WindowManager::getFocusedWindow()->getProperties().getMinimised()); // Minimise/Restore

}