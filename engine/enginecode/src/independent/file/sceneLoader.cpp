/*! \file sceneLoader.cpp
*
* \brief A scene loader class which loads the scene from a JSON file.
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/file/sceneLoader.h"

namespace OwlEngine
{
	std::function<void(const std::string&, const std::string&)> SceneLoader::s_loadCallback;

	void SceneLoader::load(const std::string & sceneName, const std::string & sceneFolderPath)
	{
		s_loadCallback(sceneName, sceneFolderPath);
	}

	void SceneLoader::setCallback(std::function<void(const std::string&, const std::string&)> func)
	{
		s_loadCallback = func;
	}
}