/*! \file scene.h
*
* \brief A scene contains all the data associated in a particular environment. This data will be loaded by the SceneLoader.
*
* \author Daniel Bullin
*
*/
#ifndef SCENE_H
#define SCENE_H

#include "independent/core/common.h"
#include "independent/core/destroyable.h"
#include "independent/entities/entity.h"
#include "independent/layers/layerManager.h"
#include "independent/rendering/renderPasses/renderPass.h"
#include "independent/systems/systems/physicsSystem.h"
#include <reactphysics3d/reactphysics3d.h>

namespace OwlEngine
{
	/*! \class Scene
	* \brief A scene class which contains all entities and world data
	*/
	class Scene : public Destroyable
	{
	private:
		std::string m_sceneName; //!< The name of the scene
		std::string m_sceneFolderPath; //!< The filepath to the scene folder
		LayerManager* m_layerManager; //!< A layer manager for the scene
		std::vector<RenderPass*> m_renderPasses; //!< A list of all render passes for the scene
		std::map<std::string, Entity*> m_rootEntities; //!< List of all root entities in the scene
		uint32_t m_entityNextID; //!< The next ID for a new entity
		rp3d::PhysicsWorld* m_physicsWorld; //!< A pointer to the physics world
		Camera* m_mainCamera; //!< The current main camera
		
		glm::ivec2 m_viewPort; //!< actual size of the scene
		glm::vec2 m_viewPos; //!< The position of the scene view;
#ifndef NG_DEPLOY
		bool m_enableInspector; //!< Enable inspector
		bool m_refreshInspector; //!< Tell the inspector to throw out any stored pointers
#endif // NG_DEPLOY
		bool m_entityListUpdated; //!< Has the entity list been updated
		bool m_physicsPaused; //!< Does the physics run
		float m_timeScale; //!< How fast time goes for physics

		std::vector<Entity*> m_entitiesList; //!< The list of entities in vector format
		Entity* getEntity(Entity* entity, const uint32_t uniqueID); //!< Get an entity in the scene (Includes children)
	public:
		Scene(const std::string& name, const std::string& sceneFolderPath); //!< Constructor
		~Scene(); //!< Destructor

		const std::string& getName(); //!< Get the name of the scene
		const std::string& getFolderPath(); //!< Get the file path to the scene folder

		void onUpdate(const float timestep, const float totalTime); //!< Update the scene

		void addEntity(const std::string& name, Entity* entity); //!< Add an entity to the scene
		Entity* getEntity(const std::string& name); //!< Get an entity in the scene (Doesn't include children, you must follow the tree structure)
		Entity* getEntity(const uint32_t uniqueID); //!< Get an entity in the scene (Includes children)
		std::map<std::string, Entity*> getRootEntities() const; //!< Get only the root entities of the scene
		void cleanRoot(std::string name = ""); //! Checks if root entities have parents and removes those that do (user can specify singular entities to act upon)
		std::vector<Entity*> getEntities(); //!< Get a list of all entities in the scene

		std::vector<PointLight*> getClosestPointLights(); //!< Get a list of the closest point lights
		std::vector<SpotLight*> getClosestSpotLights(); //!< Get a list of the closest spot lights
		std::vector<DirectionalLight*> getClosestDirectionalLights(); //!< Get a list of the closest directional lights

		bool checkRootEntityNameTaken(const std::string& name); //!< Returns whether the entity name has been taken

		void setMainCamera(Camera* camera); //!< Set the main camera
		Camera* getMainCamera() const; //!< Get the main camera

		LayerManager* getLayerManager(); //!< Get the layer manager

		void addRenderPass(RenderPass* pass); //!< Add a render pass to the list of passes
		std::vector<RenderPass*>& getRenderPasses(); //!< Get the list of render passes
		RenderPass* getRenderPass(const uint32_t index); //!< Get the render pass at index
		FrameBuffer* getFinalFrameBuffer(); //!< Get the final framebuffer in the list of passes for this scene

		void enableInspector(const bool value); //!< Enable/Disable the inspector
		const bool getInspectorEnabled(); //!< Get whether the inspector is enabled
		bool getRefreshInspector(bool reset); //!< Check whether the inspector needs to refresh

		void setViewPort(glm::ivec2 value); //!< Set the viewport to be a certain size
		const glm::ivec2 getViewPort(); //!< Get the size of the viewport

		void setViewPos(glm::vec2 value); //!< Set the position of the scene view
		const glm::vec2 getViewPos(); //!< Get the position of the scene view

		void setEntityListUpdated(const bool value); //!< Confirm whether the entities in the scene have been updated
		const bool getEntityListUpdated(); //!< Get whether the entities have been updated

		rp3d::PhysicsWorld* getPhysicsWorld(); //!< Get the scene's physics world
		void addPhysicsWorld(); //!< Add the scene's physics world

		void increaseNextEntityID(); //!< Increase the unique ID by 1 for the next entity
		uint32_t getUniqueID(); //!< Get the next unique ID for an entity

		void setTimeScale(float timeScale); //!< Sets how fast the time runs in the scene
		float getTimeScale(); //!< Gets the timeScale (how fast time runs in the scene)
		void setPhysicsPaused(bool paused); //!< Sets whether physics is paused or not
		bool getPhysicsPaused(); //!< Gets whether physics is paused or not

		void printSceneDetails(); //!< Print scene details
	};
}
#endif