/*! \file rigidBody.h
*
* \brief A RigiBody component that allows physics to affect the attached entity. Uses ReactPhysics3D.
*
* \author Jamie Hall & Aaron Taleghani
*
*/
#include "engine_pch.h"
#include "independent/entities/components/rigidBody.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systemManager.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! RigidBody()
	/*!
	\param world a rp3d::PhysicsWorld* - The phyiscs world
	\param type a BodyTypes::RigidBodyType - The type of RigidBody
	\param canSleep a bool - Can the rigidbody enter sleep mode
	*/
	RigidBody::RigidBody(rp3d::PhysicsWorld* world, BodyTypes::RigidBodyType type, bool canSleep) : EntityComponent(ComponentType::RigidBody)
	{
		m_bodyType = type;
		m_canSleep = canSleep;
		m_system = static_cast<PhysicsSystem*>(SystemManager::getSystemByType(SystemType::Physics));
		m_body = nullptr;

		if (world)
			m_world = world;
		else
			ENGINE_ERROR("[RigidBody::RigidBody] An invalid React3D physics world was provided.");
	}

	//! onAttach()
	void RigidBody::onAttach()
	{
		if (m_parentEntity)
		{
			if (m_bodyType == BodyTypes::RigidBodyType::PLAYER)
			{
				Camera* camera = getParent()->getComponent<Camera>();
				if (camera == nullptr) ENGINE_WARN("[RigidBody::onAttach] No camera present, cannot attach!");
			}

			Transform* trans = getParent()->getComponent<Transform>();
			if (trans == nullptr) ENGINE_WARN("[RigidBody::onAttach] No transform present, cannot attach!");
			else if (m_world == nullptr) ENGINE_WARN("[RigidBody::onAttach] No world present, cannot attach!");
			
			else
			{
				glm::quat quat = trans->getOrientationAsQuat();
				rp3d::Quaternion ori = rp3d::Quaternion(quat.x, quat.y, quat.z, quat.w);
				rp3d::Vector3 initialPos(trans->getPosition().x, trans->getPosition().y, trans->getPosition().z);
				rp3d::Transform transform(initialPos, ori);

				if (m_world)
				{
					m_body = m_world->createRigidBody(transform);

					switch (m_bodyType)
					{
						case BodyTypes::RigidBodyType::STATIC:
						{
							m_body->setType(rp3d::BodyType::STATIC);
							break;
						}
						case BodyTypes::RigidBodyType::KINEMATIC:
						{
							m_body->setType(rp3d::BodyType::KINEMATIC);
							break;

						}
						case BodyTypes::RigidBodyType::DYNAMIC:
						{
							m_body->setType(rp3d::BodyType::DYNAMIC);
							break;
						}
						case BodyTypes::RigidBodyType::PLAYER:
						{
							m_body->setType(rp3d::BodyType::DYNAMIC);
							m_body->setIsAllowedToSleep(false);
							m_body->setAngularDamping(0.0);
							m_body->setLinearDamping(0.9);
							m_body->setMass(100.0);
							break;
						}
					}
					void* userData = reinterpret_cast<void*>(getParent()->getUniqueID());
					if(userData) m_body->setUserData(userData);
					
					if (m_parentEntity->getComponent<NativeScript>())
						m_body->setIsAllowedToSleep(false);

					m_notAttached = false;
					return;
				}
				else
					ENGINE_ERROR("[RigidBody::RigidBody] An invalid React3D physics world was provided.");
			}
			m_notAttached = true;
		}
		else
			ENGINE_ERROR("[RigidBody::onAttach] An invalid parent entity is attached to this component. Component Name: {0}.", m_name);
	}

	//! onDetach()
	void RigidBody::onDetach()
	{
		if (m_body)
		{
			m_world->destroyRigidBody(m_body);
		}
	}

	//! onUpdate()
	void RigidBody::onUpdate(const float timestep, const float totalTime)
	{
		if (m_notAttached)
		{
			onAttach();
		}
		else
		{
			if (m_body)
			{
				glm::vec3 position = PhysicsSystem::reactToGLM(m_body->getTransform().getPosition());
				glm::quat orientation = glm::quat(m_body->getTransform().getOrientation().w, m_body->getTransform().getOrientation().x, m_body->getTransform().getOrientation().y, m_body->getTransform().getOrientation().z);

				if (m_parentEntity)
				{
					Transform* trans = m_parentEntity->getComponent<Transform>();
					if (trans)
					{
						if (trans->getChanged() || m_bodyType == BodyTypes::RigidBodyType::STATIC)
						{
							rp3d::Vector3 pos = rp3d::Vector3(trans->getPosition().x, trans->getPosition().y, trans->getPosition().z);
							glm::quat quat = trans->getOrientationAsQuat();
							rp3d::Quaternion ori = rp3d::Quaternion(quat.x, quat.y, quat.z, quat.w);
							m_body->setTransform(rp3d::Transform(pos, ori));
							trans->setChanged(false);
						}
						else
						{
							if (getParent()->getParentEntity())
							{
								trans->setPosition(PhysicsSystem::reactToGLM(m_body->getTransform().getPosition()) - getParent()->getParentEntity()->getComponent<Transform>()->getPosition());
							}
							else
								trans->setPosition(position);
							trans->setOrientation(orientation);
						}
					}
					else
						ENGINE_ERROR("[RigidBody::onUpdate] The entity does not have a valid transform. Entity Name: {0}.", m_parentEntity->getName());
				}
				else
					ENGINE_ERROR("[RigidBody::onUpdate] An invalid parent entity is attached to this component. Component Name: {0}.", m_name);
			}
			else
				ENGINE_ERROR("[BoxCollider::onUpdate] This box collider does not have a valid React3D physics collider. Component Name: {0}.", m_name);
		}
	}

	//! printComponentDetails()
	void RigidBody::printComponentDetails()
	{
		rp3d::Vector3 position = m_body->getTransform().getPosition();
		glm::quat oriQuat = { 1.0f, m_body->getTransform().getOrientation().x, m_body->getTransform().getOrientation().y, m_body->getTransform().getOrientation().z };
		glm::vec3 orientation = glm::eulerAngles(oriQuat);
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("RigidBody Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Position: {0}, {1}, {2}", position.x, position.y, position.z);
		ENGINE_TRACE("Orientation: {0}, {1}, {2}", orientation.x, orientation.y, orientation.z);
		ENGINE_TRACE("==========================================");
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json RigidBody::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["rigidBodyType"] = m_bodyType;

		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void RigidBody::displayImGui()
	{
		if (m_body == nullptr)
		{
			ImGui::Text("Rigidbody not initialised yet!");
			return;
		}
		bool isSleep = m_body->isSleeping();
		bool isActive = m_body->isActive();
		bool isGravity = m_body->isGravityEnabled();
		bool canSleep = m_body->isAllowedToSleep();
		float mass = m_body->getMass();

		glm::vec3 pos = { m_body->getTransform().getPosition().x, m_body->getTransform().getPosition().y, m_body->getTransform().getPosition().z };
		glm::quat oriQuat = { 1.0f, m_body->getTransform().getOrientation().x, m_body->getTransform().getOrientation().y, m_body->getTransform().getOrientation().z };
		glm::vec3 ori = glm::eulerAngles(oriQuat);
		glm::vec3 line = { m_body->getLinearVelocity().x, m_body->getLinearVelocity().y, m_body->getLinearVelocity().z};
		glm::vec3 inert = { m_body->getLocalInertiaTensor().x, m_body->getLocalInertiaTensor().y, m_body->getLocalInertiaTensor().z };

		if (ImGui::DragFloat3("position###pos1", &pos.x, 0.1f, 0.f, 0.f, "%.2f")) setChanged(true);
		if (ImGui::DragFloat3("linear vel###line1", &line.x, 0.1f, 0.f, 0.f, "%.2f")) m_body->setLinearVelocity(rp3d::Vector3(line.x, line.y, line.z));
		if (ImGui::DragFloat3("rotation###ori1", &ori.x, 0.01f, 0.f, 0.f, "%.2f")) setChanged(true);
		if (ImGui::DragFloat3("inertia###inert1", &inert.x, 0.1f, 0.f, 0.f, "%.2f")) m_body->setLocalInertiaTensor(rp3d::Vector3(inert.x, inert.y, inert.z));

		

		ImGui::Text("Mass: %f", mass);
		ImGui::Checkbox("sleeping###slep1", &isSleep);
		ImGui::Checkbox("active###act1", &isActive);
		ImGui::Checkbox("gravity###grav1", &isGravity);
		ImGui::Checkbox("can sleep###can1", &canSleep);
		ImGui::Checkbox("changed###chang1", &m_changed);

		std::string text = "";
		switch (m_bodyType)
		{
		case BodyTypes::RigidBodyType::STATIC:
			text += "Static";
			break;

		case BodyTypes::RigidBodyType::KINEMATIC:
			text += "Kinematic";
			break;

		case BodyTypes::RigidBodyType::DYNAMIC:
			text += "Dynamic";
			break;

		case BodyTypes::RigidBodyType::PLAYER:
			text += "Player (Dynamic)";
			break;

		default:
			text += "Not impl. in inspector";
			break;
		}
		if (ImGui::BeginCombo(std::string("type###rbtype" + m_name).c_str(), text.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			if (ImGui::Selectable("Static"))
			{
				m_bodyType = BodyTypes::RigidBodyType::STATIC;
				m_body->setType(rp3d::BodyType::STATIC);
			}
			if (ImGui::Selectable("Kinematic"))
			{
				m_bodyType = BodyTypes::RigidBodyType::KINEMATIC;
				m_body->setType(rp3d::BodyType::KINEMATIC);
			}
			if (ImGui::Selectable("Dynamic"))
			{
				m_bodyType = BodyTypes::RigidBodyType::DYNAMIC;
				m_body->setType(rp3d::BodyType::DYNAMIC);
			}
			if (ImGui::Selectable("Player"))
			{
				m_bodyType = BodyTypes::RigidBodyType::PLAYER;
				m_body->setType(rp3d::BodyType::DYNAMIC);
			}

			ImGui::EndCombo();
		}	
	}
#endif // NG_DEPLOY

	//! setVelocity()
	/*!
	\param front a const glm::vec3 - The front 
	*/
	void RigidBody::setVelocity(glm::vec3 front)
	{
		rp3d::Vector3 vel(front.x, 0.f, front.z);
		m_body->setLinearVelocity(m_body->getLinearVelocity() + vel);
	}

	void RigidBody::setVelocityUp(const glm::vec3 up)
	{
		rp3d::Vector3 vel(up.x, up.y, up.z);
		m_body->setLinearVelocity(m_body->getLinearVelocity() + vel);
	}

	void RigidBody::setType(BodyTypes::RigidBodyType type)
	{
		m_bodyType = type;
		switch (type)
		{
		case BodyTypes::RigidBodyType::STATIC:
			m_body->setType(rp3d::BodyType::STATIC);
			break;

		case BodyTypes::RigidBodyType::DYNAMIC:
			m_body->setType(rp3d::BodyType::DYNAMIC);
			break;

		case BodyTypes::RigidBodyType::KINEMATIC:
			m_body->setType(rp3d::BodyType::KINEMATIC);
			break;

		case BodyTypes::RigidBodyType::PLAYER:
			m_body->setType(rp3d::BodyType::DYNAMIC);
			break;
		}
	}

	void RigidBody::applyForce(float velocity)
	{
		rp3d::Vector3 currentVel = m_body->getLinearVelocity();
		float velChange = velocity - currentVel.length();
		float forceScalar = m_body->getMass() * velChange;

		rp3d::Vector3 direction;

		rp3d::Transform transform = m_body->getTransform();
		rp3d::Quaternion q = transform.getOrientation();

		direction.x = -2.f * (q.x * q.z - q.w * q.y);
		direction.y = 2.f * (q.y * q.z + q.w * q.x);
		direction.z = 1.f - 2.f*(q.x * q.x + q.y * q.y);

		rp3d::Vector3 force(direction.x * forceScalar, direction.y * forceScalar, direction.z * forceScalar);

		m_body->applyForceToCenterOfMass(force);
	}

	//! getBody()
	/*!
	\return a rp3d::RigidBody* - The react physics RigidBody
	*/
	rp3d::RigidBody * RigidBody::getBody()
	{
		return m_body;
	}

	//! getType()
	/*!
	\return a RigidBodyType - The type of RigidBody
	*/
	BodyTypes::RigidBodyType RigidBody::getType()
	{
		return m_bodyType;
	}
}