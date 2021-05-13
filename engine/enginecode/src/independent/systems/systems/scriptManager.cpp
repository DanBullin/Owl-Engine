/*! \file scriptManager.cpp
*
* \brief A manager which manages all the scripts
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/systems/systems/scriptManager.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/log.h"
#include <json.hpp>

namespace OwlEngine
{
	bool ScriptManager::s_enabled = false;
	std::vector<std::string> ScriptManager::s_scriptNames;
	std::function<NativeScript*(std::string, nlohmann::json)> ScriptManager::s_scriptCallback;

	//! SceneManager()
	ScriptManager::ScriptManager() : System(SystemType::ScriptManager)
	{
	}

	//! ~SceneManager()
	ScriptManager::~ScriptManager()
	{
	}

	//! start()
	void ScriptManager::start()
	{
		// Start this system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[ScriptManager::start] Starting the script manager system.");

			nlohmann::json configData = ResourceManager::getJSON("assets/scripts.json");

			for (auto& script : configData["scripts"])
			{
				ENGINE_TRACE("[ScriptManager::start] Loading script: {0}.", script["name"].get<std::string>());
				addScript(script["name"].get<std::string>());
			}

			s_enabled = true;
		}
	}

	//! ~stop()
	void ScriptManager::stop()
	{
		// Stop this system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[ScriptManager::stop] Stopping the script manager system.");
			s_enabled = false;
		}
	}

	//! addScript()
	/*!
	\param scriptName a const std::string& - The name of the script
	*/
	void ScriptManager::addScript(const std::string& scriptName)
	{
		for (auto& name : s_scriptNames)
		{
			if (name == scriptName) return;
		}
		s_scriptNames.emplace_back(scriptName);
	}

	//! getScriptNames()
	/*!
	\return a const std::vector<std::string>& - A list of all script names
	*/
	const std::vector<std::string>& ScriptManager::getScriptNames()
	{
		return s_scriptNames;
	}

	//! setNewScriptCallback()
	/*!
	\param func a std::function<NativeScript*(std::string, nlohmann::json)> - The function callback
	*/
	void ScriptManager::setNewScriptCallback(std::function<NativeScript*(std::string, nlohmann::json)> func)
	{
		s_scriptCallback = func;
	}

	//! createNewScript()
	/*!
	\param scriptName a const std::string& - The name of the new script to create
	*/
	NativeScript* ScriptManager::createNewScript(const std::string& scriptName)
	{
		return s_scriptCallback(scriptName, {});
	}
}