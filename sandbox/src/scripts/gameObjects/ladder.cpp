/*! \file doorLock.cpp
*
* \brief A script class for the door locks
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/ladder.h"
#include "scripts/gameObjects/player.h"
#include "settings/settings.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/utils/mathUtils.h"
#include "independent/systems/systems/audioManager.h"
#include "independent/systems/systems/timerSystem.h"
#include "scripts/menus/levelCompleteScript.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

//! Ladder()
Ladder::Ladder()
{
	setScriptName("Ladder");
	m_ascendRate = 2.f;
	m_goalHeight = 10.f;
	m_anglePrecision = 5.f;
	m_minDistance = 15.f;
	m_ascendTextEntity = nullptr;

	m_playerTransform = nullptr;
	m_ladderTransform = nullptr;
	m_allowAscend = false; 
	m_ascendAnim = false; 
	m_showText = false;
	m_sound = nullptr;
}

//! Ladder()
/*
\param scriptData a nlohmann::json - The script JSON data
*/
Ladder::Ladder(nlohmann::json scriptData)
{
	setScriptName("Ladder");
	m_goalHeight = scriptData["goalHeight"];
	m_ascendRate = scriptData["ascendRate"];
	m_anglePrecision = scriptData["anglePrecision"];
	m_minDistance = scriptData["minDistance"];

	m_ascendTextEntity = nullptr;
	m_playerTransform = nullptr;
	m_ladderTransform = nullptr;
	m_allowAscend = false;
	m_ascendAnim = false;
	m_showText = false;
}

//! ~DoorLock()
Ladder::~Ladder()
{
}

//! onAttach()
void Ladder::onAttach()
{
	TimerSystem::startTimer("StateText");
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Ladder::onPostUpdate(const float timestep, const float totalTime)
{
	if (m_showText)
	{
		if (m_ascendTextEntity)
		{
			char ch = (char)Settings::getKeys(PlayerConfig::UnlockDoor).first;
			std::string str1 = "Press ";
			std::string str2 = " to ascend.";
			m_ascendTextEntity->getComponent<Text>()->setText(str1 + ch + str2);
			m_ascendTextEntity->setDisplay(true);
		}
	}
}

void Ladder::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_playerTransform)
	{
		auto playerCam = SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("PlayerCam");
		if (playerCam) m_playerTransform = playerCam->getComponent<Transform>();
	}

	if (!m_ladderTransform)
	{
		if (m_parentEntity) m_ladderTransform = m_parentEntity->getComponent<Transform>();
	}

	if (!m_ascendTextEntity)
	{
		m_ascendTextEntity = getParent()->getParentScene()->getEntity("Canvas")->getChildEntity("AscendText");
	}

	float angle = m_playerTransform->angle(getParent()->getComponent<Transform>());
	float dist = getParent()->getComponent<Transform>()->distance(m_playerTransform);

	// Check if we the door should open
	if (angle < m_anglePrecision && dist <= m_minDistance)
	{
		m_allowAscend = true;
		m_showText = true;
	}
	else
	{
		m_allowAscend = false;
		m_showText = false;
		m_ascendTextEntity->setDisplay(false);
	}

	if (m_ascendAnim)
	{
		glm::vec3 pos = m_playerTransform->getParent()->getParentEntity()->getComponent<Transform>()->getLocalPosition();
		m_playerTransform->getParent()->getParentEntity()->getComponent<Transform>()->setPosition(pos + glm::vec3(0.f, m_ascendRate * timestep, 0.f));
		if (pos.y >= m_goalHeight)
		{
			//!< Exit the scene!
			m_ascendAnim = false;
			
		}

	}
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Ladder::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::UnlockDoor).first && m_allowAscend)
	{
		Player* player = static_cast<Player*>(m_playerTransform->getParent()->getParentEntity()->getComponent<NativeScript>());
		Item* selectedItem = player->getItem(player->getHotbar()->getSelectedItem());
		if (selectedItem)
		{
			if (selectedItem->getType() == Items::Hand && !m_ascendAnim)
			{
				m_ascendAnim = true;
				m_playerTransform->getParent()->getParentEntity()->getComponent<RigidBody>()->setType(BodyTypes::RigidBodyType::STATIC);
				static_cast<LevelCompleteScript*>(getParent()->getParentScene()->getEntity("LevelCompleteEnt")->getComponent<NativeScript>())->play();

				//getParent()->getComponent<StudioSound>()->playSound();
			}
			else if (selectedItem->getType() == Items::Hand && m_ascendAnim)
			{
				reset();
			}
		}
	}
}

void Ladder::reset()
{
	m_ascendAnim = false;
	if(m_playerTransform) m_playerTransform->getParent()->getParentEntity()->getComponent<RigidBody>()->setType(BodyTypes::RigidBodyType::PLAYER);
}

//! getAsJSON()
/*!
\return a nlohmann::ordered_json - The component's data in JSON format
*/
nlohmann::ordered_json Ladder::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["anglePrecision"] = m_anglePrecision;
	data["minDistance"] = m_minDistance;
	data["goalHeight"] = m_goalHeight;
	data["ascendRate"] = m_ascendRate;
	return data;
}

#ifndef NG_DEPLOY
//! displayImGui
void Ladder::displayImGui()
{
	NativeScript::displayImGui();

	ImGui::DragFloat("angle Precision###angle1", &m_anglePrecision, 1.f, 0.f, 90.f, "%.2f");
	ImGui::DragFloat("min Distance###dist1", &m_minDistance, 0.1f, 0.f, 150.f, "%.2f");

	ImGui::DragFloat("ascend rate###asc1", &m_ascendRate, 0.01f);
	ImGui::DragFloat("goal height###goal1", &m_goalHeight, 0.05f);
	ImGui::Checkbox("Ascending", &m_ascendAnim);
	ImGui::Checkbox("Can Ascend", &m_allowAscend);
	ImGui::Checkbox("Showing Text", &m_showText);
	if (ImGui::Button("Reset")) reset();

	//m_ascendTextEntity = nullptr;
	//m_playerTransform = nullptr;
	//m_ladderTransform = nullptr;
	//m_sound = nullptr;
}
#endif // NG_DEPLOY