/*! \file monster.cpp
*
* \brief The monster class
*
* \author: Jamie Hall
*
*/

#include "sandbox_pch.h"
#include "scripts/gameObjects/monster.h"
#include "independent/entities/entity.h"
#include "independent/systems/components/scene.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif
//! Monster()
Monster::Monster()
{
	setScriptName("Monster");
	m_patrolType = PatrolType::PatrolNavigateType::TRACK;
	m_killingEnabled = true;
	m_huntingEnabled = true;
	m_detectDist = 5.0f;
	m_velocity = 1.0f;
}

Monster::Monster(nlohmann::json scriptData)
{
	setScriptName("Monster");
	m_patrolType = scriptData["patrolType"];
	m_killingEnabled = scriptData["isKillingEnabled"];
	m_huntingEnabled = scriptData["isHuntingEnabled"];
	m_detectDist = scriptData["detectDist"];
	m_velocity = scriptData["velocity"];
}

//! ~Monster()
Monster::~Monster()
{
}

nlohmann::ordered_json Monster::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["patrolType"] = m_patrolType;
	data["isKillingEnabled"] = m_killingEnabled;
	data["isHuntingEnabled"] = m_huntingEnabled;
	data["detectDist"] = m_detectDist;
	data["velocity"] = m_velocity;
	return data;
}

//! onAttach
void Monster::onAttach()
{
	// Set defaults
	m_iterator = 0;
	updateMovementNodeVector();
}

//! onPreUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/

