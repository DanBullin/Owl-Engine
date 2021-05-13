/*! \file physicsSystem.h
*
* \brief A physics system that initialises and updates a ReactPhysics3D physics world.
*
* \author Jamie Hall & Aaron Taleghani
*
*/
#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "independent/systems/system.h"
#include "independent/systems/systems/eventManager.h"
#include <reactphysics3d/reactphysics3d.h>

namespace OwlEngine
{
	/*! \class PhysicsEventListener
	* \brief An event listener used to handle physics body collisions
	*/
	class PhysicsEventListener : public rp3d::EventListener
	{
	private:
		static Scene* s_currentScene; //!< Pointer to the currently active scene
	public:
		void setCurrentScene(Scene* scene); //!< Set the currently active scene
		virtual void onContact(const CollisionCallback::CallbackData& callbackData) override; //!< Occurs when two collider objects collide with each other
		virtual void onTrigger(const rp3d::OverlapCallback::CallbackData& callbackData) override; //!< Occurs when a trigger passes through a collider
	};

	/*! \class PhysicsSystem
	* \brief A physics system that initialises and updates a ReactPhysics3D physics world.
	*/
	class PhysicsSystem : public System
	{
	private:
		static bool s_enabled; //!< Is the system enabled?
		static std::vector<rp3d::PhysicsWorld*> s_physicsWorlds; //!< List of all physics worlds
		static rp3d::PhysicsCommon s_physicsCommon; //!< React physics common object
		static PhysicsEventListener s_listener; //!< Custom event listener
	public:
		PhysicsSystem(); //!< Constructor
		~PhysicsSystem(); //!< Deconstructor

		void start() override; //!< Create physics world
		void stop() override; //!< Destroy physics world
		static rp3d::PhysicsWorld* addWorld(); //!< Create a new physics world
		static glm::vec3 reactToGLM(rp3d::Vector3 vec); //!< Convert a rp3D Vector3 to a glm vec3
		static rp3d::Vector3 GLMToReact(glm::vec3 vec); //!< Convert a GLM vec3 to a rp3D Vector3
		static void removeWorld(rp3d::PhysicsWorld* world); //!< Delete a new physics world
		static void onUpdate(rp3d::PhysicsWorld* world, Scene* scene, const float timestep, const float totalTime); //!< Update physics world
		static rp3d::PhysicsWorld* getPhysicsWorld(const uint32_t index); //!< Getter for physics world
		static rp3d::PhysicsCommon& getPhysicsCommon(); //!< Getter for physics common object
	};
}
#endif