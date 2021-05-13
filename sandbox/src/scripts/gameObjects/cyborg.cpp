/*! \file cyborg.cpp
*
* \brief A Cyborg dummy class
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/cyborg.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/entities/entity.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

//! Cyborg()
Cyborg::Cyborg()
{
	m_controller = nullptr;
	m_originalScale = { 0.f, 0.f, 0.f };
	setScriptName("Cyborg");
}

//! ~Cyborg()
Cyborg::~Cyborg()
{
}

//! onKeyPress()
/*!
\param e a KeyPressedEvent& - A key press event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Cyborg::onKeyPress(KeyPressedEvent & e, const float timestep, const float totalTime)
{
	if (!m_controller) m_controller = getParent()->getComponent<CharacterController>();
	else
	{
		if (e.getKeyCode() == Keys::UP)
		{
			m_controller->move(FORWARD, timestep);
		}

		if (e.getKeyCode() == Keys::DOWN)
		{
			m_controller->move(BACKWARD, timestep);
		}

		if (e.getKeyCode() == Keys::LEFT)
		{
			m_controller->move(LEFT, timestep);
		}

		if (e.getKeyCode() == Keys::RIGHT)
		{
			m_controller->move(RIGHT, timestep);
		}

		if (e.getKeyCode() == Keys::SPACE)
		{
			m_controller->move(UP, timestep);
		}

		if (e.getKeyCode() == Keys::X)
		{
			m_controller->move(RIGHT, timestep);
		}
	}

	if (e.getKeyCode() == Keys::L)
	{
		ENGINE_INFO("Hey ho");
	}
}

//! getAsJSON()
/*!
\return a nlohmann::ordered_json - The component's data in JSON format
*/
nlohmann::ordered_json Cyborg::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["originalScale"] = {0.0f, 0.0f, 0.0f};
	return data;
}

#ifndef NG_DEPLOY
void Cyborg::displayImGui()
{
	NativeScript::displayImGui();
	if (ImGui::DragFloat3("originalScale###pos1", &m_originalScale.x, 0.1f, 0.f, 0.f, "%.2f")) setChanged(true);
}
#endif // NG_DEPLOY