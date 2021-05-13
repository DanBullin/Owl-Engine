/*! \file scriptManager.h
*
* \brief A manager which manages all the scripts
*
* \author Daniel Bullin
*
*/
#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "independent/core/common.h"
#include "independent/systems/system.h"
#include "independent/entities/components/nativeScript.h"

namespace OwlEngine
{
	/*! \class ScriptManager
	* \brief A manager class which manages scripts
	*/
	class ScriptManager : public System
	{
	private:
		static bool s_enabled; //!< Is the script manager enabled
		static std::vector<std::string> s_scriptNames; //!< List of all the scripts
		static std::function<NativeScript*(std::string, nlohmann::json)> s_scriptCallback; //!< The add script callback
	public:
		ScriptManager(); //!< Constructor
		~ScriptManager(); //!< Destructor

		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static void addScript(const std::string& scriptName); //!< Add a script name to the manager
		static const std::vector<std::string>& getScriptNames(); //!< Get the script names

		static void setNewScriptCallback(std::function<NativeScript*(std::string, nlohmann::json)> func); //!< Set the callback function when adding new scripts
		static NativeScript* createNewScript(const std::string& scriptName); //!< Create a new script using the callback
	};
}
#endif