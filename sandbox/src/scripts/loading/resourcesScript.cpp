/*! \file resourcesScript.cpp
*
* \brief A resources script
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/loading/resourcesScript.h"
#include "settings/settings.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/threadManager.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/utils/resourceLoader.h"
#include "loaders/sandboxLoader.h"

//! ResourcesScript()
ResourcesScript::ResourcesScript()
{
	m_uploadedModels = false;
	m_loadingText = nullptr;
	m_thread = nullptr;
	setScriptName("ResourcesScript");
}

//! ~ResourcesScript()
ResourcesScript::~ResourcesScript()
{
}

//! onAttach()
void ResourcesScript::onAttach()
{
	WindowManager::getFocusedWindow()->setDecorated(false);
	Settings::loadFromFile();
	ThreadManager::startThread("LoadResources", false, ResourceManager::loadTResources);
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void ResourcesScript::onPostUpdate(const float timestep, const float totalTime)
{
	if (!m_loadingText || !m_thread)
	{
		m_loadingText = getParent()->getParentScene()->getEntity("LoadingText")->getComponent<Text>();
		m_thread = ThreadManager::getThread("LoadResources");
	}

	m_loadingText->setText("Loading Resource: " + ResourceManager::getResBeingLoaded());

	if (m_thread)
	{
		if (m_thread->isFinished())
		{
			ResourceLoader::uploadModels();
			m_uploadedModels = true;
			ThreadManager::deleteThread("LoadResources");
			m_thread = nullptr;
		}
	}

	if (totalTime > 5.f && m_uploadedModels)
	{
		SandboxLoader::load("mainMenu", "assets/scenes/mainMenu/");
		SceneManager::setActiveScene("mainMenu", false);
		WindowManager::getFocusedWindow()->setDecorated(true);
		SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("menu_music")->getComponent<StudioSound>()->playSound();
	}
}