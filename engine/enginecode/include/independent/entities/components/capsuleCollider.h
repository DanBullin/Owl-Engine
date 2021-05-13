/*! \file sphereCollider.h
*
* \brief A sphere collider entity component that attaches to a rigidbody. Uses ReactPhysics3D.
*
* \author Jamie Hall & Aaron Taleghani
*
*/
#ifndef CAPSULECOLLIDER_H
#define CAPSULECOLLIDER_H


#include <glm/glm.hpp>
#include "independent/entities/entityComponent.h"
#include "independent/entities/components/rigidBody.h"
#include "independent/systems/system.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/physicsSystem.h"
#include <reactphysics3d/reactphysics3d.h>

namespace OwlEngine
{
	/*! \class CapsuleCollider
	* \brief A capsule collider entity component that attaches to a rigidbody. Uses ReactPhysics3D.
	*/
	class CapsuleCollider : public EntityComponent
	{
	private:
		glm::vec3 m_position; //!< Collider position as glm vec3
		float m_radius; //!< Radius of the capsules shapes hemispheres
		float m_height; //!< Distance betweenthe centre of each hemisphere
		rp3d::Collider* m_collider; //!< Pointer to collider object
		rp3d::CapsuleShape* m_shape; //!< Pointer to the colliders shape
		PhysicsSystem* m_system; //!< Pointer to the physics system
		bool m_isTrigger = false; //!< Is this collider a trigger
		bool m_notAttached = false; //!< Is this component attached properly
	public:
		CapsuleCollider(const float radius, const float height, glm::vec3 position); //!< Constructor
		void onAttach() override;	//!< Occurs when component is attached to an entity
		void onDetach() override;	//!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override;	//!< Update the component
		void printComponentDetails() override;	//!< Print component details
		virtual  nlohmann::ordered_json getAsJSON() override; //!< Get the component's data as JSON for saving
#ifndef NG_DEPLOY
		virtual void displayImGui() override; //!< Get the component's data displayed in ImGui form
#endif // NG_DEPLOY

		glm::vec3 getPosition(); //!< Returns the local position of the collider
		void setPosition(glm::vec3 position); //!< Sets the local position of the collider

		float getRadius(); //!< Returns the radius of the collider shape
		void setRadius(const float radius); //!< Sets the radius of the collider shape

		float getHeight(); //!< Returns the height of the shape
		void setHeight(const float height); //!< Sets the height of the shape

		bool getIsTrigger(); //!< Returns true if collider is trigger
		void setIsTrigger(bool isTrigger); //!< Set the collider as a trigger

		void onRender(); //!< Draw the collider
	};
}

#endif