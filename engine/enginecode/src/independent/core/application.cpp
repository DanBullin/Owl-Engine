/*! \file application.cpp
*
* \brief Main application, the start point of the engine
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/core/application.h"
#include "independent/systems/systemManager.h"
#include "file/fastLoadScreen.h"

namespace OwlEngine
{
	Application* Application::s_instance = nullptr; //!< Set the static instance pointer to null
	bool Application::s_running = true; //!< Set to true

	//! Application()
	Application::Application()
	{
		// Set instance variable
		if (s_instance == nullptr) { s_instance = this; }

		// Initialise the system manager with a maximum number of systems, then add all systems that will be needed
		SystemManager::initialise(18);

		SystemManager::addSystem(SystemType::Logger);
		SystemManager::addSystem(SystemType::Randomiser);
		SystemManager::addSystem(SystemType::TimerSystem);
		SystemManager::addSystem(SystemType::WindowAPISystem);
		SystemManager::addSystem(SystemType::WindowManager);
		FastLoadScreen::fls();
		SystemManager::addSystem(SystemType::Physics);
		SystemManager::addSystem(SystemType::EventManager);
		SystemManager::addSystem(SystemType::Audio);
		SystemManager::addSystem(SystemType::ResourceManager);
		SystemManager::addSystem(SystemType::FontManager);
		SystemManager::addSystem(SystemType::SceneManager);
		SystemManager::addSystem(SystemType::RenderSystem);
#ifndef NG_DEPLOY
		SystemManager::addSystem(SystemType::Inspector);
#endif // NG_DEPLOY
		SystemManager::addSystem(SystemType::ThreadManager);
		SystemManager::addSystem(SystemType::ScriptManager);
		
		ResourceManager::loadNTResources();

		// Start timers for FPS and total application time
		TimerSystem::startTimer("FPS");
		TimerSystem::startTimer("TotalTime");
	}

	//! ~Application()
	Application::~Application()
	{
		s_instance = nullptr;

		// Destroy all systems and their resources
		SystemManager::destroy();
	}

	//! run()
	void Application::run()
	{
		while (s_running)
		{
			Scene* scene = SceneManager::getActiveScene();

			// Check to make sure the active scene is valid
			if (!scene)
			{
				ENGINE_ERROR("[Application::run] The active scene is an invalid scene.");
				Application::stop();
			}

			float timestep = GET_TIME("FPS", true, true);
			float totalTime = GET_TIME("TotalTime", false, true);

			// Update all necassary items through the event manager
			TIME_FUNCTION("onUpdateP", EventManager::onUpdate(scene, timestep, totalTime));

			TIME_FUNCTION("onRenderP",
				if (WindowManager::getFocusedWindow())
					RenderSystem::onRender(scene));

#ifndef NG_DEPLOY
			// Inspect the scene with the inspector
			TIME_FUNCTION("onInspectP",
			if(scene->getInspectorEnabled())
				Inspector::inspect()); 
#endif // NG_DEPLOY
			
			// Destroy all scheduled scenes
			SceneManager::destroyScheduledScenes();

			// Deregister all windows that are scheduled to be deleted
			// This must be done at the end of the frame
			WindowManager::deregisterScheduledWindows();

			// Check exit conditions
			if (checkExitConditions())
				Application::stop();
		}
	}

	//! stop()
	void Application::stop()
	{
		// Stop the game loop
		s_running = false;
	}

	//! checkExitConditions()
	/*!
	\return a const bool - The exit conditions has been checked and return a value if they are true
	*/
	const bool Application::checkExitConditions()
	{
		// No more windows are open, so let's close the application
		if (WindowManager::getRegisteredWindows().size() == 0)
			return true;

		return false;
	}
}