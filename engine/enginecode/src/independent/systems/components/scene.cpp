/*! \file scene.cpp
*
* \brief A scene contains all the data associated in a particular environment. This data will be loaded by the SceneLoader.
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/systems/components/scene.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/eventManager.h"
#include "independent/systems/systems/physicsSystem.h"


namespace OwlEngine
{
	//! Scene()
	/*!
	\param name a const std::string& - The name of the scene
	\param sceneFolderPath a const std::string& - The file path to the folder of the scene
	*/
	Scene::Scene(const std::string& name, const std::string& sceneFolderPath)
	{
		m_sceneName = name;
		m_sceneFolderPath = sceneFolderPath;
		m_layerManager = new LayerManager(this);
		m_renderPasses.reserve(ResourceManager::getConfigValue(Config::MaxRenderPassesPerScene));
		m_physicsWorld = nullptr;
		m_mainCamera = nullptr;
		m_entityListUpdated = true;
		m_physicsPaused = false;
		m_timeScale = 1.0f;
		m_entityNextID = 0;
		m_viewPos = { 0.f, 0.f };

#ifndef NG_DEPLOY
		m_enableInspector = false;
		m_refreshInspector = true;
#endif // NG_DEPLOY

		// Print the scene's details upon creation
		ENGINE_TRACE("[Scene::Scene] Scene details upon creation for: {0}.", m_sceneName);
		printSceneDetails();
	}

	//! ~Scene()
	Scene::~Scene()
	{
		ENGINE_INFO("[Scene::~Scene] Destroying scene: {0}.", m_sceneName);

		// If there is any entities in the scene, delete them
		if (m_rootEntities.size() != 0)
		{
			// Delete all root entities which will delete all children too
			for (auto& rootEntity : m_rootEntities)
				delete rootEntity.second;

			m_rootEntities.clear();
		}

		// If there is a valid layer manager, delete it
		if (m_layerManager)
		{
			delete m_layerManager;
			m_layerManager = nullptr;
		}

		// If there are valid render passes, delete them
		if (m_renderPasses.size() != 0)
		{
			for (auto& renderPass : m_renderPasses)
				if (renderPass)
				{
					renderPass->onDetach();
					delete renderPass;
				}

			m_renderPasses.clear();
		}

		m_entityNextID = 0;

		// If there is a valid physics world, delete it through the physics system
		if (m_physicsWorld)
		{
			PhysicsSystem::removeWorld(m_physicsWorld);
			m_physicsWorld = nullptr;
		}

		// Camera pointer is deleted by the entity it is attached to, so we only need to set the pointer
		if(m_mainCamera) m_mainCamera = nullptr;
#ifndef NG_DEPLOY
		m_enableInspector = false;
		m_refreshInspector = true;
#endif // NG_DEPLOY
		m_entityListUpdated = true;
		m_entitiesList.clear();
	}

	//! getName()
	/*!
	\return a const std::string& - The name of the scene
	*/
	const std::string& Scene::getName()
	{
		return m_sceneName;
	}

	//! getFolderPath()
	/*!
	\return a const std::string& - The file path to the folder of the scene
	*/
	const std::string& Scene::getFolderPath()
	{
		return m_sceneFolderPath;
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The update time step
	\param totalTime a const float - The total application time
	*/
	void Scene::onUpdate(const float timestep, const float totalTime)
	{
		// Delete any entities that are scheuled to be deleted, can also update components if entity is not to be deleted
		for (auto it = m_rootEntities.cbegin(); it != m_rootEntities.cend(); )
		{
			// Check if entity is a valid entity
			if (it->second)
			{
				// Check if the entity is to be destroyed
				if (it->second->getDestroyed())
				{
					// Do NOT delete the entity which contains the main camera
					if (it->second == m_mainCamera->getParent())
					{
						ENGINE_ERROR("[Scene::onUpdate] Cannot delete the entity when it contains the scene's main camera. Scene: {0}, Entity: {1}.", m_sceneName, it->second->getName());
						it->second->undestroy();
						return;
					}

					// Check for validness again and then delete
					if(it->second) delete it->second;
					m_rootEntities.erase(it++);
					setEntityListUpdated(true);
				}
				else
				{
					// Entity is not to be deleted, update components
					if (it->second)
					{
						it->second->onUpdate(timestep, totalTime);

						for (auto& component : it->second->getAllComponents())
						{
							// Check if component is valid
							if(component)
								component->onUpdate(timestep, totalTime);
						}
						++it;
					}
				}
			}
		}

		// Update the layer manager if it is valid
		if(m_layerManager)
			m_layerManager->onUpdate(timestep, totalTime);

		// Update the physics world if it is valid
		if (m_physicsWorld)
		{
			float physicsTime = timestep * m_timeScale;
			if (!m_physicsPaused) PhysicsSystem::onUpdate(m_physicsWorld, this, physicsTime, totalTime);
		}
			
	}

	//! addEntity()
	/*!
	\param name a const std::string& - The name of the entity
	\param entity an Entity* - A pointer to the entity
	*/
	void Scene::addEntity(const std::string& name, Entity* entity)
	{
		if (!checkRootEntityNameTaken(name) && name != "")
		{
			// If an invalid entity was provided, just exit out the function
			if (!entity)
			{
				ENGINE_ERROR("[Scene:addEntity] An invalid entity pointer was provided for entity name: {0} for scene: {1}.", name, m_sceneName);
				return;
			}

			//remove it from the list its currently sitting in
			if (entity->getParentEntity() != nullptr)
				entity->getParentEntity()->getChildEntities().erase(entity->getName());
			else if (entity->getSelected())
				entity->getParentScene()->cleanRoot(entity->getName());

			//get its position and offset it correctly
			Transform* childTrans = entity->getComponent<Transform>();
			if (childTrans) {
				glm::vec3 childPos = childTrans->getPosition();
				childTrans->setPosition(childPos);
			}

			// Set the parent scene of the entity and its name
			m_rootEntities[name] = entity;
			entity->setParentScene(this);
			entity->setParentEntity(nullptr);
			entity->setName(name);
			entity->setUniqueID(m_entityNextID); m_entityNextID++;
			setEntityListUpdated(true);
		}
		else
			ENGINE_ERROR("[Scene::addEntity] Name for entity already taken. Cannot add. Entity Name: {0} for scene: {1}.", name, m_sceneName);
	}

	//! getEntity()
	/*!
	\param name a const std::string& - The name of the entity
	\return an Entity* - A pointer to the entity
	*/
	Entity* Scene::getEntity(const std::string& name)
	{
		// Check if entity name exists
		if (checkRootEntityNameTaken(name) && name != "")
		{
			if(!m_rootEntities[name])
				ENGINE_ERROR("[Scene::getEntity] The entity we're retrieving is an invalid entity. Scene Name: {0}", m_sceneName);

			return m_rootEntities[name];
		}
		else
			ENGINE_ERROR("[Scene::getEntity] Cannot get entity named: {0}.", name);

		// Can't find entity
		return nullptr;
	}

	//! getEntity
	/*!
	\param uniqueID a const uint32_t - The ID of the entity
	\return an Entity* - A pointer to the entity
	*/
	Entity* Scene::getEntity(const uint32_t uniqueID)
	{
		for (auto& entity : m_rootEntities)
		{
			if (entity.second->getUniqueID() == uniqueID)
				return entity.second;

			auto ent = getEntity(entity.second, uniqueID);
			if (ent)
				return ent;
		}
		return nullptr;
	}

	//! getEntity
	/*!
	\param entity an Entity* - The entity to search for children with ID
	\param uniqueID a const uint32_t - The ID of the entity
	\return an Entity* - A pointer to the entity
	*/
	Entity* Scene::getEntity(Entity* entity, const uint32_t uniqueID)
	{
		for (auto& child : entity->getChildEntities())
		{
			if (child.second->getUniqueID() == uniqueID)
				return child.second;

			auto ent = getEntity(child.second, uniqueID);
			if (ent)
				return ent;
		}
		return nullptr;
	}

	//! getRootEntities()
	/*!
	\return a std::map<std::string, Entity*> - Returns the list of root entities
	*/
	std::map<std::string, Entity*> Scene::getRootEntities() const
	{
		return m_rootEntities;
	}

	//! cleanRoot()
	/*!
	\param name  - Specific value to clean out (if none checks all variables for cleaning)
	*/
	void Scene::cleanRoot(std::string name)
	{
		if (name != "" && m_rootEntities[name] != nullptr)
		{
			m_rootEntities.erase(name);
		}
		else
		{
			ENGINE_WARN("[Scene::cleanRoot] Couldn't find entity in root, cleaning whole root! (If you see this warning frequently something is wrong!)");
			//Go through root and find everything thats erasable
			std::vector<std::string> eraseable;
			for (auto entity : m_rootEntities)
			{
				if (entity.second != nullptr)
				{
					Entity* parent = entity.second->getParentEntity();
					if (parent != nullptr)
					{
						//Child has parent
						if (parent->getChildEntity(entity.second->getName()) != nullptr)
						{
							//Parent has child
							eraseable.push_back(entity.first);
						}
					}
				}
				else eraseable.push_back(entity.first);
			}

			//Take out everything erasable
			for (std::string eraseName : eraseable)
			{
				m_rootEntities.erase(eraseName);
			}
		}
		
	}

	//! getEntities()
	/*!
	\return a std::vector<Entity*> - A list of all entities in the scene
	*/
	std::vector<Entity*> Scene::getEntities()
	{
		// A list of all the entities in this scene
		if (m_entityListUpdated)
		{
			// The entities in this scene has been updated
			// Clear the rendering entity list and refill it
			m_entitiesList.clear();

			for (auto& entity : m_rootEntities)
			{
				// Only add entity if its valid
				if (entity.second)
				{
					std::vector<Entity*> entList;
					entity.second->getAllEntities(entList);
					m_entitiesList.push_back(entity.second);
					m_entitiesList.insert(m_entitiesList.end(), entList.begin(), entList.end());
				}
			}

			m_entityListUpdated = false;
		}
		return m_entitiesList;
	}

	//! getClosestPointLights()
	/*!
	\return a std::vector<PointLight*> - A list of point lights sorted by distance from main camera
	*/
	std::vector<PointLight*> Scene::getClosestPointLights()
	{
		std::vector<PointLight*> pointLights;

		for (auto& entity : getEntities())
		{
			auto pointLight = entity->getComponent<PointLight>();
			if (pointLight)
			{
				if (pointLight->getParent()->getDisplay())
					pointLights.emplace_back(pointLight);
			}
		}

		std::sort(pointLights.begin(), pointLights.end(),
			[=](PointLight*& a, PointLight*& b)
		{
			return glm::distance(a->getWorldPosition(), getMainCamera()->getWorldPosition()) < glm::distance(b->getWorldPosition(), getMainCamera()->getWorldPosition());
		}
		);

		return pointLights;
	}

	//! getClosestSpotLights()
	/*!
	\return a std::vector<PointLight*> - A list of spot lights sorted by distance from main camera
	*/
	std::vector<SpotLight*> Scene::getClosestSpotLights()
	{
		std::vector<SpotLight*> spotLights;

		for (auto& entity : getEntities())
		{
			auto spotLight = entity->getComponent<SpotLight>();
			if (spotLight)
			{
				if(spotLight->getParent()->getDisplay())
					spotLights.emplace_back(spotLight);
			}
		}

		std::sort(spotLights.begin(), spotLights.end(),
			[=](SpotLight*& a, SpotLight*& b)
		{
			return glm::distance(a->getWorldPosition(), getMainCamera()->getWorldPosition()) < glm::distance(b->getWorldPosition(), getMainCamera()->getWorldPosition());
		}
		);

		return spotLights;
	}

	//! getClosestDirectionalLights()
	/*!
	\return a std::vector<PointLight*> - A list of directional lights sorted by distance from main camera
	*/
	std::vector<DirectionalLight*> Scene::getClosestDirectionalLights()
	{
		std::vector<DirectionalLight*> dirLights;

		for (auto& entity : getEntities())
		{
			auto dirLight = entity->getComponent<DirectionalLight>();
			if (dirLight)
			{
				if (dirLight->getParent()->getDisplay())
					dirLights.emplace_back(dirLight);
			}
		}

		std::sort(dirLights.begin(), dirLights.end(),
			[=](DirectionalLight*& a, DirectionalLight*& b)
		{
			return glm::distance(a->getWorldPosition(), getMainCamera()->getWorldPosition()) < glm::distance(b->getWorldPosition(), getMainCamera()->getWorldPosition());
		}
		);

		return dirLights;
	}

	//! checkRootEntityNameTaken()
	/*!
	\param name a const std::string& - The name of the entity
	\return a bool - Does the entity name exist in our entity list
	*/
	bool Scene::checkRootEntityNameTaken(const std::string& name)
	{
		// Check if the name given exists in the entity list
		if (m_rootEntities.find(name) != m_rootEntities.end() && name != "")
			return true;
		return false;
	}

	//! setMainCamera()
	/*!
	\param camera a Camera* - A pointer to the camera
	*/
	void Scene::setMainCamera(Camera* camera)
	{
		if (!camera)
			ENGINE_ERROR("[Scene::setMainCamera] The camera provided is not a valid camera.", (void*)camera);
		else
		{
			// Set the current main camera's value to false
			if (m_mainCamera)
				m_mainCamera->setMainCamera(false);

			// Update main camera with camera passed
			m_mainCamera = camera;
			camera->setMainCamera(true);
			camera->updateProjection();
		}
	}

	//! getMainCamera()
	/*!
	\return a Camera* - A pointer to the main camera
	*/
	Camera* Scene::getMainCamera() const
	{
		return m_mainCamera;
	}

	//! getLayerManager()
	/*!
	\return a LayerManager* - A pointer to the layer manager
	*/
	LayerManager* Scene::getLayerManager()
	{
		return m_layerManager;
	}

	//! addRenderPass()
	/*!
	\param pass a RenderPass* - The render pass to add
	*/
	void Scene::addRenderPass(RenderPass* pass)
	{
		// If the maximum number of passes has not been reached
		if (m_renderPasses.size() < m_renderPasses.capacity())
		{
			// Check to make sure the pass we're adding is valid
			if (pass)
			{
				m_renderPasses.emplace_back(pass);
				pass->attachScene(this);
				pass->setIndex(static_cast<uint32_t>(m_renderPasses.size()) - 1);
				pass->onAttach();
			}
			else
				ENGINE_ERROR("[Scene::addRenderPass] The pass we're attempting to add is invalid. Scene Name: {0}", m_sceneName);
		}
		else
			ENGINE_ERROR("[Scene:addRenderPass] The render pass limit per scene has been reached. Scene Name: {0}.", m_sceneName);
	}

	//! getRenderPasses()
	/*!
	\return a std::vector<RenderPass*>& - A list of all renderpasses for this scene
	*/
	std::vector<RenderPass*>& Scene::getRenderPasses()
	{
		return m_renderPasses;
	}

	//! getRenderPass()
	/*!
	\param index a const uint32_t - The render pass at index
	\return a RenderPass* - The render pass at index
	*/
	RenderPass* Scene::getRenderPass(const uint32_t index)
	{
		if (m_renderPasses.size() > index)
			return m_renderPasses.at(index);
		else
			ENGINE_ERROR("[Scene:getRenderPass] The index provided doesn't match with an existing render pass. Scene Name: {0}, Index: {1}.", m_sceneName, index);

		return nullptr;
	}

	//! getFinalFrameBuffer()
	/*!
	\return a FrameBuffer* - The framebuffer
	*/
	FrameBuffer* Scene::getFinalFrameBuffer()
	{
		if(m_renderPasses.back())
			return m_renderPasses.back()->getFrameBuffer();
		else
		{
			ENGINE_ERROR("[Scene::getFinalFrameBuffer] This is an invalid framebuffer. Scene Name: {0}.", m_sceneName);
			return nullptr;
		}
	}

	//! enableInspector()
	/*!
	\param value a const bool - Enable/Disable the inspector
	*/
	void Scene::enableInspector(const bool value)
	{
#ifndef NG_DEPLOY
		m_enableInspector = value;
		if (value)
		{
			getMainCamera()->updateProjection(m_viewPort);
		}
		else
		{
			setViewPort(WindowManager::getFocusedWindow()->getProperties().getSize());
			m_refreshInspector = true;
		}

#endif // NG_DEPLOY
	}

	//! getInspectorEnabled()
	/*!
	\return a const bool - Is the inspector enabled
	*/
	const bool Scene::getInspectorEnabled()
	{
#ifdef NG_DEPLOY
		return false;
#else
		return m_enableInspector;
#endif // NG_DEPLOY
		
	}

	//! getRefreshInspector()
	/*!
	\param reset a bool - Reset the inspector
	*/
	bool Scene::getRefreshInspector(bool reset)
	{
#ifdef NG_DEPLOY
		return true;
#else
		if (m_refreshInspector)
		{
			if (reset) m_refreshInspector = false;
			return true;
		}
		return false;
#endif // NG_DEPLOY
	}

	//! setViewport()
	/*!
	\param value a glm::ivec2 - The new size of the viewport
	*/
	void Scene::setViewPort(glm::ivec2 value)
	{
		m_viewPort = value;
		if (m_mainCamera != nullptr) m_mainCamera->updateProjection(m_viewPort);
		auto framebuffers = ResourceManager::getResourcesOfType<FrameBuffer>(ResourceType::FrameBuffer);
		for (auto& fbo : framebuffers)
		{
			fbo->resize(m_viewPort);
		}
	}

	//! getViewPort()
	/*!
	\return a const glm::ivec2 - The size of the viewport
	*/
	const glm::ivec2 Scene::getViewPort()
	{
		return m_viewPort;
	}

	//! setViewPos()
	/*!
	\param value a glm::vec2 - The new position of the viewport
	*/
	void Scene::setViewPos(glm::vec2 value)
	{
		m_viewPos = value;
	}

	//! getViewPos()
	/*!
	\return a const glm::vec2 - The viewport position
	*/
	const glm::vec2 Scene::getViewPos()
	{
		return m_viewPos;
	}

	//! setEntityListUpdated()
	/*!
	\param value a const bool
	*/
	void Scene::setEntityListUpdated(const bool value)
	{
		m_entityListUpdated = value;
	}

	//! getEntityListUpdated()
	/*!
	\return a const bool - Has the entity list been updated
	*/
	const bool Scene::getEntityListUpdated()
	{
		return m_entityListUpdated;
	}

	//! getPhysicsWorld()
	/*!
	\return a rp3d::PhysicsWorld* - A pointer to the physics world
	*/
	rp3d::PhysicsWorld* Scene::getPhysicsWorld()
	{
		return m_physicsWorld;
	}

	//! addPhysicsWorld()
	void Scene::addPhysicsWorld()
	{
		if(!m_physicsWorld)
			m_physicsWorld = PhysicsSystem::addWorld();
		else
			ENGINE_ERROR("[Scene:addPhysicsWorld] This scene already has a physics world. Scene Name: {0}.", m_sceneName);
	}

	//! increaseNextEntityID()
	void Scene::increaseNextEntityID()
	{
		m_entityNextID++;
	}

	//! getUniqueID()
	/*!
	\return a uint32_t - The unique ID of the entity
	*/
	uint32_t Scene::getUniqueID()
	{
		return m_entityNextID;
	}

	//! setTimeScale()
	/*!
	\param timeScale a float - The new timescale for the physics
	*/
	void Scene::setTimeScale(float timeScale)
	{
		m_timeScale = timeScale;
	}

	//! getTimeScale()
	/*!
	\return a float - The physics timescale
	*/
	float Scene::getTimeScale()
	{
		return m_timeScale;
	}

	//! setPhysicsPaused()
	/*!
	\param paused a bool - Set the physics to be paused
	*/
	void Scene::setPhysicsPaused(bool paused)
	{
		m_physicsPaused = paused;
	}

	//! getPhysicsPaused()
	/*!
	\return a bool - Is the physics paused
	*/
	bool Scene::getPhysicsPaused()
	{
		return m_physicsPaused;
	}

	//! printSceneDetails()
	void Scene::printSceneDetails()
	{
		ENGINE_TRACE("===========================================");
		ENGINE_TRACE("Scene Address: {0}", (void*)this);
		ENGINE_TRACE("Scene Name: {0}", getName());
		ENGINE_TRACE("Layer Manager Address: {0}", (void*)getLayerManager());
		ENGINE_TRACE("Number of Render Passes: {0}", getRenderPasses().size());
		ENGINE_TRACE("Number of Root Entities: {0}", m_rootEntities.size());
		ENGINE_TRACE("Physics World Address: {0}", (void*)getPhysicsWorld());
		ENGINE_TRACE("Main Camera Address: {0}", (void*)getMainCamera());
#ifndef NG_DEPLOY
		ENGINE_TRACE("Inspector Enabled: {0}", m_enableInspector);
#endif // NG_DEPLOY
		ENGINE_TRACE("Entity List Updated: {0}", m_entityListUpdated);
		ENGINE_TRACE("Physics Paused: {0}", m_physicsPaused);
		ENGINE_TRACE("Physics Timescale: {0}", m_timeScale);
		ENGINE_TRACE("Scheduled for Deletion: {0}", getDestroyed());
		ENGINE_TRACE("===========================================");
	}
}