/*! \file sphereCollider.cpp
*
* \brief A sphere collider entity component that attaches to a rigidbody. Uses ReactPhysics3D.
*
* \author Jamie Hall
*
*/
#include "engine_pch.h"
#include "independent/entities/components/sphereCollider.h"
#include "independent/systems/systems/log.h"
#include "independent/entities/entity.h"
#include "independent/systems/systemManager.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! SphereCollider()
	/*!
	\param radius a const float - The radius of the colliders sphere shape
	\param position a glm::vec3 - The local position of the collider (Relative to the rigidbody)
	*/
	SphereCollider::SphereCollider(const float radius, glm::vec3 position) : EntityComponent(ComponentType::SphereCollider)
	{
		m_position = position;
		m_radius = radius;
		m_system = static_cast<PhysicsSystem*>(SystemManager::getSystemByType(SystemType::Physics));
	}

	//! onAttach()
	void SphereCollider::onAttach()
	{
		if (m_parentEntity)
		{
			Transform* trans = m_parentEntity->getComponent<Transform>();
			RigidBody* rigid = m_parentEntity->getComponent<RigidBody>();

			if (m_system == nullptr) ENGINE_WARN("[SphereCollider::onAttach] No physics system present, cannot attach!");
			else if (trans == nullptr) ENGINE_WARN("[SphereCollider::onAttach] No transform present, cannot attach!");
			else if (rigid == nullptr) ENGINE_WARN("[SphereCollider::onAttach] No rigidbody present, cannot attach!");
			else if (rigid->getBody() == nullptr) ENGINE_WARN("[SphereCollider::onAttach] Rigidbody not initialised, cannot attach!");
			else
			{
				auto& physicsCommon = m_system->getPhysicsCommon();

				m_shape = physicsCommon.createSphereShape(m_radius);
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
					ENGINE_ERROR("[SphereCollider::onAttach] Cannot add shape to collider as the shape is an invalid shape. Component Name: {0}.", m_name);

				m_notAttached = false;
				return;
			}
			m_notAttached = true;
		}
		else
			ENGINE_ERROR("[SphereCollider::onAttach] An invalid parent entity is attached to this component. Component Name: {0}.", m_name);
	}

	//! onDetach()
	void SphereCollider::onDetach()
	{

	}

	//! onUpdate()
	/*!
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void SphereCollider::onUpdate(const float timestep, const float totalTime)
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
				ENGINE_ERROR("[SphereCollider::onUpdate] This sphere collider does not have a valid React3D physics collider. Component Name: {0}.", m_name);
			m_collider->setIsTrigger(m_isTrigger);
		}
	}

	//! printComponentDetails()
	void SphereCollider::printComponentDetails()
	{
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Sphere Collider Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Local Position: {0}, {1}, {2}", m_position.x, m_position.y, m_position.z);
		ENGINE_TRACE("Size (Radius): {0}", m_radius);
		ENGINE_TRACE("==========================================");
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json SphereCollider::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["radius"] = m_radius;
		data["localPosition"] = { m_position.x, m_position.y, m_position.z };
		data["isTrigger"] = m_isTrigger;

		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void SphereCollider::displayImGui()
	{
		if (ImGui::DragFloat3("position###spherePos", &m_position.x, 0.1f, 0.f, 0.f, "%.2f")) setPosition(m_position);
		if (ImGui::DragFloat("radius###sphereRadius", &m_radius, 0.05f, 0.f, 0.f, "%.2f")) setRadius(m_radius);
		ImGui::Checkbox("set trigger###trigger", &m_isTrigger);
	}
#endif // NG_DEPLOY

	//! getPosition()
	/*!
	\return a vec3 - Colliders current position
	*/
	glm::vec3 SphereCollider::getPosition()
	{
		return m_position;
	}

	//! setPosition()
	/*!
	\param position a glm::vec3 - The local position of the collider
	*/
	void SphereCollider::setPosition(glm::vec3 position)
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
			ENGINE_ERROR("[SphereCollider::setPosition] This box collider does not have a valid React3D physics collider. Component Name: {0}.", m_name);
	}

	//! getRadius()
	/*!
	\return a float - The radius of the colliders shape
	*/
	float SphereCollider::getRadius()
	{
		return m_radius;
	}

	//! setRadius()
	/*!
	\param radius a const float - The radius of the colliders shape
	*/
	void SphereCollider::setRadius(const float radius)
	{
		if (m_shape)
		{
			m_radius = radius;
			m_shape->setRadius(m_radius);
		}
		else
			ENGINE_ERROR("[SphereCollider::setHalfExtents] This box collider does not have a valid React3D shape. Component Name: {0}.", m_name);
	}

	//! getIsTrigger()
	/*!
	\return a bool - Is the collider a trigger
	*/
	bool SphereCollider::getIsTrigger()
	{
		return m_isTrigger;
	}

	//! setIsTrigger()
	/*!
	\param isTrigger - Is the collider a trigger
	*/
	void SphereCollider::setIsTrigger(bool isTrigger)
	{
		m_isTrigger = isTrigger;
	}

	//! onRender()
	void SphereCollider::onRender()
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, getParent()->getComponent<Transform>()->getPosition());
		model = glm::translate(model, getPosition());
		model = glm::scale(model, glm::vec3(m_radius, m_radius, m_radius));

		Renderer3D::submit("SphereCollider", ResourceManager::getResource<Model3D>("sphere")->getMeshes().at(0).getGeometry(),
			ResourceManager::getResource<Material>("colliderMaterial"), model);
	}
}