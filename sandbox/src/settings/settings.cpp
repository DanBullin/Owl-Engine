/*! \file settings.h
*
* \brief This class handles the player's personal settings for aspects which are configurable
*
* \author Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "settings/settings.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/audioManager.h"

namespace OwlEngine
{
	std::map<PlayerConfig::ConfigData, std::pair<uint32_t, uint32_t>> Settings::s_keyBinds;
	bool Settings::s_muteAudio = false;
	float Settings::s_volume = 10.0f;
	bool Settings::s_vSync = false;

	//! getJSON
	/*!
	\param func a PlayerConfig::ConfigData - The player config
	\return a nlohmann::ordered_json - The data in json format
	*/
	nlohmann::ordered_json Settings::getJSON(PlayerConfig::ConfigData func)
	{
		nlohmann::ordered_json data;
		data["function"] = PlayerConfig::toString(func);
		data["key1"] = getKeys(func).first;
		data["key2"] = getKeys(func).second;
		return data;
	}

	//! Settings()
	Settings::Settings()
	{
	}
	
	//! ~Settings()
	Settings::~Settings()
	{
	}

	//! loadFromFile()
	void Settings::loadFromFile()
	{
		nlohmann::json jsonData = ResourceManager::getJSON("assets/playerConfig.json");

		s_muteAudio = jsonData["muteAudio"].get<bool>();
		AudioManager::muteAudio(s_muteAudio);

		s_volume = jsonData["volume"].get<float>();
		AudioManager::getVolumeSettings(s_volume);

		s_vSync = jsonData["enableVSync"].get<bool>();
		WindowManager::getFocusedWindow()->setVSync(s_vSync);

		for (auto& keybind : jsonData["keybinds"])
		{
			bindKey(PlayerConfig::toType(keybind["function"].get<std::string>()), keybind["key1"].get<uint32_t>(), keybind["key2"].get<uint32_t>());
		}
	}

	//! saveToFile()
	void Settings::saveToFile()
	{
		nlohmann::ordered_json config;
		std::string configFile = "assets/playerConfig.json";

		config["muteAudio"] = s_muteAudio;
		config["volume"] = s_volume;
		config["enableVSync"] = s_vSync;

		for (auto& keys : s_keyBinds)
		{
			config["keybinds"] += getJSON(keys.first);
		}


		std::ofstream file(configFile);
		file << std::setw(4) << config << std::endl;
		file.close();
	}

	void Settings::setBool(const PlayerConfig::ConfigData function, bool value)
	{
		switch (function)
		{
			case PlayerConfig::ConfigData::MuteAudio:
			{
				s_muteAudio = value;
				AudioManager::muteAudio(value);
				break;
			}
			case PlayerConfig::ConfigData::ToggleVSync:
			{
				s_vSync = value;
				WindowManager::getFocusedWindow()->setVSync(s_vSync);
				break;
			}
		}
	}

	void Settings::setFloat(const PlayerConfig::ConfigData function, float value)
	{
		switch (function)
		{
		case PlayerConfig::ConfigData::Volume:
		{
			s_volume = value;
			AudioManager::getVolumeSettings(value);
			break;
		}
		}
	}

	bool Settings::getBool(const PlayerConfig::ConfigData function)
	{
		switch (function)
		{
			case PlayerConfig::ConfigData::MuteAudio:
			{
				return s_muteAudio;
			}
			case PlayerConfig::ConfigData::ToggleVSync:
			{
				return s_vSync;
			}
		}
		return false;
	}

	float Settings::getFloat(const PlayerConfig::ConfigData function)
	{
		switch (function)
		{
		case PlayerConfig::ConfigData::Volume:
		{
			return s_volume;
		}
		}
		return false;
	}

	//! bindKey()
	/*!
	\param function a const PlayerConfig::ConfigData - The function to bind to a key (enum)
	\param key1 a const uint32_t - The key to bind
	\param key2 a const uint32_t - The key to bind
	*/
	void Settings::bindKey(const PlayerConfig::ConfigData function, const uint32_t key1, const uint32_t key2)
	{
		s_keyBinds[function] = { key1, key2 };
	}

	//! getKeys()
	/*!
	\param function a const PlayerConfig::ConfigData - The function to check if the keys for it as being pressed
	\return a std::pair<uint32_t, uint32_t> - The pair of keys bound to this function
	*/
	std::pair<uint32_t, uint32_t> Settings::getKeys(const PlayerConfig::ConfigData function)
	{
		if (s_keyBinds.find(function) != s_keyBinds.end())
			return s_keyBinds[function];
		else
			return { 0, 0 };
	}
}