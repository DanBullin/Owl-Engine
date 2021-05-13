/*! \file rigidBody.h
*
* \brief A RigiBody component that allows physics to affect the attached entity. Uses ReactPhysics3D.
*
* \author Jamie Hall & Aaron Taleghani
*
*/
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>
#include "independent/core/common.h"
#include "independent/entities/entityComponent.h"
#include "independent/systems/systems/physicsSystem.h"
#include <reactphysics3d/reactphysics3d.h>

namespace OwlEngine
{
	namespace BodyTypes
	{
		/*! \enum RigidBodyType
		* \brief The different types of RigidBodies
		*/
		enum RigidBodyType {
			STATIC,
			KINEMATIC,
			DYNAMIC,
			PLAYER
		};
	}

	/*! \class RigidBody
	* \brief A RigidBody component that allows physics to affect the attatched entity. Uses ReactPhysics3D.
	*/
	class RigidBody : public EntityComponent
	{
	private:
		BodyTypes::RigidBodyType m_bodyType; //!< Rigidbody type
		rp3d::RigidBody* m_body; //!< Pointer to the react physics body
		rp3d::PhysicsWorld* m_world; //!< Pointer to the react physics world
		PhysicsSystem* m_system; //!< The physics system
		void* m_userData; //!< Pointer to rigidbody userdata
		bool m_notAttached = false; //!< Has this been attached properly
		bool m_canSleep = true; //!< Can this rigidbody sleep
	public:	
		RigidBody(rp3d::PhysicsWorld* world, BodyTypes::RigidBodyType type, bool canSleep = true); //!< Constructor

		rp3d::RigidBody* getBody(); //!< Returns a pointer to the rigidbody
		BodyTypes::RigidBodyType getType(); //!< Returns the rigidbody type

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the rigid body
		void printComponentDetails() override; //!< Print component details

		void setVelocity(const glm::vec3 front); //!< Set the RigidBodys velocity
		void setVelocityUp(const glm::vec3 up); //!< Set the RigidBodys velocity, ignoring the Y constraint

		void setType(BodyTypes::RigidBodyType type); //!< Sets the rigidbody's type

		void applyForce(float velocity); //!< Apply a force to the RigidBody
		virtual  nlohmann::ordered_json getAsJSON() override; //!< Get the component's data as JSON for saving
#ifndef NG_DEPLOY
		virtual void displayImGui() override; //!< Get the component's data displayed in ImGui form
#endif // NG_DEPLOY
	};
}

#endif