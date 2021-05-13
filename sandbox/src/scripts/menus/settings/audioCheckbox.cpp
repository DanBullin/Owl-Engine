/*! \file audioCheckbox.cpp
*
* \brief A script for a checkbox to change audio settings
*
* \author: Aaron Taleghani
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/settings/audioCheckbox.h"
#include "independent/entities/entity.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

AudioCheckbox::AudioCheckbox()
{
	m_function = PlayerConfig::toType("None");
	setScriptName("AudioCheckbox");
	m_render = nullptr;
}

AudioCheckbox::AudioCheckbox(nlohmann::json scriptData)
{
	setScriptName("AudioCheckbox");
	m_function = PlayerConfig::toType(scriptData["playerConfig"].get<std::string>());
	m_render = nullptr;
}

AudioCheckbox::~AudioCheckbox()
{
}

void AudioCheckbox::onAttach()
{
	if (!m_render) m_render = getParent()->getComponent<MeshRender2D>();

	if (!m_text) m_text = getParent()->getParentEntity()->getComponent<Text>();

	std::string volStr = std::to_string(volume);
	volStr = volStr.substr(0, volStr.find('.') + 3);

	if (Settings::getFloat(m_function))
	{
		m_render->setMaterial(ResourceManager::getResource<Material>("checkboxUMaterial"));
		m_text->setText(std::string("Change Volume:							" + volStr));
	}
	else
		m_render->setMaterial(ResourceManager::getResource<Material>("checkboxUMaterial"));
}

void AudioCheckbox::onPostUpdate(const float timestep, const float totalTime)
{
}

void AudioCheckbox::onMouseRelease(MouseReleasedEvent & e, const float timestep, const float totalTime)
{
	glm::vec2 mousePosition = InputPoller::getMousePosition();
	bool containsMouse = m_render->containsPoint(mousePosition);

	if (e.getButton() == Mouse::LEFTBUTTON && containsMouse)
	{
		volumeIt++;

		if (volumeIt < 4)
		{
			volume = audioLevels[volumeIt];
		}
		else
		{
			volumeIt = 0;
		}

		Settings::setFloat(m_function, volume);
		
		std::string volStr = std::to_string(volume);
		volStr = volStr.substr(0, volStr.find('.') + 3);


		if (Settings::getFloat(m_function))
		{
			m_render->setMaterial(ResourceManager::getResource<Material>("checkboxUMaterial"));
			m_text->setText(std::string("Change Volume:							" + volStr));
		}
		else
			m_render->setMaterial(ResourceManager::getResource<Material>("checkboxUMaterial"));
	}
}

nlohmann::ordered_json AudioCheckbox::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["playerConfig"] = toString(m_function);
	return data;
}

#ifndef NG_DEPLOY
void AudioCheckbox::displayImGui()
{
	NativeScript::displayImGui();
	ImGui::Text(std::string(std::to_string(volume)).c_str());

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