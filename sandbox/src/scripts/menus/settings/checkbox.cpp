/*! \file checkbox.cpp
*
* \brief A script for a checkbox to change settings
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/settings/checkbox.h"
#include "independent/entities/entity.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

Checkbox::Checkbox()
{
	m_function = PlayerConfig::toType("None");
	setScriptName("Checkbox");
	m_render = nullptr;
}

Checkbox::Checkbox(nlohmann::json scriptData)
{
	setScriptName("Checkbox");
	m_function = PlayerConfig::toType(scriptData["playerConfig"].get<std::string>());
	m_render = nullptr;
}

Checkbox::~Checkbox()
{
}

void Checkbox::onAttach()
{
	if (!m_render) m_render = getParent()->getComponent<MeshRender2D>();

	if (Settings::getBool(m_function))
		m_render->setMaterial(ResourceManager::getResource<Material>("checkboxCMaterial"));
	else
		m_render->setMaterial(ResourceManager::getResource<Material>("checkboxUMaterial"));
}

void Checkbox::onPostUpdate(const float timestep, const float totalTime)
{
}

void Checkbox::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	bool containsMouse = m_render->containsPoint(mousePosition);

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		Settings::setBool(m_function, !Settings::getBool(m_function));

		if (Settings::getBool(m_function))
			m_render->setMaterial(ResourceManager::getResource<Material>("checkboxCMaterial"));
		else
			m_render->setMaterial(ResourceManager::getResource<Material>("checkboxUMaterial"));
	}
}

nlohmann::ordered_json Checkbox::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["playerConfig"] = toString(m_function);
	return data;
}

#ifndef NG_DEPLOY
void Checkbox::displayImGui()
{
	NativeScript::displayImGui();

	ImGui::BeginGroup();
	if (ImGui::BeginCombo(std::string("config###playerConfig2").c_str(), toString(m_function).c_str(), ImGuiComboFlags_PopupAlignLeft))
	{
		for (uint32_t i = 0; i < PlayerConfig::configCount; i++)
		{
			if (ImGui::Selectable(toString(PlayerConfig::ConfigData(i)).c_str()))
			{
				m_function = PlayerConfig::ConfigData(i);
			}
		}
		ImGui::EndCombo();
	}
	ImGui::EndGroup();
}
#endif