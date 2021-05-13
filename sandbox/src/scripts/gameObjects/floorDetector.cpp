/*! \file floorDetector.cpp
*
* \brief A floor detector script to check if the player can jump
*
* \author: Jamie Hall
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/floorDetector.h"
#include "settings/settings.h"
#include "independent/entities/entity.h"
#include "scripts/gameObjects/player.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/utils/mathUtils.h"

//! FloorDetector()
FloorDetector::FloorDetector()
{
	setScriptName("FloorDetector");
	m_text1 = new Text("You hit a trigger!", { 1.0, 1.0, 1.0, 0.0 }, "Ubuntu40B");
	m_show = false;
}

//! ~FloorDetector()
FloorDetector::~FloorDetector()
{
	if (m_text1) delete m_text1; m_text1 = nullptr;
}

void FloorDetector::onPreUpdate(const float timestep, const float totalTime)
{
	m_show = false;
}

void FloorDetector::onRender(const Renderers renderer, const std::string & renderState)
{
	if (renderer == Renderers::Renderer2D && m_show)
	{
		glm::vec4 tint2 = m_text1->getColour();
		m_text1->setColour({ tint2.r, tint2.g, tint2.b, 1.f });

		auto viewPort = getParent()->getParentScene()->getViewPort();
		Renderer2D::submitText(m_text1, MathUtils::getModelMatrix({ (viewPort.x / 2) - 200, (viewPort.y / 2), 0.5f }));
	}
}

//! onTrigger()
void FloorDetector::onTrigger(Entity* overlapEntity)
{
	if (overlapEntity != m_parentEntity->getParentEntity())
	{
		m_show = true;
	}
}