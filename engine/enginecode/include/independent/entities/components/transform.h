/*! \file transform.h
*
* \brief The position, scale and rotation of an 3D entity
*
* \author Daniel Bullin
*
*/
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "independent/entities/entityComponent.h"

namespace OwlEngine
{
	/*! \class Transform
	* \brief The position, scale and rotation of an 3D object in the world
	*/
	class Transform : public EntityComponent
	{
	private:
		glm::vec3 m_position; //!< Position of the entity
		glm::vec3 m_orientation; //!< Orientation of the entity
		glm::vec3 m_scale; //!< Scale of the entity

		glm::vec3 m_forward; //!< forward of the entity
		glm::vec3 m_up; //!< up of the entity 

		bool m_inheritRotation; //!< Whether the entity gains roation from its parent
		bool m_rotationalPosition; //!< Whether the entity's position changes with parent rotation

	public:
		Transform(const float xPos, const float yPos, const float zPos, const float xRotation, const float yRotation, const float zRotation, const float sX, const float sY, const float sZ,
			const float forwardX, const float forwardY, const float forwardZ, const float upX, const float upY, const float upZ); //!< Constructor requiring all transform types
		~Transform(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the component
		void printComponentDetails() override; //!< Print component details
		virtual nlohmann::ordered_json getAsJSON() override; //!< Get the component in JSON form for saving
#ifndef NG_DEPLOY
		virtual void displayImGui() override; //!< Get the component's data displayed in ImGui form
#endif // NG_DEPLOY

		glm::vec3 getPosition(); //!< Get the position of the entity in the world
		glm::vec3 getLocalPosition(); //!< Get the position of the entity in the world
		void setPosition(const float xPos, const float yPos, const float zPos); //!< Set the position of the entity in the world
		void setPosition(const glm::vec3& newPos); //!< Set the position of the entity in the world
		void setWorldPosition(const glm::vec3& newPos); //!< Set the position of the entity in the world

		glm::vec3 getOrientation(); //!< Get the orientation of the entity in the world
		glm::quat getOrientationAsQuat(); //!< Get the orientation of the entity as a quaternion
		void setOrientation(const float xRotation, const float yRotation, const float zRotation); //!< Set the orientation of the entity in the world
		void setOrientation(const glm::vec3& newOrientation); //!< Set the orientation of the entity in the world
		void setOrientation(const glm::quat& newOrientation); //!< Set the orientation of the entity from a quaternion

		glm::vec3 getScale(); //!< Get the scale of the entity in the world
		void setScale(const float sX, const float sY, const float sZ); //!< Set the scale of the entity in the world
		void setScale(const glm::vec3& newScale); //!< Set the scale of the entity in the world

		glm::vec3 getForward(); //!< Get the forward of the entity in the world
		void setForward(const float xPos, const float yPos, const float zPos); //!< Set the forward of the entity in the world
		void setForward(const glm::vec3& newForward); //!< Set the forward of the entity in the world

		glm::vec3 getUp(); //!< Get the up of the entity in the world
		void setUp(const float xPos, const float yPos, const float zPos); //!< Set the up of the entity in the world
		void setUp(const glm::vec3& newUp); //!< Set the up of the entity in the world

		void setRotationalPositionEnabled(bool rotPos); //!< Lets the user modify the bool
		void setInheritRotation(bool inherit); //!< Lets the user modify the bool

		float distance(Transform* otherTransform); //!< Get the distance between this transform and the position of another
		float angle(Transform* otherTransform); //!< Get the angle between this transform and another based on camera's front direction

		glm::mat4 getModelMatrix(); //!< Get the model matrix of the geometry
		glm::mat4 getAudioMatrix(); //!< Get the matrix for audio listeners and events
	};
}
#endif