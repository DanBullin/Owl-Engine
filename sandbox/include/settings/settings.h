/*! \file settings.h
*
* \brief This class handles the player's personal settings for aspects which are configurable
*
* \author Daniel Bullin
*
*/
#ifndef SETTINGS_H
#define SETTINGS_H

#include <inttypes.h>
#include <json.hpp>
#include <map>

namespace OwlEngine
{
	namespace PlayerConfig
	{
		/*! \enum ConfigData
		* \brief The different types of configurable data for the player
		*/
		enum ConfigData
		{
			None, MoveForward, MoveLeft, MoveRight, MoveBack, Pause, MoveUp, MoveDown, FreezeCamera, UnlockDoor, SaveScene, EnableInspector, FullscreenWindow, ToggleCursorInput, ToggleVSync, MinimiseWindow,
			ToggleColliders, Hotbar1, Hotbar2, Hotbar3, Hotbar4, Hotbar5, Pickup, MuteAudio, Volume
		};

		static uint32_t configCount = 26;

		//! toType()
		/*!
		\param configType a const std::string& - The config type as a string
		\return a const ConfigData - The config type
		*/
		static const ConfigData toType(const std::string& configType)
		{
			if (configType == "MoveForward") return ConfigData::MoveForward;
			if (configType == "MoveLeft") return ConfigData::MoveLeft;
			if (configType == "MoveRight") return ConfigData::MoveRight;
			if (configType == "MoveBack") return ConfigData::MoveBack;
			if (configType == "Pause") return ConfigData::Pause;
			if (configType == "MoveUp") return ConfigData::MoveUp;
			if (configType == "MoveDown") return ConfigData::MoveDown;
			if (configType == "FreezeCamera") return ConfigData::FreezeCamera;
			if (configType == "UnlockDoor") return ConfigData::UnlockDoor;
			if (configType == "SaveScene") return ConfigData::SaveScene;
			if (configType == "EnableInspector") return ConfigData::EnableInspector;
			if (configType == "FullscreenWindow") return ConfigData::FullscreenWindow;
			if (configType == "ToggleCursorInput") return ConfigData::ToggleCursorInput;
			if (configType == "ToggleVSync") return ConfigData::ToggleVSync;
			if (configType == "MinimiseWindow") return ConfigData::MinimiseWindow;
			if (configType == "ToggleColliders") return ConfigData::ToggleColliders;
			if (configType == "Hotbar1") return ConfigData::Hotbar1;
			if (configType == "Hotbar2") return ConfigData::Hotbar2;
			if (configType == "Hotbar3") return ConfigData::Hotbar3;
			if (configType == "Hotbar4") return ConfigData::Hotbar4;
			if (configType == "Hotbar5") return ConfigData::Hotbar5;
			if (configType == "Pickup") return ConfigData::Pickup;
			if (configType == "MuteAudio") return ConfigData::MuteAudio;
			if (configType == "Volume") return ConfigData::Volume;
			else return ConfigData::None;
		}

		//! toString()
		/*!
		\param configType a ConfigData - The config type
		\return a std::string - The type as a string
		*/
		static std::string toString(ConfigData configType)
		{
			switch (configType)
			{
			case ConfigData::MoveForward: return "MoveForward";
			case ConfigData::MoveLeft: return "MoveLeft";
			case ConfigData::MoveRight: return "MoveRight";
			case ConfigData::MoveBack: return "MoveBack";
			case ConfigData::Pause: return "Pause";
			case ConfigData::MoveUp: return "MoveUp";
			case ConfigData::MoveDown: return "MoveDown";
			case ConfigData::FreezeCamera: return "FreezeCamera";
			case ConfigData::UnlockDoor: return "UnlockDoor";
			case ConfigData::SaveScene: return "SaveScene";
			case ConfigData::EnableInspector: return "EnableInspector";
			case ConfigData::FullscreenWindow: return "FullscreenWindow";
			case ConfigData::ToggleCursorInput: return "ToggleCursorInput";
			case ConfigData::ToggleVSync: return "ToggleVSync";
			case ConfigData::MinimiseWindow: return "MinimiseWindow";
			case ConfigData::ToggleColliders: return "ToggleColliders";
			case ConfigData::Hotbar1: return "Hotbar1";
			case ConfigData::Hotbar2: return "Hotbar2";
			case ConfigData::Hotbar3: return "Hotbar3";
			case ConfigData::Hotbar4: return "Hotbar4";
			case ConfigData::Hotbar5: return "Hotbar5";
			case ConfigData::Pickup: return "Pickup";
			case ConfigData::MuteAudio: return "MuteAudio";
			case ConfigData::Volume: return "Volume";
			case ConfigData::None: return "None";
			default: return "NONE";
			}
		}
	}

	/*! \class Settings
	* \brief A class which handles the player's personal settings for aspects which are configurable
	*/
	class Settings
	{
	private:
		static std::map<PlayerConfig::ConfigData, std::pair<uint32_t, uint32_t>> s_keyBinds; //!< The available keybinds
		static bool s_muteAudio; //!< Is audio muted
		static float s_volume; //!< Is audio muted
		static bool s_vSync; //!< Is VSync enabled
		static nlohmann::ordered_json getJSON(PlayerConfig::ConfigData func); //!< Get the json format
	public:
		Settings(); //!< Constructor
		~Settings(); //!< ~Destructor

		static void loadFromFile(); //!< Load the settings stored in file
		static void saveToFile(); //!< Save the current player config settings
		static void setBool(const PlayerConfig::ConfigData function, bool value); //!< Set a bool
		static void setFloat(const PlayerConfig::ConfigData function, float value); //!< Set a float
		static bool getBool(const PlayerConfig::ConfigData function); //!< Get a bool
		static float getFloat(const PlayerConfig::ConfigData function); //!< Get a float
		static void bindKey(const PlayerConfig::ConfigData function, const uint32_t key1, const uint32_t key2); //!< Bind a key to a function
		static std::pair<uint32_t, uint32_t> getKeys(const PlayerConfig::ConfigData function); //!< Get the keys bound to the function
	};
}
#endif