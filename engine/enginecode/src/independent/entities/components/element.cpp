/*! \file element.cpp
*
* \brief A child component to a canvas
*
* \author Joshua Simons
*
*/
#include "engine_pch.h"
#include "independent/entities/components/element.h"
#include "independent/entities/entity.h"
#include "independent/entities/components/transform.h"
#include "independent/systems/components/scene.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	namespace UI
	{
		//! Element()
		Element::Element() : EntityComponent(ComponentType::Element)
		{
			m_anchor = glm::vec2(0);
			m_offset = glm::vec2(0);
			m_screenSize = glm::vec2(0);
			m_useAbsoluteSize = true;
			m_scaleSize = { 0.f, 0.f };
		}

		//! Elemment()
		/*!
		\param anchor a glm::vec2 - The element's anchor
		*/
		Element::Element(glm::vec2 anchor) : EntityComponent(ComponentType::Element)
		{
			m_anchor = anchor;
			m_offset = glm::vec2(0);
			m_screenSize = glm::vec2(0);
			m_useAbsoluteSize = true;
			m_scaleSize = { 0.f, 0.f };
		}

		//! ~Element()
		Element::~Element()
		{

		}

		//! onAttach()
		void Element::onAttach()
		{
			//onResize(getParent()->getParentScene()->getViewPort(), true);
		}

		//! onDetach()
		void Element::onDetach()
		{

		}

		//! onUpdate()
		/*!
		\param timestep a const float - The time step
		\param totalTime a const float - The total time of the application
		*/
		void Element::onUpdate(const float timestep, const float totalTime)
		{
			Transform* trans = m_parentEntity->getComponent<Transform>();
			if (trans != nullptr)
			{
				m_offset = glm::vec2(trans->getPosition().x - (m_anchor.x * m_screenSize.x), trans->getPosition().y - (m_anchor.y * m_screenSize.y));
			}
		}

		//! getAnchor()
		/*!
		\return a glm::vec2 - The element's anchor
		*/
		glm::vec2 Element::getAnchor()
		{
			return m_anchor;
		}

		//! setAnchor()
		/*!
		\param anchor a glm::vec2 - The element's anchor
		*/
		void Element::setAnchor(glm::vec2 anchor)
		{
			m_anchor = anchor;
		}

		//! onResize()
		/*!
		\param size a glm::vec2 - The size change
		\param whole a bool - Is the size a size change
		*/
		void Element::onResize(glm::vec2 size, bool whole)
		{
			Transform* trans = m_parentEntity->getComponent<Transform>();
			if (trans == nullptr)
			{
				ENGINE_ERROR("No transform attached to entity with canvas element!");
				return;
			}

			if (!whole)
			{
				//Simple difference method
				glm::vec3 pos = trans->getPosition();
				pos += glm::vec3(size.x * m_anchor.x, size.y * m_anchor.y, 0.0f);
				trans->setPosition({ pos.x, ceil(pos.y), pos.z });

				m_screenSize += size;
			}
			else
			{
				m_screenSize = size;

				//Position of the anchor
				glm::vec3 pos = glm::vec3(size.x * m_anchor.x, size.y * m_anchor.y, trans->getPosition().z);

				//Adjust by offset
				pos += glm::vec3(m_offset.x, m_offset.y, 0.0f);

				trans->setPosition(pos);
			}

			if (!m_useAbsoluteSize)
			{
				trans->setScale({ m_scaleSize.x * m_screenSize.x, m_scaleSize.y * m_screenSize.y, 1.f });
			}
		}

		//! getOffset()
		/*!
		\return a glm::vec2 - The element's offset
		*/
		glm::vec2 Element::getOffset()
		{
			return m_offset;
		}

		//! setOffset()
		/*!
		\param offset a glm::vec2 - The element's offset
		*/
		void Element::setOffset(glm::vec2 offset)
		{
			m_offset = offset;
		}

		glm::vec2 Element::getScaleSize()
		{
			return m_scaleSize;
		}

		void Element::setScaleSize(glm::vec2 size)
		{
			m_scaleSize = size;
		}

		bool Element::getAbsoluteScale()
		{
			return m_useAbsoluteSize;
		}

		void Element::setAbsoluteScale(bool use)
		{
			m_useAbsoluteSize = use;
		}

		//! getScreenSize()
		/*!
		\return a glm::vec2 - The screen size
		*/
		glm::vec2 Element::getScreenSize()
		{
			return m_screenSize;
		}

		//! setScreenSize()
		/*!
		\param screenSize a glm::vec2 - The screen size
		*/
		void Element::setScreenSize(glm::vec2 screenSize)
		{
			m_screenSize = screenSize;
		}

		//! printComponentDetails()
		void Element::printComponentDetails()
		{
		}

		//! getAsJSON()
		/*!
		\return a nlohmann::ordered_json - The component's data in JSON format
		*/
		nlohmann::ordered_json Element::getAsJSON()
		{
			nlohmann::ordered_json data;
			data["name"] = m_name;
			data["type"] = Components::toString(m_componentType);
			data["anchor"] = {m_anchor.x, m_anchor.y };
			data["offset"] = { m_offset.x, m_offset.y };
			data["screenSize"] = { m_screenSize.x, m_screenSize.y };
			data["useAbsoluteSize"] = m_useAbsoluteSize;
			data["scaleSize"] = { m_scaleSize.x, m_scaleSize.y };
			return data;
		}

#ifndef NG_DEPLOY
		//! displayImGui()
		void Element::displayImGui()
		{
			ImGui::DragFloat2("anchor", &m_anchor.x, 0.01f, 0.0f, 1.0f);
			if (ImGui::DragFloat2("offset", &m_offset.x, 0.1f)) onResize(m_screenSize, true);
			if (ImGui::DragFloat2("scale size", &m_scaleSize.x, 0.1f)) onResize(m_screenSize, true);

			ImGui::Checkbox("use Absolute size###absoluteSize01", &m_useAbsoluteSize);

			if (ImGui::Button("Reset to anchor"))
			{
				m_offset = glm::vec2(0);
				m_screenSize = m_parentEntity->getParentEntity()->getComponent<Canvas>()->getScreenSize();

				onResize(m_screenSize, true);
			}
			//ImGui::DragFloat2("screen size", &m_screenSize.x, 0.01f, 0.0f, 1.0f);
		}
#endif

	}
}