/*! \file doorLock.cpp
*
* \brief A script class for the door locks
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/doorLock.h"
#include "scripts/gameObjects/staticLight.h"
#include "scripts/gameObjects/player.h"
#include "settings/settings.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/utils/mathUtils.h"
#include "independent/systems/systems/audioManager.h"
#include "independent/systems/systems/timerSystem.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

//! DoorLock()
DoorLock::DoorLock()
{
	setScriptName("DoorLock");
	m_lockColour = DoorLockColour::GREEN;
	m_turnInterval = 20.f;
	m_goalRotation = 90.f;
	m_originalRotation = 0.f;
	m_anglePrecision = 5.f;
	m_minDistance = 15.f;
	m_lockTextEntity = nullptr;

	m_playerTransform = nullptr;
	m_successText = nullptr;
	m_failText = nullptr;
	m_doorTransform = nullptr;
	m_allowDoorOpen = false;
	m_doorOpenAni = false;
	m_doorName = "";
	m_stateTextTimer = false;
	m_unlocked = false;
	m_showText = false;
	m_sound = nullptr;
}

//! DoorLock()
/*
\param scriptData a nlohmann::json - The script JSON data
*/
DoorLock::DoorLock(nlohmann::json scriptData)
{
	setScriptName("DoorLock");
	m_lockColour = scriptData["colour"];
	m_turnInterval = scriptData["turnInterval"];
	m_goalRotation = scriptData["goalRotation"];
	m_originalRotation = scriptData["startRotation"];
	m_anglePrecision = scriptData["anglePrecision"];
	m_minDistance = scriptData["minDistance"];
	m_doorName = scriptData["doorName"];

	m_lockTextEntity = nullptr;
	m_playerTransform = nullptr;
	m_doorTransform = nullptr;
	m_successText = nullptr;
	m_failText = nullptr;
	m_allowDoorOpen = false;
	m_doorOpenAni = false;
	m_unlocked = false;
	m_showText = false;
	m_stateTextTimer = false;
}

//! ~DoorLock()
DoorLock::~DoorLock()
{
}

