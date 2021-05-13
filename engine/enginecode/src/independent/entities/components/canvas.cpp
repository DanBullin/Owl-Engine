/*! \file canvas.cpp
*
* \brief Updates UI children
*
* \author Joshua Simons
*
*/
#include "engine_pch.h"
#include "independent/entities/components/canvas.h"
#include "entities/entity.h"
#include "independent/systems/components/scene.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/entities/components/element.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	namespace UI
	{
		//! Canvas()
		Canvas::Canvas() : EntityComponent(ComponentType::Canvas)
		{
			m_initialized = false;
			m_watching = false;
			m_screenSize = glm::vec2(0);
		}

		//! Canvas
		/*!
		\param size a glm::vec2 - The size of the canvas
		\param watching a bool - Is it watching the scene viewport
		\param init a bool - Has it been initialised
		*/
		Canvas::Canvas(glm::vec2 size, bool watching, bool init) : EntityComponent(ComponentType::Canvas)
		{
			m_initialized = init;
			m_watching = watching;
			m_screenSize = size;
		}

		//! ~Canvas()
		Canvas::~Canvas()
		{
		}

		//! onAttach()
		void Canvas::onAttach()
		{

		}

		//! onDetach()
		void Canvas::onDetach()
		{

		}

		//! onUpdate()
		/*!
		\param timestep a const float - The time step
		\param totalTime a const float - The total time of the application
		*/
		void Canvas::onUpdate(const float timestep, const float totalTime)
		{
			if (!m_initialized)
			{
				if (m_parentEntity)
				{
					if (m_parentEntity->getParentScene())
					{
						if (m_parentEntity->getParentScene()->getInspectorEnabled()) m_screenSize = m_parentEntity->getParentScene()->getViewPort();
						else m_screenSize = WindowManager::getFocusedWindow()->getProperties().getSizef();

						m_initialized = true;
						return;
					}
					else ENGINE_WARN("Trying to init canvas on entity with no scene!");
					return;
				}
				else ENGINE_WARN("Trying to init canvas with no parent!");
				return;
			}

			if (!m_watching) return;

			glm::vec2 current = m_screenSize;
			glm::vec2 difference = glm::vec2(0);
			if (m_parentEntity->getParentScene()->getInspectorEnabled()) current = m_parentEntity->getParentScene()->getViewPort();
			else current = WindowManager::getFocusedWindow()->getProperties().getSizef();

			if (current != m_screenSize) difference = current - m_screenSize;
			else return;

			ENGINE_WARN("Resize detected!");
			for (auto child : m_parentEntity->getChildEntities())
			{
				if (child.second->getComponent<Element>() != nullptr)
				{
					child.second->getComponent<Element>()->onResize(difference);
				}
			}

			m_screenSize = current;
		}

		//! setScreenSize()
		/*!
		\param size a glm::vec2 - The screen size
		*/
		void Canvas::setScreenSize(glm::vec2 size)
		{
			m_screenSize = size;
		}

		//! getScreenSize()
		/*
		\return a glm::vec2 - The screen size
		*/
		glm::vec2 Canvas::getScreenSize()
		{
			return m_screenSize;
		}

		//! printComponentDetails()
		void Canvas::printComponentDetails()
		{
		}

		//! getAsJSON()
		/*!
		\return a nlohmann::ordered_json - The component's data in JSON format
		*/
		nlohmann::ordered_json Canvas::getAsJSON()
		{
			nlohmann::ordered_json data;
			data["name"] = m_name;
			data["type"] = Components::toString(m_componentType);
			data["size"] = {m_screenSize.x, m_screenSize.y};
			data["watch"] = m_watching;
			data["init"] = m_initialized;
			return data;
		}

#ifndef NG_DEPLOY
		//! displayImGui()
		void Canvas::displayImGui()
		{
			ImGui::Checkbox("watching", &m_watching);
			ImGui::DragFloat2("size", &m_screenSize.x);
		}
#endif // NG_DEPLOY
	}
}