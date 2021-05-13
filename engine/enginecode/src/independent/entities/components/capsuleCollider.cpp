/*! \file capsuleCollider.cpp
*
* \brief A capsule collider entity component that attaches to a rigidbody. Uses ReactPhysics3D.
*
* \author Jamie Hall
*
*/
#include "engine_pch.h"
#include "independent/entities/components/sphereCollider.h"
#include "independent/systems/systems/log.h"
#include "independent/entities/entity.h"
#include "independent/systems/systemManager.h"
#include "independent\entities\components\capsuleCollider.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! CapsuleCollider()
	/*!
	\param radius a const float - The radius of the colliders capsule shape hemisphere
	\param height a const float - The distance between the centre of each of the capsules hemispheres
	\param position a glm::vec3 - The local position of the collider (Relative to the rigidbody)
	*/
	CapsuleCollider::CapsuleCollider(const float radius, const float height, glm::vec3 position) : EntityComponent(ComponentType::CapsuleCollider)
	{
		m_position = position;
		m_radius = radius;
		m_height = height;
		m_system = static_cast<PhysicsSystem*>(SystemManager::getSystemByType(SystemType::Physics));
	}

	//! onAttach()
	void CapsuleCollider::onAttach()
	{
		if (m_parentEntity)
		{
			Transform* trans = m_parentEntity->getComponent<Transform>();
			RigidBody* rigid = m_parentEntity->getComponent<RigidBody>();

			if (m_system == nullptr) ENGINE_WARN("[CapsuleCollider::onAttach] No physics system present, cannot attach!");
			else if (trans == nullptr) ENGINE_WARN("[CapsuleCollider::onAttach] No transform present, cannot attach!");
			else if (rigid == nullptr) ENGINE_WARN("[CapsuleCollider::onAttach] No rigidbody present, cannot attach!");
			else if (rigid->getBody() == nullptr) ENGINE_WARN("[CapsuleCollider::onAttach] Rigidbody not initialised, cannot attach!");
			else
			{
				auto& physicsCommon = m_system->getPhysicsCommon();

				m_shape = physicsCommon.createCapsuleShape(m_radius, m_height);
				rp3d::Vector3 RPposition(m_position.x, m_position.y, m_position.z);
				rp3d::Quaternion orientation = rigid->getBody()->getTransform().getOrientation();
				rp3d::Transform transform(RPposition, orientation);

				if (m_shape)
					m_collider = rigid->getBody()->addCollider(m_shape, transform);
				else
					ENGINE_ERROR("[CapsuleCollider::onAttach] Cannot add shape to collider as the shape is an invalid shape. Component Name: {0}.", m_name);

				m_notAttached = false;
				return;
			}
			m_notAttached = true;
		}
		else
			ENGINE_ERROR("[CapsuleCollider::onAttach] An invalid parent entity is attached to this component. Component Name: {0}.", m_name);
	}

	//! onDetach()
	void CapsuleCollider::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void CapsuleCollider::onUpdate(const float timestep, const float totalTime)
	{
		if (m_notAttached)
		{
			onAttach();
		}
		else
		{
			if (m_collider)
			{
				rp3d::Vector3 newPosition = m_collider->getLocalToBodyTransform().getPosition();
				m_position = { newPosition.x, newPosition.y, newPosition.z };
			}
			else
				ENGINE_ERROR("[CapsuleCollider::onUpdate] This capsule collider does not have a valid React3D physics collider. Component Name: {0}.", m_name);
			m_collider->setIsTrigger(m_isTrigger);
		}
	}

	//! printComponentDetails()
	void CapsuleCollider::printComponentDetails()
	{
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json CapsuleCollider::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["radius"] = m_radius;
		data["height"] = m_height;
		data["localPosition"] = { m_position.x, m_position.y, m_position.z };

		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void CapsuleCollider::displayImGui()
	{
		if (ImGui::DragFloat3("position###capsulePos", &m_position.x, 0.1f, 0.f, 0.f, "%.2f")) setPosition(m_position);
		if (ImGui::DragFloat("height###capsuleHeight", &m_height, 0.05f, 0.f, 0.f, "%.2f")) setHeight(m_height);
		if (ImGui::DragFloat("radius###capsuleRadius", &m_radius, 0.05f, 0.f, 0.f, "%.2f")) setRadius(m_radius);
		ImGui::Checkbox("set trigger###trigger", &m_isTrigger);
	}
#endif // NG_DEPLOY

	//! getPosition()
	/*!
	\return a vec3 - Colliders current position
	*/
	glm::vec3 CapsuleCollider::getPosition()
	{
		return m_position;
	}

	//! setPosition()
	/*!
	\param position a glm::vec3 - The local position of the collider
	*/
	void CapsuleCollider::setPosition(glm::vec3 position)
	{
		if (m_collider)
		{
			m_position = position;

			rp3d::Vector3 RPposition(position.x, position.y, position.z);
			rp3d::Quaternion orientation = rp3d::Quaternion::identity();
			rp3d::Transform transform(RPposition, orientation);

			m_collider->setLocalToBodyTransform(transform);
		}
		else
			ENGINE_ERROR("[CapsuleCollider::setPosition] This box collider does not have a valid React3D physics collider. Component Name: {0}.", m_name);
	}

	//! getRadius()
	/*!
	\return a float - The radius of the colliders shape
	*/
	float CapsuleCollider::getRadius()
	{
		return m_radius;
	}

	//! setRadius()
	/*!
	\param radius a const float - The radius of the colliders shape
	*/
	void CapsuleCollider::setRadius(const float radius)
	{
		if (m_shape)
		{
			m_radius = radius;
			m_shape->setRadius(m_radius);
		}
		else
			ENGINE_ERROR("[CapsuleCollider::setHalfExtents] This box collider does not have a valid React3D shape. Component Name: {0}.", m_name);
	}

	//! getHeight()
	/*!
	\return a float - The height of the colliders shape
	*/
	float CapsuleCollider::getHeight()
	{
		return m_height;
	}

	//! setRadius()
	/*!
	\param height a const float - The height of the colliders shape
	*/
	void CapsuleCollider::setHeight(const float height)
	{
		if (m_shape)
		{
			m_height = height;
			m_shape->setHeight(m_height);
		}
		else
			ENGINE_ERROR("[CapsuleCollider::setHalfExtents] This box collider does not have a valid React3D shape. Component Name: {0}.", m_name);
	}

	//! getIsTrigger()
	/*!
	\return a bool - Is the collider a trigger
	*/
	bool CapsuleCollider::getIsTrigger()
	{
		return m_isTrigger;
	}

	//! setIsTrigger()
	/*!
	\param isTrigger - Is the collider a trigger
	*/
	void CapsuleCollider::setIsTrigger(bool isTrigger)
	{
		m_isTrigger = isTrigger;
	}

	//! onRender()
	void CapsuleCollider::onRender()
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, getParent()->getComponent<Transform>()->getPosition());
		model = glm::translate(model, getPosition());
		model = glm::scale(model, glm::vec3(m_radius, m_height, m_radius));

		Renderer3D::submit("SphereCollider", ResourceManager::getResource<Model3D>("sphere")->getMeshes().at(0).getGeometry(),
			ResourceManager::getResource<Material>("colliderMaterial"), model);
	}
}