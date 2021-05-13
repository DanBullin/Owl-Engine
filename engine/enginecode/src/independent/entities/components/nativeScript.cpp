/*! \file nativeScript.cpp
*
* \brief A native script (C++) component
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/entities/components/nativeScript.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/scriptManager.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! NativeScript()
	NativeScript::NativeScript() : EntityComponent(ComponentType::NativeScript)
	{
		setScriptName("NativeScript");
	}

	//! ~NativeScript()
	NativeScript::~NativeScript()
	{
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json NativeScript::getAsJSON()
	{
		nlohmann::ordered_json data;
		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["scriptName"] = m_scriptName;
		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void NativeScript::displayImGui()
	{
		ImGui::BeginGroup();
		if (ImGui::BeginCombo(std::string("script###script01" + m_name).c_str(), m_scriptName.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			std::vector<std::string> scriptNames = ScriptManager::getScriptNames();

			for (uint32_t i = 0; i < scriptNames.size(); i++)
			{
				if (ImGui::Selectable(scriptNames.at(i).c_str()))
				{
					NativeScript* ptr = this;
					getParent()->detachDelayDeletion(this);

					NativeScript* newScript = ScriptManager::createNewScript(scriptNames.at(i));
					newScript->setName(getName());
					getParent()->attach<NativeScript>(newScript);
					delete ptr;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::EndGroup();
	}
#endif // NG_DEPLOY

	//! getScriptName()
	/*!
	\return a std::string - The name of the script
	*/
	std::string NativeScript::getScriptName()
	{
		return m_scriptName;
	}

	//! setScriptName()
	/*!
	\param name a std::string - The name of the script
	*/
	void NativeScript::setScriptName(const std::string& name)
	{
		m_scriptName = name;
	}
}