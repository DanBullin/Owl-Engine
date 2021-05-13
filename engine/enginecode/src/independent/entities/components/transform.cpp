/*! \file transform.cpp
*
* \brief The position, scale and rotation of an 3D entity
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/entities/components/transform.h"
#include "independent/entities/components/camera.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/sceneManager.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! Transform()
	/*!
	\param xPos a const float - The x position of the entity in the game world
	\param yPos a const float - The y position of the entity in the game world
	\param zPos a const float - The z position of the entity in the game world
	\param xRotation a const float - The rotation around the x axis of the entity in the game world
	\param yRotation a const float - The rotation around the y axis of the entity in the game world
	\param zRotation a const float - The rotation around the z axis of the entity in the game world
	\param sX a const float - The scale in the x axis of the entity in the game world
	\param sY a const float - The scale in the y axis of the entity in the game world
	\param sZ a const float - The scale in the z axis of the entity in the game world
	*/
	Transform::Transform(const float xPos, const float yPos, const float zPos, const float xRotation, const float yRotation, const float zRotation, const float sX, const float sY, const float sZ, const float forwardX, const float forwardY, const float forwardZ, const float upX, const float upY, const float upZ)
		: EntityComponent(ComponentType::Transform)
	{
		setPosition(xPos, yPos, zPos);
		setOrientation(xRotation, yRotation, zRotation);
		setScale(sX, sY, sZ);
		setForward(forwardX, forwardY, forwardZ);
		setUp(upX, upY, upZ);

		m_inheritRotation = true;
		m_rotationalPosition = false;
	}

	//! ~Transform()
	Transform::~Transform()
	{
	}

	//! onAttach()
	void Transform::onAttach()
	{
	}

	//! onDetach
	void Transform::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void Transform::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void Transform::printComponentDetails()
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[Transform::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("====================================================");
		ENGINE_TRACE("Transform Details for Entity: {0}", parent->getName());
		ENGINE_TRACE("====================================================");
		ENGINE_TRACE("Position: {0}, {1}, {2}", m_position.x, m_position.y, m_position.z);
		ENGINE_TRACE("Orientation: {0}, {1}, {2}", m_orientation.x, m_orientation.y, m_orientation.z);
		ENGINE_TRACE("Scale: {0}, {1}, {2}", m_scale.x, m_scale.y, m_scale.z);
		ENGINE_TRACE("====================================================");
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json Transform::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["position"] = { m_position.x, m_position.y, m_position.z };
		data["rotation"] = { m_orientation.x, m_orientation.y, m_orientation.z };
		data["scale"] = { m_scale.x, m_scale.y, m_scale.z };
		data["forward"] = { m_forward.x, m_forward.y, m_forward.z };
		data["up"] = { m_up.x, m_up.y, m_up.z };

		data["inherit"] = m_inheritRotation;
		data["rotPos"] = m_rotationalPosition;

		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void Transform::displayImGui()
	{
		if (ImGui::DragFloat3("position###pos1", &m_position.x, 0.1f, 0.f, 0.f, "%.2f")) setChanged(true);
		if (ImGui::DragFloat3("rotation###rot1", &m_orientation.x, 1.f, 0.f, 0.f, "%.0f")) setChanged(true);
		if (ImGui::DragFloat3("scale###scal1", &m_scale.x, 0.05f, 0.0f, 999999.f, "%.2f")) setChanged(true);
		if (ImGui::DragFloat3("forward###for1", &m_forward.x, 0.05f, 0.0f, 999999.f, "%.2f")) setChanged(true);
		if (ImGui::DragFloat3("up###up1", &m_up.x, 0.05f, 0.0f, 999999.f, "%.2f")) setChanged(true);
		ImGui::Checkbox("changed###chang1", &m_changed);

		ImGui::Checkbox("inherit rotation###inherit1", &m_inheritRotation);
		ImGui::Checkbox("rotational Position###rotpos1", &m_rotationalPosition);
	}
#endif // NG_DEPLOY

	//! getPosition()
	/*!
	\return a glm::vec3 - The position of the entity in the game world
	*/
	glm::vec3 Transform::getPosition()
	{
		if (getParent()->getParentEntity())
			return getParent()->getParentEntity()->getComponent<Transform>()->getPosition() + m_position;
		else
			return m_position;
	}

	glm::vec3 Transform::getLocalPosition()
	{
		return m_position;
	}

	//! setPosition()
	/*!
	\param xPos a const float - The x position of the entity
	\param yPos a const float - The y position of the entity
	\param zPos a const float - The z position of the entity
	*/
	void Transform::setPosition(const float xPos, const float yPos, const float zPos)
	{
		setPosition({ xPos, yPos, zPos });
	}

	//! setPosition()
	/*!
	\param newPos a const glm::vec3& - The position of the entity
	*/
	void Transform::setPosition(const glm::vec3& newPos)
	{
		m_position = newPos;
	}

	void Transform::setWorldPosition(const glm::vec3 & newPos)
	{
		if (getParent()->getParentEntity())
			m_position = newPos - getParent()->getParentEntity()->getComponent<Transform>()->getPosition();
	}

	//! getOrientation()
	/*!
	\return a glm::vec3 - The orientation of the transform in euler angles
	*/
	glm::vec3 Transform::getOrientation()
	{
		if (getParent()->getParentEntity() && m_inheritRotation)
			return getParent()->getParentEntity()->getComponent<Transform>()->getOrientation() + m_orientation;
		else
			return m_orientation;
	}

	//! getOrientationAsQuat()
	/*!
	\return a glm::quat - The orientation of the transform
	*/
	glm::quat Transform::getOrientationAsQuat()
	{
		if (getParent()->getParentEntity() && m_inheritRotation)
			return glm::quat((getParent()->getParentEntity()->getComponent<Transform>()->getOrientation() + m_orientation) * 3.14f/180.f);
		else
			return glm::quat(m_orientation * (3.14f/180.f) );
	}

	//! setOrientation()
	/*!
	\param xRotation a const float - The rotation around the x axis of the entity
	\param yRotation a const float - The rotation around the y axis of the entity
	\param zRotation a const float - The rotation around the z axis of the entity
	*/
	void Transform::setOrientation(const float xRotation, const float yRotation, const float zRotation)
	{
		setOrientation({ xRotation, yRotation, zRotation });
	}

	//! setOrientation()
	/*!
	\param newOrientation a const glm::vec3& - The orientation of the entity
	*/
	void Transform::setOrientation(const glm::vec3& newOrientation)
	{
		m_orientation = newOrientation;
	}

	//! setOrientation()
	/*!
	\param newOrientation a const glm::quat& - The orientation of the entity
	*/
	void Transform::setOrientation(const glm::quat& newOrientation)
	{
		glm::vec3 eulerOrientation = glm::eulerAngles(newOrientation) * 180.f/3.14f;
		m_orientation = eulerOrientation;
	}

	//! getScale()
	/*!
	\return a glm::vec3& - The scale of the transform
	*/
	glm::vec3 Transform::getScale()
	{
		if (getParent()->getParentEntity())
			return getParent()->getParentEntity()->getComponent<Transform>()->getScale() * m_scale;
		else
			return m_scale;
	}

	//! setScale()
	/*!
	\param sX a const float - The scale of the transform in the x axis
	\param sY a const float - The scale of the transform in the y axis
	\param sZ a const float - The scale of the transform in the z axis
	*/
	void Transform::setScale(const float sX, const float sY, const float sZ)
	{
		setScale({ sX, sY, sZ });
	}

	//! setScale()
	/*!
	\param newScale a const glm::vec3& - The scale of the entity
	*/
	void Transform::setScale(const glm::vec3& newScale)
	{
		if (newScale.x >= 0.f && newScale.y >= 0.f && newScale.z >= 0.f)
			m_scale = newScale;
		else
			ENGINE_ERROR("[Transform::setScale] An invalid scale value was provided. Scale: {0}, {1}, {2}.", newScale.x, newScale.y, newScale.z);
	}

	//! getForward()
	/*!
	\return a glm::vec3 - The forward of the entity in the game world
	*/
	glm::vec3 Transform::getForward()
	{
		if (getParent()->getParentEntity())
			return getParent()->getParentEntity()->getComponent<Transform>()->getForward() + m_forward;
		else
			return m_forward;
	}

	//! setForward()
	/*!
	\param xFor a const float - The x forward of the entity
	\param yFor a const float - The y forward of the entity
	\param zFor a const float - The z forward of the entity
	*/
	void Transform::setForward(const float xFor, const float yFor, const float zFor)
	{
		setForward({ xFor, yFor, zFor });
	}

	//! setForward()
	/*!
	\param newForward a const glm::vec3& - The forward of the entity
	*/
	void Transform::setForward(const glm::vec3 & newForward)
	{
		m_forward = newForward;
	}

	//! getUp()
	/*!
	\return a glm::vec3 - The up of the entity in the game world
	*/
	glm::vec3 Transform::getUp()
	{
		if (getParent()->getParentEntity())
			return getParent()->getParentEntity()->getComponent<Transform>()->getUp() + m_up;
		else
			return m_up;
	}

	//! setForward()
	/*!
	\param xUp a const float - The x up of the entity
	\param yUp a const float - The y up of the entity
	\param zUp a const float - The z up of the entity
	*/
	void Transform::setUp(const float xUp, const float yUp, const float zUp)
	{
		setUp({ xUp, yUp, zUp });
	}

	//! setForward()
	/*!
	\param newUp a const glm::vec3& - The up of the entity
	*/
	void Transform::setUp(const glm::vec3 & newUp)
	{
		m_up = newUp;
	}

	void Transform::setRotationalPositionEnabled(bool rotPos)
	{
		m_rotationalPosition = rotPos;
	}

	void Transform::setInheritRotation(bool inherit)
	{
		m_inheritRotation = inherit;
	}

	// distance()
	/*!
	\param otherTransform a Transform* - The other transform
	\return a float - The distance between the two transforms in world space
	*/
	float Transform::distance(Transform* otherTransform)
	{
		return glm::distance(getPosition(), otherTransform->getPosition());
	}

	// angle()
	/*!
	\param otherTransform a Transform* - The other transform
	\return a float - The angle between the two transforms using the camera's front direction
	*/
	float Transform::angle(Transform* otherTransform)
	{
		auto camDir = SceneManager::getActiveScene()->getMainCamera()->getCameraData().Front;
		glm::vec3 playerCamVector = glm::normalize((getPosition() + camDir) - getPosition());
		glm::vec3 playerObjVector = glm::normalize(otherTransform->getPosition() - getPosition());
		return glm::degrees(glm::acos(glm::dot(playerCamVector, playerObjVector)));
	}

	//! getModelMatrix()
	/*!
	\return a glm::mat4 - The model transformation matrix
	*/
	glm::mat4 Transform::getModelMatrix()
	{
		// Order: Translate then Rotation then Scale
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, getPosition());
		model = glm::rotate(model, glm::radians(getOrientation().x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(getOrientation().y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(getOrientation().z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, getScale());
		return model;
	}

	//! getAudioMatrix()
	/*!
	\return a glm::mat4 - The audio transformation matrix
	*/
	glm::mat4 Transform::getAudioMatrix()
	{
		glm::mat4 audio = glm::mat4(1.f);
		audio[3][0] = getPosition().x; audio[3][1] = getPosition().y; audio[3][2] = getPosition().z;
		audio[2][0] = getForward().x; audio[2][1] = getForward().y; audio[2][2] = getForward().z;
		audio[1][0] = getUp().x; audio[1][1] = getUp().y; audio[1][2] = getUp().z;
		return audio;
	}
}