//! onAttach()
void DoorLock::onAttach()
{
	// Add light entity
	Entity* lightEntity = new Entity;
	getParent()->addChildEntity("Light", lightEntity);
	lightEntity->setLayer(getParent()->getLayer());
	lightEntity->setDisplay(true);
	lightEntity->setSaved(false);

	float yRot = getParent()->getComponent<Transform>()->getOrientation().y;

	if(yRot == 0)
		lightEntity->attach<Transform>("Transform", 0.67f, 0.25f, 0.12f, 0.f, 0.f, 0.f, 0.4f, 0.4f, 0.4f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f);
	else
		lightEntity->attach<Transform>("Transform", -0.67f, 0.25f, -0.12f, 0.f, 0.f, 0.f, 0.4f, 0.4f, 0.4f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f);

	auto col = getColour(m_lockColour);
	lightEntity->attach<PointLight>("PointLight", glm::vec3(0.f, 0.f, 0.f), glm::vec3(col.r, col.g, col.b), glm::vec3(0.f, 0.f, 0.f), 1.f, 5.f, 5.f);
	lightEntity->attach<MeshRender3D>("MeshRender", ResourceManager::getResource<Model3D>("sphere"), ResourceManager::getResource<Material>("lightSourceMaterial"));

	NativeScript* nativeScript = new StaticLight;
	nativeScript->setName("Script1");
	lightEntity->attach<NativeScript>(nativeScript);

	TimerSystem::startTimer("StateText");
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DoorLock::onPostUpdate(const float timestep, const float totalTime)
{
	if (m_showText)
	{
		if (m_lockTextEntity)
		{
			char ch = (char)Settings::getKeys(PlayerConfig::UnlockDoor).first;
			std::string str1 = "Press ";
			std::string str2 = " to unlock the door.";
			m_lockTextEntity->getComponent<Text>()->setText(str1 + ch + str2);
			m_lockTextEntity->setDisplay(true);
		}
	}
}

void DoorLock::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_playerTransform)
	{
		auto playerCam = SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("PlayerCam");
		if (playerCam)
			m_playerTransform = playerCam->getComponent<Transform>();
	}

	if (!m_doorTransform)
	{
		if (m_doorName != "")
		{
			auto door = SceneManager::getActiveScene()->getEntity(m_doorName);
			if (door)
				m_doorTransform = door->getComponent<Transform>();
		}
	}

	if (!m_lockTextEntity)
	{
		m_lockTextEntity = getParent()->getParentScene()->getEntity("Canvas")->getChildEntity("DoorLockText");
		m_successText = getParent()->getParentScene()->getEntity("Canvas")->getChildEntity("UnlockSuccessText");
		m_failText = getParent()->getParentScene()->getEntity("Canvas")->getChildEntity("UnlockFailText");
	}

	float angle = m_playerTransform->angle(getParent()->getComponent<Transform>());
	float dist = getParent()->getComponent<Transform>()->distance(m_playerTransform);

	// Check if we the door should open
	if (angle < m_anglePrecision && dist <= m_minDistance)
	{
		m_allowDoorOpen = true;
		m_showText = true;
	}
	else
	{
		m_allowDoorOpen = false;
		m_showText = false;
		m_lockTextEntity->setDisplay(false);
	}

	if (m_doorOpenAni)
	{
		m_doorTransform->setOrientation(m_doorTransform->getOrientation() + glm::vec3(0.f, m_turnInterval * timestep, 0.f));

		if (m_goalRotation < 0.f)
		{
			if (m_doorTransform->getOrientation().y <= m_goalRotation)
				m_doorOpenAni = false;
		}
		else
		{
			if (m_doorTransform->getOrientation().y >= m_goalRotation)
				m_doorOpenAni = false;
		}
	}

	if (m_stateTextTimer && TimerSystem::getTime("StateText", false, false) > 1.5f)
	{
		if (m_successText->getDisplay())
			m_successText->setDisplay(false);

		if (m_failText->getDisplay())
			m_failText->setDisplay(false);
	}
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void DoorLock::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::UnlockDoor).first && m_allowDoorOpen)
	{
		Player* player = static_cast<Player*>(m_playerTransform->getParent()->getParentEntity()->getComponent<NativeScript>());
		Item* selectedItem = player->getItem(player->getHotbar()->getSelectedItem());
		TimerSystem::startTimer("StateText");
		m_stateTextTimer = true;
		if (selectedItem)
		{
			if (selectedItem->getType() == Items::Keycard && selectedItem->getState() == m_lockColour)
			{
				// Open Door
				if (!m_doorOpenAni && !m_unlocked && m_doorTransform)
				{
					m_doorOpenAni = true;
					Entity* door = getParent()->getParentScene()->getEntity(m_doorName);
					if (door)
					{
						m_colliderHalfExtents = door->getComponent<BoxCollider>()->getHalfExtents();
						m_localPos = door->getComponent<BoxCollider>()->getPosition();
						door->detach<RigidBody>();
						door->detach<BoxCollider>();
					}
					getParent()->getChildEntity("doorOpen")->getComponent<StudioSound>()->playSound();
					m_unlocked = true;
					m_successText->setDisplay(true);
					getParent()->getChildEntity("card_accepted")->getComponent<StudioSound>()->playSound();
					player->removeItem(Items::Keycard, m_lockColour);
				}
			}
			else
			{
				m_failText->setDisplay(true);
				getParent()->getChildEntity("card_denied")->getComponent<StudioSound>()->playSound();
			}
		}
		else
		{
			m_failText->setDisplay(true);
			getParent()->getChildEntity("card_denied")->getComponent<StudioSound>()->playSound();
		}
	}
}

