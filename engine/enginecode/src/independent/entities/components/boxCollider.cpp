/*! \file boxCollider.cpp
*
* \brief A box collider entity component that attaches to a rigidbody. Uses ReactPhysics3D.
*
* \author Jamie Hall & Aaron Taleghani
*
*/
#include "engine_pch.h"
#include "independent/entities/components/boxCollider.h"
#include "independent/systems/systems/log.h"
#include "independent/entities/entity.h"
#include "independent/systems/systemManager.h"
#include "independent/rendering/renderers/renderer3D.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! BoxCollider()
	/*!
	\param halfExtents a glm::vec3& - The half extents of the colliders box shape
	\param position a glm::vec3 - The local position of the collider (Relative to the rigidbody)
	*/
	BoxCollider::BoxCollider(glm::vec3& halfExtents, glm::vec3 position) : EntityComponent(ComponentType::BoxCollider)
	{
		m_position = position;
		m_halfExtents = halfExtents;
		m_system = static_cast<PhysicsSystem*>(SystemManager::getSystemByType(SystemType::Physics));
	}

	//! onAttach()
	void BoxCollider::onAttach()
	{
		if (m_parentEntity)
		{
			Transform* trans = m_parentEntity->getComponent<Transform>();
			RigidBody* rigid = m_parentEntity->getComponent<RigidBody>();

			if (m_system == nullptr) ENGINE_WARN("[BoxCollider::onAttach] No physics system present, cannot attach!");
			else if (trans == nullptr) ENGINE_WARN("[BoxCollider::onAttach] No transform present, cannot attach!");
			else if (rigid == nullptr) ENGINE_WARN("[BoxCollider::onAttach] No rigidbody present, cannot attach!");
			else if (rigid->getBody() == nullptr) ENGINE_WARN("[BoxCollider::onAttach] Rigidbody not initialised, cannot attach!");
			else
			{
				auto& physicsCommon = m_system->getPhysicsCommon();

				m_shape = physicsCommon.createBoxShape(rp3d::Vector3(m_halfExtents.x, m_halfExtents.y, m_halfExtents.z));
				rp3d::Vector3 RPposition(m_position.x, m_position.y, m_position.z);
				rp3d::Quaternion orientation = rigid->getBody()->getTransform().getOrientation();
				rp3d::Transform transform(RPposition, orientation);

				if (m_shape)
				{
					m_collider = rigid->getBody()->addCollider(m_shape, transform);
					m_collider->getMaterial().setFrictionCoefficient(1250.0);
					m_collider->getMaterial().setBounciness(0.0);
					m_collider->getMaterial().setMassDensity(100.0);
				}
				else
					ENGINE_ERROR("[BoxCollider::onAttach] Cannot add shape to collider as the shape is an invalid shape. Component Name: {0}.", m_name);

				m_notAttached = false;
				return;
			}
			m_notAttached = true;
		}
		else
			ENGINE_ERROR("[BoxCollider::onAttach] An invalid parent entity is attached to this component. Component Name: {0}.", m_name);
	}

	//! onDetach()
	void BoxCollider::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void BoxCollider::onUpdate(const float timestep, const float totalTime)
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
				m_collider->setIsTrigger(m_isTrigger);
			}
			else
				ENGINE_ERROR("[BoxCollider::onUpdate] This box collider does not have a valid React3D physics collider. Component Name: {0}.", m_name);
		}
	}

	//! printComponentDetails()
	void BoxCollider::printComponentDetails()
	{
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Box Collider Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Local Position: {0}, {1}, {2}", m_position.x, m_position.y, m_position.z);
		ENGINE_TRACE("Size (Half extents): {0}, {1}, {2}", m_halfExtents.x, m_halfExtents.y, m_halfExtents.z);
		ENGINE_TRACE("==========================================");
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json BoxCollider::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["halfExtents"] = { m_halfExtents.x, m_halfExtents.y, m_halfExtents.z };
		data["localPosition"] = { m_position.x, m_position.y, m_position.z };
		data["isTrigger"] = m_isTrigger;
		
		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void BoxCollider::displayImGui()
	{
		if (ImGui::DragFloat3("position###boxPos", &m_position.x, 0.1f, 0.f, 0.f, "%.2f")) setPosition(m_position);
		if (ImGui::DragFloat3("half-extents###halfExtents", &m_halfExtents.x, 0.05f, 0.01f, 9999.f, "%.2f")) setHalfExtents(m_halfExtents);
		ImGui::Checkbox("set trigger###trigger", &m_isTrigger);
	}
#endif // NG_DEPLOY

	//! getPosition()
	/*!
	\return a glm::vec3 - Colliders current position
	*/
	glm::vec3 BoxCollider::getPosition()
	{
		return m_position;
	}

	//! setPosition()
	/*!
	\param position a glm::vec3 - The local position of the collider
	*/
	void BoxCollider::setPosition(glm::vec3 position)
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
			ENGINE_ERROR("[BoxCollider::setPosition] This box collider does not have a valid React3D physics collider. Component Name: {0}.", m_name);
	}

	//! getHalfExtents
	/*!
	\return a glm::vec3 - Collider shapes current half extents
	*/
	glm::vec3 BoxCollider::getHalfExtents()
	{
		return m_halfExtents;
	}

	//! setHalfextents()
	/*!
	\param halfExtents a glm::vec3& - The half extents of the colliders shape
	*/
	void BoxCollider::setHalfExtents(glm::vec3& halfExtents)
	{
		if (m_shape)
		{
			m_halfExtents = halfExtents;
			m_shape->setHalfExtents(rp3d::Vector3(halfExtents.x, halfExtents.y, halfExtents.z));
		}
		else
			ENGINE_ERROR("[BoxCollider::setHalfExtents] This box collider does not have a valid React3D shape. Component Name: {0}.", m_name);
	}

	//! getIsTrigger
	/*!
	\return a bool - True if collider is a trigger
	*/
	bool BoxCollider::getIsTrigger()
	{
		return m_isTrigger;
	}

	//! setIsTrigger
	/*!
	\param a bool - True if collider is a trigger
	*/
	void BoxCollider::setIsTrigger(bool isTrigger)
	{
		m_isTrigger = isTrigger;
	}

	//! onRender()
	void BoxCollider::onRender()
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, getParent()->getComponent<Transform>()->getPosition());
		model = glm::translate(model, getPosition());
		model = glm::scale(model, getHalfExtents() * glm::vec3(2.f, 2.f, 2.f));

		Renderer3D::submit("BoxCollider", ResourceManager::getResource<Model3D>("cube")->getMeshes().at(0).getGeometry(),
			ResourceManager::getResource<Material>("colliderMaterial"), model);
	}

	//! containsPoint()
	/*!
	\param point a const glm::vec3& - The point
	\return a bool - Does the point lie within the collider
	*/
	bool BoxCollider::containsPoint(const glm::vec3 & point)
	{
		auto transform = getParent()->getComponent<Transform>();

		if (transform)
		{
			glm::vec3 centre = transform->getPosition() + m_position;



			glm::vec2 pos = { transform->getPosition().x, transform->getPosition().y };
			glm::vec2 topLeft = pos - glm::vec2(transform->getScale().x / 2.f, transform->getScale().y / 2.f);
			glm::vec2 bottomRight = pos + glm::vec2(transform->getScale().x / 2.f, transform->getScale().y / 2.f);

			if (point.x >= centre.x - m_halfExtents.x && point.x <= centre.x + m_halfExtents.x)
			{
				if (point.y >= centre.y - m_halfExtents.y && point.y <= centre.y + m_halfExtents.y)
				{
					if (point.z >= centre.z - m_halfExtents.z && point.z <= centre.z + m_halfExtents.z)
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	//! vectorIntersects()
	/*!
	\param pos a const glm::vec3& - The point
	\param vec a const glm::Vec3& vec
	\param vecMultiplier a const float - The vector multiplier
	\return a bool - Does the vector intersecr with the collider
	*/
	bool BoxCollider::vectorIntersects(const glm::vec3 & pos, const glm::vec3 & vec, const float vecMultiplier)
	{
		glm::vec3 vecFinal = vec * vecMultiplier;
		glm::vec3 endPos = pos + vecFinal;

		for (float f = 0.0; f < 1.1; f += 0.1)
		{
			if (containsPoint(pos + (f * (endPos - pos))))
			{
				return true;
			}
		}
		return false;
	}
}