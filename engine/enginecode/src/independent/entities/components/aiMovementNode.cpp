/*! \file AIMovementNode.cpp
*
* \brief A node used for moving entity navigation
*
* \author Jamie Hall
*
*/
#include "engine_pch.h"
#include "independent/entities/components/AIMovementNode.h"
#include "independent/entities/entity.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! AIMovementNode()
	AIMovementNode::AIMovementNode(const std::string& attachedEntName) : EntityComponent(ComponentType::AIMovementNode)
	{
		setAttachedEntity(attachedEntName);
	}

	//! ~AIMovementNode
	AIMovementNode::~AIMovementNode()
	{
	}

	//! onAttach
	void AIMovementNode::onAttach()
	{
	}

	//! onDetach
	void AIMovementNode::onDetach()
	{
	}

	//! onUpdate
	/*!
	\param timestep a const float - The timestep
	\param totalTime a const float - The total time of the application
	*/
	void AIMovementNode::onUpdate(const float timestep, const float totalTime)
	{
	}

	void AIMovementNode::printComponentDetails()
	{
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("AI Movement Node Details for Entity: {0}", getParent()->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Attatched entity: ", m_attachedEntityName);
		ENGINE_TRACE("==========================================");
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json AIMovementNode::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["attachedEntity"] = getAttachedEntityName();
		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void AIMovementNode::displayImGui()
	{
		ImGui::Text("AIMovementNode");

		ImGui::NewLine();
		char buf[256];
		strcpy(buf, m_attachedEntityName.c_str());
		ImGui::InputText(std::string("Attached Entity###03" + m_name).c_str(), buf, IM_ARRAYSIZE(buf));
		m_attachedEntityName = std::string(buf);
		// Demonstrate keeping auto focus on the input box
		if (ImGui::IsItemHovered() || (ImGui::IsItemFocused() && ImGui::IsItemActive() && ImGui::IsMouseClicked(0)))
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
	}
#endif //NG_DEPLOY

	//! getAttachedEntityName()
	/*!
	\return std::string - The name of the attached entity
	*/
	const std::string& AIMovementNode::getAttachedEntityName()
	{
		return m_attachedEntityName;
	}

	//! setAttachedEntityName()
	/*!
	\param a const std::string& - The name of the new entity to attach to
	*/
	void AIMovementNode::setAttachedEntity(const std::string& newEntityName)
	{
		m_attachedEntityName = newEntityName;
	}
}