void DoorLock::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (renderer == Renderers::Renderer3D)
	{
		if (m_unlocked)
			ResourceManager::getResource<Model3D>("doorLock")->getMeshes().at(3).setMaterial(ResourceManager::getResource<Material>("unlockedMaterial"));
		else
			ResourceManager::getResource<Model3D>("doorLock")->getMeshes().at(3).setMaterial(ResourceManager::getResource<Material>("lockedMaterial"));
	}
}

void DoorLock::onRender(const Renderers renderer, const std::string & renderState)
{
	/*if (renderer == Renderers::Renderer3D)
	{
		if (m_unlocked)
			Renderer3D::submit("KeyCard1", ResourceManager::getResource<Model3D>("keycard")->getMeshes().at(0).getGeometry(), ResourceManager::getResource<Model3D>("keycard")->getMeshes().at(0).getMaterial(), MathUtils::getModelMatrix(m_doorTransform->getPosition()));
	}*/
}

void DoorLock::reset()
{
	if (m_doorTransform)
	{
		m_doorTransform->setOrientation(0.f, m_originalRotation, 0.f);
		m_unlocked = false;
		m_doorTransform->getParent()->attach<RigidBody>("RigidBody1", getParent()->getParentScene()->getPhysicsWorld(), BodyTypes::KINEMATIC);
		m_doorTransform->getParent()->attach<BoxCollider>("Collider1", m_colliderHalfExtents, m_localPos);
		m_doorTransform->getParent()->getComponent<BoxCollider>()->setIsTrigger(false);
		m_doorOpenAni = false;
	}
}

//! getAsJSON()
/*!
\return a nlohmann::ordered_json - The component's data in JSON format
*/
nlohmann::ordered_json DoorLock::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["colour"] = m_lockColour;
	data["turnInterval"] = m_turnInterval;
	data["goalRotation"] = m_goalRotation;
	data["startRotation"] = m_originalRotation;
	data["anglePrecision"] = m_anglePrecision;
	data["minDistance"] = m_minDistance;
	data["doorName"] = m_doorName;
	return data;
}

#ifndef NG_DEPLOY
//! displayImGui
void DoorLock::displayImGui()
{
	NativeScript::displayImGui();
	
	ImGui::BeginGroup();
	if (ImGui::BeginCombo(std::string("lockColour###lockColour01" + getString(m_lockColour)).c_str(), getString(m_lockColour).c_str(), ImGuiComboFlags_PopupAlignLeft))
	{
		if (ImGui::Selectable("RED")) m_lockColour = DoorLockColour::RED;
		if (ImGui::Selectable("BLUE")) m_lockColour = DoorLockColour::BLUE;
		if (ImGui::Selectable("GREEN")) m_lockColour = DoorLockColour::GREEN;
		if (ImGui::Selectable("YELLOW")) m_lockColour = DoorLockColour::YELLOW;
		getParent()->getChildEntity("Light")->getComponent<PointLight>()->setDiffuseFactor(getColour(m_lockColour));
		ImGui::EndCombo();
	}
	ImGui::EndGroup();

	ImGui::DragFloat("turnInterval###turn1", &m_turnInterval, 0.1f, -100.f, 100.f, "%.2f");
	ImGui::DragFloat("goalRotation###goal1", &m_goalRotation, 2.f, -180.f, 180.f, "%.2f");
	ImGui::DragFloat("startRotation###goal2", &m_originalRotation, 2.f, -180.f, 180.f, "%.2f");
	ImGui::DragFloat("anglePrecision###angle1", &m_anglePrecision, 1.f, 0.f, 90.f, "%.2f");
	ImGui::DragFloat("minDistance###dist1", &m_minDistance, 2.f, 0.f, 50.f, "%.2f");

	char buf[128];
	strcpy(buf, m_doorName.c_str());
	ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() - 65);
	if (ImGui::InputText(std::string("doorName###doorName").c_str(), buf, IM_ARRAYSIZE(buf)))
	{
		m_doorName = std::string(buf);
		m_doorTransform = nullptr;
	}
	ImGui::PopItemWidth();

}
#endif // NG_DEPLOY