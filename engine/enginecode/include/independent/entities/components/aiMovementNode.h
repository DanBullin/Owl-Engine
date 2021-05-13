/*! \file AIMovementNode.h
*
* \brief A node used for moving entity navigation
*
* \author Jamie Hall
*
*/
#ifndef AIMOVEMENTNODE_H
#define AIMOVEMENTNODE_H


#include <glm/glm.hpp>
#include "independent/entities/entityComponent.h"
#include "independent/systems/systems/log.h"
namespace OwlEngine
{
	/*! \class AIMovementNode
	* \brief A sphere collider entity component that attaches to a rigidbody. Uses ReactPhysics3D.
	*/
	class AIMovementNode : public EntityComponent
	{
	private:
		std::string m_attachedEntityName; //!< Pointer to the entity which uses this node
	public:
		AIMovementNode(const std::string& attachedEntName);	//!< Constructor
		~AIMovementNode();	//!< Deconstructor
		void onAttach() override;	//!< Occurs when component is attached to an entity
		void onDetach() override;	//!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override;	//!< Update the component
		void printComponentDetails() override;	//!< Print component details
		virtual  nlohmann::ordered_json getAsJSON() override; //!< Get the component's data as JSON for saving
#ifndef NG_DEPLOY
		virtual void displayImGui() override; //!< Get the component's data displayed in ImGui form
#endif // NG_DEPLOY

		const std::string& getAttachedEntityName();	//!< Get the attached entities name
		void setAttachedEntity(const std::string& newEntityName);	//!< Set the attached entities name
	};
}

#endif