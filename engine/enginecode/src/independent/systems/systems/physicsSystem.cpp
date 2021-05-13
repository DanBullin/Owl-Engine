/*! \file physicsSystem.cpp
*
* \brief A physics system that initialises and updates a ReactPhysics3D physics world.
*
* \author Jamie Hall & Aaron Taleghani
*
*/
#include "engine_pch.h"
#include "independent/systems/systems/physicsSystem.h"
#include "independent/systems/components/scene.h"
#include "independent/systems/systems/log.h"

namespace OwlEngine
{
	bool PhysicsSystem::s_enabled = false; //!< Initialise with a default value of false
	std::vector<rp3d::PhysicsWorld*> PhysicsSystem::s_physicsWorlds; //!< List of all physics worlds
	rp3d::PhysicsCommon PhysicsSystem::s_physicsCommon; //!< React physics common object
	PhysicsEventListener PhysicsSystem::s_listener; //!< Custom event listener

	//! PhysicsSystem()
	PhysicsSystem::PhysicsSystem() : System(SystemType::Physics)
	{
	}

	//! ~PhysicsSystem()
	PhysicsSystem::~PhysicsSystem()
	{
	}

	//! start()
	void PhysicsSystem::start()
	{
		// Start the system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[PhysicsSystem::start] Starting the physics system.");
			s_enabled = true;
		}
	}

	//! stop()
	void PhysicsSystem::stop()
	{
		// Start the system if its disabled
		if (s_enabled)
		{
			ENGINE_INFO("[PhysicsSystem::stop] Stopping the physics system.");

			for (auto& world : s_physicsWorlds)
				s_physicsCommon.destroyPhysicsWorld(world);

			s_physicsWorlds.clear();

			s_enabled = false;
		}
	}

	//! addWorld()
	/*!
	\return a rp3d::PhysicsWorld* - A pointer to the new world
	*/
	rp3d::PhysicsWorld* PhysicsSystem::addWorld()
	{
		s_physicsWorlds.push_back(s_physicsCommon.createPhysicsWorld());
		s_physicsWorlds.back()->setEventListener(&s_listener);
		s_physicsWorlds.back()->setGravity({ 0.f, -9.81f, 0.f});
		return s_physicsWorlds.back();
	}

	//! reactToGLM()
	/*!
	\param a rp3d::Vector3 - React physics vector 3
	\return a glm::vec3 - Converted GLM vector 3
	*/
	glm::vec3 PhysicsSystem::reactToGLM(rp3d::Vector3 vec)
	{
		return { vec.x, vec.y, vec.z };
	}

	//! GLMToReact()
	/*!
	\param a glm::vec3 - GLM vector 3
	\return a rp3d::Vector3 - Converted react physics vector 3
	*/
	rp3d::Vector3 PhysicsSystem::GLMToReact(glm::vec3 vec)
	{
		return { vec.x, vec.y, vec.z };
	}

	//! removeWorld()
	/*!
	\param world a rp3d::PhysicsWorld* - A pointer to the world
	*/
	void PhysicsSystem::removeWorld(rp3d::PhysicsWorld* world)
	{
		for (auto& world : s_physicsWorlds)
		{
		}
	}

	//! onUpdate()
	/*!
	\param world a rp3d::PhysicsWorld* - A pointer to the physics world
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void PhysicsSystem::onUpdate(rp3d::PhysicsWorld* world, Scene* scene, const float timestep, const float totalTime)
	{
		if (scene) s_listener.setCurrentScene(scene);

		if (world) world->update(timestep);
		else
			ENGINE_ERROR("[PhysicsSystem::onUpdate] Invalid physics world was provided.");
	}

	//! getPhysicsWorld()
	/*!
	\param index a const uint32_t - The index of the physics world
	\return a rp3d::PhysicsWorld* - Pointer to the physics world
	*/
	rp3d::PhysicsWorld * PhysicsSystem::getPhysicsWorld(const uint32_t index)
	{
		return s_physicsWorlds.at(index);
	}

	//! getPhysicsCommon()
	/*!
	\return a rp3d::PhysicsCommon& - Reference to the physics common object
	*/
	rp3d::PhysicsCommon& PhysicsSystem::getPhysicsCommon()
	{
		return s_physicsCommon;
	}

	// EVENT LISTENER

	Scene* PhysicsEventListener::s_currentScene = nullptr; //!< Initialise with null pointer

	//! setCurrentScene
	/*!
	\param scene a Scene* - The currently active scene
	*/
	void PhysicsEventListener::setCurrentScene(Scene* scene)
	{
		s_currentScene = scene;
	}

	//! onContact()
	/*!
	\param callbackData a const CollisionCallback::CallbackData& - Callback data for the collision
	*/
	void PhysicsEventListener::onContact(const CollisionCallback::CallbackData & callbackData)
	{
		for (uint32_t p = 0; p < callbackData.getNbContactPairs(); p++)
		{
			CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

			uint32_t index1 = reinterpret_cast<uint32_t>(contactPair.getBody1()->getUserData());
			uint32_t index2 = reinterpret_cast<uint32_t>(contactPair.getBody2()->getUserData());

			// Call native script onContact functions
			if (s_currentScene)
			{
				Entity* entity1 = s_currentScene->getEntity(index1);
				Entity* entity2 = s_currentScene->getEntity(index2);

				if (entity1)
				{
					if (entity2)
					{
						if (entity1->containsComponent<NativeScript>())
							entity1->getComponent<NativeScript>()->onContact(entity2);

						if (entity2->containsComponent<NativeScript>())
							entity2->getComponent<NativeScript>()->onContact(entity1);
					}
				}
			}
		}
	}

	//! onTrigger()
	/*!
	\param callbackData a const rp3d::OverlapCallback::CallbackData& - Callback data for the overlap
	*/
	void PhysicsEventListener::onTrigger(const rp3d::OverlapCallback::CallbackData & callbackData)
	{
		for (uint32_t p = 0; p < callbackData.getNbOverlappingPairs(); p++)
		{
			rp3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);

			uint32_t index1 = reinterpret_cast<uint32_t>(overlapPair.getBody1()->getUserData());
			uint32_t index2 = reinterpret_cast<uint32_t>(overlapPair.getBody2()->getUserData());

			// Call native script onTrigger functions
			if (s_currentScene)
			{
				Entity* entity1 = s_currentScene->getEntity(index1);
				Entity* entity2 = s_currentScene->getEntity(index2);

				if (entity1)
				{
					if (entity2)
					{
						if (entity1->containsComponent<NativeScript>())
							entity1->getComponent<NativeScript>()->onTrigger(entity2);

						if (entity2->containsComponent<NativeScript>())
							entity2->getComponent<NativeScript>()->onTrigger(entity1);
					}
				}
			}
		}
	}
}