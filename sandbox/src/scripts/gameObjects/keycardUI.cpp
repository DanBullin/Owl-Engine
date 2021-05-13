/*! \file keycardUI.cpp
*
* \brief A UI script for the keycards
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/keycardUI.h"
#include "independent/entities/entity.h"

KeycardUI::KeycardUI()
{
	setScriptName("KeycardUI");
	m_tint = { 0.f, 0.f, 0.f, 1.f };
}

KeycardUI::~KeycardUI()
{
}

void KeycardUI::onSubmit(const Renderers renderer, const std::string & renderState)
{
	if (renderer == Renderers::Renderer2D)
	{
		getParent()->getComponent<MeshRender2D>()->getMaterial()->setTint(m_tint);
	}
}

void KeycardUI::setTint(const glm::vec4 & tint)
{
	m_tint = tint;
}
