/*! \file sceneSaver.cpp
*
* \brief A scene saver class which saves the scene to a JSON file.
*
* \author Joshua Simons
*
*/
#include "engine_pch.h"
#include "independent/file/sceneSaver.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/log.h"
#include <iomanip>

namespace OwlEngine
{
	//! save()
	/*!
	\param scene a Scene* - A pointer to the scene
	*/
	void SceneSaver::save(Scene* scene)
	{
		if (!scene)
		{
			ENGINE_ERROR("[SceneSaver::save] Invalid scene provided! Unable to save!");
			return;
		}
		else ENGINE_INFO("[SceneSaver::save] Saving scene to file... Scene Name: {0}", scene->getName());

		nlohmann::ordered_json config;
		if (scene->getPhysicsWorld() != nullptr)
		{
			config["addPhysicsWorld"] = true;
			config["pausePhysics"] = scene->getPhysicsPaused();
			config["timeScale"] = scene->getTimeScale();
		}
		else config["addPhysicsWorld"] = false;

		config["viewPort"] = {scene->getViewPort().x, scene->getViewPort().y};

		for (auto& layer : scene->getLayerManager()->getLayers())
		{
			nlohmann::ordered_json lay;
			lay["name"] = layer->getLayerName();
			lay["active"] = layer->getActive();
			lay["displayed"] = layer->getDisplayed();
			config["layers"] += lay;
		}

		for (auto& rp : scene->getRenderPasses())
		{
			nlohmann::ordered_json pass;
			pass["name"] = rp->getName();
			pass["enabled"] = rp->getEnabled();
			config["renderPasses"] += pass;
		}
		std::string conFile = scene->getFolderPath() + "config.json";
		std::ofstream conf(conFile);
		conf << std::setw(4) << config << std::endl;
		conf.close();
		ENGINE_INFO("[SceneSaver::save] Scene config saved at {0}", conFile);
		
		nlohmann::ordered_json data;
		auto entities = scene->getRootEntities();
		for (auto& ent : entities)
		{
			if(ent.second->getSaved())
				data["entities"] += ent.second->getAsJson();
		}

		std::string entFile = scene->getFolderPath() + "entities.json";

		std::ofstream file(entFile);
		file << std::setw(4) << data << std::endl;
		file.close();
		ENGINE_INFO("[SceneSaver::save] Scene entities saved at {0}", entFile);
	}
}