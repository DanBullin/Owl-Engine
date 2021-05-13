/*! \file sceneLoader.h
*
* \brief A scene loader class which loads the scene from a JSON file.
*
* \author Daniel Bullin
*
*/
#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <string>
#include <json.hpp>

namespace OwlEngine
{
	using json = nlohmann::json; //!< Type alias

	/*! \class SceneLoader
	* \brief A loader class which loads the scene object by creating all the objects from files
	*/
	class SceneLoader
	{
	private:
		static std::function<void(const std::string&, const std::string&)> s_loadCallback; //!< The load function callback
	public:
		static void load(const std::string& sceneName, const std::string& sceneFolderPath); //!< Load the scene
		static void setCallback(std::function<void(const std::string&, const std::string&)> func); //!< Set the load function callback
	};
}

#endif