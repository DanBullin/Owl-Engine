/*! \file viewableMap.cpp
*
* \brief A script to display a 3D map
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/viewableMap.h"
#include "independent/entities/entity.h"
#include "independent/systems/components/scene.h"
#include "independent/rendering/renderers/renderer3D.h"
#include "independent/utils/mathUtils.h"

ViewableMap::ViewableMap(nlohmann::json scriptData)
{
	setScriptName("ViewableMap");
	m_player = nullptr;
}

ViewableMap::~ViewableMap()
{
}

void ViewableMap::onPreUpdate(const float timestep, const float totalTime)
{
	if(!m_player) m_player = static_cast<Player*>(getParent()->getParentScene()->getEntity("Player1")->getComponent<NativeScript>());

	if (m_player)
	{
		if (m_player->getItem(m_player->getHotbar()->getSelectedItem()))
		{
			if (m_player->getItem(m_player->getHotbar()->getSelectedItem())->getType() == Items::Map)
				getParent()->setDisplay(true);
			else
				getParent()->setDisplay(false);
		}
		else
			getParent()->setDisplay(false);
	}
}

void ViewableMap::onAttach()
{
}