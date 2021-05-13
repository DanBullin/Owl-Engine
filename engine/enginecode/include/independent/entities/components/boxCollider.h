/*! \file boxCollider.h
*
* \brief A box collider entity component that attaches to a rigidbody. Uses ReactPhysics3D.
*
* \author Jamie Hall & Aaron Taleghani
*
*/
#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <glm/glm.hpp>
#include "independent/entities/entityComponent.h"
#include "independent/entities/components/rigidBody.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/physicsSystem.h"
#include <reactphysics3d/reactphysics3d.h>

namespace OwlEngine
{
	/*! \class BoxCollider
	* \brief A box collider entity component that attaches to a rigidbody. Uses ReactPhysics3D.
	*/
	class BoxCollider : public EntityComponent
	{
	private:
		glm::vec3 m_position; //!< Collider position as glm vec3
		glm::vec3 m_halfExtents; //!< Half extents of box shape
		rp3d::Collider* m_collider; //!< Pointer to collider object
		rp3d::BoxShape* m_shape; //!< Pointer to the colliders shape
		PhysicsSystem* m_system; //!< Pointer to the physics system
		bool m_isTrigger = false; //!< Is this collider a trigger
		bool m_notAttached = false; //!< Is this component attached properly
	public:
		BoxCollider(glm::vec3& halfExtents, glm::vec3 position); //!< Constructor
		void onAttach() override;	//!< Occurs when component is attached to an entity
		void onDetach() override;	//!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override;	//!< Update the component
		void printComponentDetails() override;	//!< Print component details
		nlohmann::ordered_json getAsJSON() override; //!< Get the component's data as JSON for saving
#ifndef NG_DEPLOY
		void displayImGui() override; //!< Get the component's data displayed in ImGui form
#endif // NG_DEPLOY

		glm::vec3 getPosition(); //!< Returns the local position of the collider
		void setPosition(glm::vec3 position); //!< Sets the local position of the collider

		glm::vec3 getHalfExtents(); //!< Returns the half extents of the collider shape
		void setHalfExtents(glm::vec3& halfExtents); //!< Sets the half extents of the collider shape

		bool getIsTrigger(); //!< Returns true if collider is a trigger
		void setIsTrigger(bool isTrigger); //!< Set the collider as a trigger

		void onRender(); //!< Draw the collider

		bool containsPoint(const glm::vec3& point); //!< Contains a point in the geometry
		bool vectorIntersects(const glm::vec3& pos, const glm::vec3& vec, const float vecMultiplier); //!< Contains a vector in the geometry
	};
}

#endif