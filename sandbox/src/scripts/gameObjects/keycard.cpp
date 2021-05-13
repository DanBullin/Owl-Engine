/*! \file keycard.cpp
*
* \brief A script class for the keycards
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/keycard.h"
#include "scripts/gameObjects/staticLight.h"
#include "scripts/gameObjects/player.h"
#include "settings/settings.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/utils/mathUtils.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

//! Keycard()
Keycard::Keycard()
{
	setScriptName("Keycard");
	m_lockColour = DoorLockColour::GREEN;
	m_pickUpText = nullptr;

	m_playerTransform = nullptr;
	m_showText = false;
}

//! Keycard()
/*
\param scriptData a nlohmann::json - The script JSON data
*/
Keycard::Keycard(nlohmann::json scriptData)
{
	setScriptName("Keycard");
	m_lockColour = scriptData["colour"];

	m_pickUpText = nullptr;
	m_playerTransform = nullptr;
	m_showText = false;
}

//! ~Keycard()
Keycard::~Keycard()
{
}

//! onAttach()
void Keycard::onAttach()
{
	// Add light entity
	Entity* lightEntity = new Entity;
	getParent()->addChildEntity("Light", lightEntity);
	lightEntity->setLayer(getParent()->getLayer());
	lightEntity->setDisplay(true);
	lightEntity->setSaved(false);
	lightEntity->attach<Transform>("Transform", 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.4f, 0.4f, 0.4f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f);

	auto col = getColour(m_lockColour);
	lightEntity->attach<PointLight>("PointLight", glm::vec3(0.f, 0.f, 0.f), glm::vec3(col.r, col.g, col.b), glm::vec3(0.f, 0.f, 0.f), 1.f, 5.f, 5.f);
	lightEntity->attach<MeshRender3D>("MeshRender", ResourceManager::getResource<Model3D>("keycardModel"), ResourceManager::getResource<Material>("lightSourceMaterial"));

	NativeScript* nativeScript = new StaticLight;
	nativeScript->setName("Script1");
	lightEntity->attach<NativeScript>(nativeScript);
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Keycard::onPostUpdate(const float timestep, const float totalTime)
{
	if (m_showText && !getParent()->getDestroyed())
	{
		if (m_pickUpText)
		{
			char ch = (char)Settings::getKeys(PlayerConfig::Pickup).first;
			std::string str1 = "Press ";
			std::string str2 = " to pickup the keycard.";
			m_pickUpText->getComponent<Text>()->setText(str1 + ch + str2);
			m_pickUpText->setDisplay(true);
		}
	}
}

//! onPreUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Keycard::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_playerTransform)
	{
		auto playerCam = SceneManager::getActiveScene()->getEntity("Player1")->getChildEntity("PlayerCam");
		if (playerCam)
			m_playerTransform = playerCam->getComponent<Transform>();
	}

	if (!m_pickUpText)
	{
		m_pickUpText = getParent()->getParentScene()->getEntity("Canvas")->getChildEntity("PickupText");
	}

	// Check if we the door should open
	if (getParent()->getComponent<BoxCollider>()->vectorIntersects(getParent()->getParentScene()->getMainCamera()->getWorldPosition(), getParent()->getParentScene()->getMainCamera()->getCameraData().Front, 5.f))
	{
		m_showText = true;
	}
	else
	{
		m_showText = false;
		m_pickUpText->setDisplay(false);
	}
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Keycard::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Pickup).first && m_showText)
	{
		Player* player = static_cast<Player*>(m_playerTransform->getParent()->getParentEntity()->getComponent<NativeScript>());
		if (!player->getItem(Items::Keycard, m_lockColour))
		{
			static_cast<Player*>(m_playerTransform->getParent()->getParentEntity()->getComponent<NativeScript>())->setItem(Items::Keycard, m_lockColour);
			getParent()->destroy();
			m_pickUpText->setDisplay(false);
			getParent()->getChildEntity("Pickup")->getComponent<StudioSound>()->playSound();
		}
	}
}

//! getAsJSON()
/*!
\return a nlohmann::ordered_json - The component's data in JSON format
*/
nlohmann::ordered_json Keycard::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["colour"] = m_lockColour;
	return data;
}

#ifndef NG_DEPLOY
//! displayImGui()
void Keycard::displayImGui()
{
	NativeScript::displayImGui();

	ImGui::BeginGroup();
	if (ImGui::BeginCombo(std::string("lockColour###lockColour01" + getString(m_lockColour)).c_str(), getString(m_lockColour).c_str(), ImGuiComboFlags_PopupAlignLeft))
	{
		if (ImGui::Selectable("RED")) m_lockColour = DoorLockColour::RED;
		if (ImGui::Selectable("BLUE")) m_lockColour = DoorLockColour::BLUE;
		if (ImGui::Selectable("GREEN")) m_lockColour = DoorLockColour::GREEN;
		if (ImGui::Selectable("YELLOW")) m_lockColour = DoorLockColour::YELLOW;
		getParent()->getComponent<PointLight>()->setDiffuseFactor(getColour(m_lockColour));
		ImGui::EndCombo();
	}
	ImGui::EndGroup();
}
#endif // NG_DEPLOY