void Monster::onPreUpdate(float timestep, float totalTime)
{
	// Update the global entity and movement node lists when global list changes
	if (m_globalEntities.size() != getParent()->getParentScene()->getEntities().size())
	{
		updateMovementNodeVector();
	}

	// Update player entity
	if (!m_playerEnt)
	{
		m_playerEnt = getParent()->getParentScene()->getEntity("Player1");
	}
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Monster::onPostUpdate(float timestep, float totalTime)
{
	// Get the distance between the player and the monster
	float playerXDist;
	float playerZDist;
	if (m_playerEnt)
	{
		glm::vec3 playerPos = m_playerEnt->getComponent<Transform>()->getPosition();
		glm::vec3 monsterPos = getParent()->getComponent<Transform>()->getPosition();
		playerXDist = monsterPos.x - playerPos.x;
		playerZDist = monsterPos.z - playerPos.z;
	}

	// Post update, under player pos check
	if (!(getParent()->getParentScene()->getPhysicsPaused()))
	{
		// Hunt player if theyre within range
		if (playerXDist < m_detectDist && playerXDist > -m_detectDist && playerZDist < m_detectDist && playerZDist > -m_detectDist && m_huntingEnabled)
			moveToNode(m_playerEnt, timestep);
		// Otherwise navigate using nodes
		else if (!m_movementNodes.empty())
		{
			// If the target is reached, iterate through the node list
			if (m_reachedTarget)
			{
				if (m_patrolType == PatrolType::PatrolNavigateType::CIRCUIT)
				{
					if (m_movementNodes[m_iterator] == m_movementNodes.back())
						m_iterator = 0;
					else
						m_iterator += 1;
				}
				else if (m_patrolType == PatrolType::PatrolNavigateType::TRACK)
				{
					if (m_movementNodes[m_iterator] == m_movementNodes.back())
						m_invertTraversal = true;
					else if (m_movementNodes[m_iterator] == m_movementNodes.front())
						m_invertTraversal = false;

					if (m_invertTraversal)
						m_iterator -= 1;
					else
						m_iterator += 1;
				}
				m_reachedTarget = false;
			}
			moveToNode(m_movementNodes[m_iterator], timestep);
		}
	}
}

//! onTrigger()
/*!
\param collisionEntity a Entity* - Pointer to other entity in collision
*/
void Monster::onTrigger(Entity * overlapEntity)
{
	if (overlapEntity->getName() == "Player1" && m_killingEnabled)
	{
		// Get scenes game over object
		m_gameOverEnt = getParent()->getParentScene()->getEntity("GameOverEnt");
		if (m_gameOverEnt)
		{
			// Get script object from entity, call game over function
			m_gameOverScript = static_cast<GameOverScript*>(m_gameOverEnt->getComponent<NativeScript>());
			m_gameOverScript->activateGameOver();
		}
	}
}


#ifndef NG_DEPLOY
//! displayImGui()
void Monster::displayImGui()
{
	if (ImGui::Button("Update Nodes")) { updateMovementNodeVector(); }
	ImGui::Checkbox("Enable Killing###killEnable", &m_killingEnabled);
	ImGui::Checkbox("Enable Hunting###huntEnable", &m_huntingEnabled);

	ImGui::DragFloat("Detect Distance", &m_detectDist, 0.1f);
	ImGui::DragFloat("Chasing Speed", &m_velocity, 0.1f);

	std::string text = "";
	switch (m_patrolType)
	{
	case PatrolType::PatrolNavigateType::TRACK:
		text += "Track";
		break;

	case PatrolType::PatrolNavigateType::CIRCUIT:
		text += "Circuit";
		break;
	default:
		text += "Error getting patrol type";
		break;
	}
	if (ImGui::BeginCombo(std::string("patroltype###ptype" + m_name).c_str(), text.c_str(), ImGuiComboFlags_PopupAlignLeft))
	{
		if (ImGui::Selectable("Track"))
		{
			m_patrolType = PatrolType::PatrolNavigateType::TRACK;
		}
		if (ImGui::Selectable("Circuit"))
		{
			m_patrolType = PatrolType::PatrolNavigateType::CIRCUIT;
		}

		ImGui::EndCombo();
	}
}
#endif

//! moveToNode()
/*!
\param targetNode a Entity* - Pointer to targeted entity
*/
void Monster::moveToNode(Entity* targetNode, float timestep)
{
	glm::vec3 velocityX = { m_velocity, 0.0, 0.0 };
	glm::vec3 velocityZ = { 0.0, 0.0, m_velocity };
	glm::vec3 moveVec = { 0.0, 0.0, 0.0 };

	Transform* monsterTrans = getParent()->getComponent<Transform>();
	glm::vec3 targetPos = targetNode->getComponent<Transform>()->getPosition();

	// Compare monster and target positions
	float xDiff = monsterTrans->getPosition().x - targetPos.x;
	float zDiff = monsterTrans->getPosition().z - targetPos.z;

	// Move in X direction
	if (xDiff > 0.1 || xDiff < -0.1)
	{
		if (monsterTrans->getPosition().x < targetPos.x)
			moveVec += velocityX;
		else if (monsterTrans->getPosition().x > targetPos.x)
			moveVec -= velocityX;
	}

	// Move in Z direction
	if (zDiff > 0.1 || zDiff < -0.1)
	{
		if (monsterTrans->getPosition().z < targetPos.z)
			moveVec += velocityZ;
		else if (monsterTrans->getPosition().z > targetPos.z)
			moveVec -= velocityZ;
	}

	monsterTrans->setPosition(monsterTrans->getPosition() + (moveVec * timestep));

	// Update diff values
	xDiff = monsterTrans->getPosition().x - targetPos.x;
	zDiff = monsterTrans->getPosition().z - targetPos.z;

	// If positions are about the same, set reached target to true
	if (xDiff < 0.1 && xDiff > -0.1 && zDiff < 0.1 && zDiff > -0.1)
	{
		m_reachedTarget = true;
		ENGINE_INFO("[Monster::moveToNode] Target node reached");
	}
	else
		m_reachedTarget = false;
}

void Monster::updateMovementNodeVector()
{
	m_globalEntities = getParent()->getParentScene()->getEntities();
	m_movementNodes.clear();
	for (const auto& node : m_globalEntities)
	{
		if (node->containsComponent<AIMovementNode>())
			if (node->getComponent<AIMovementNode>()->getAttachedEntityName() == getParent()->getName())
				m_movementNodes.push_back(node);
	}
}
