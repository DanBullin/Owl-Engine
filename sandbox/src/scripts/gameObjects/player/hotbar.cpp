/*! \file hotbar.h
*
* \brief The player's hotbar
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/player/hotbar.h"
#include "scripts/gameObjects/keycardUI.h"
#include "scripts/gameObjects/doorLock.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/SceneManager.h"
#include "settings/settings.h"
#include "scripts/gameObjects/player.h"

Hotbar::Hotbar()
{
	m_currentItem = 0;
	m_oldItem = 0;
	m_currentElement = nullptr;
	m_updated = true;
	m_player = nullptr;
}

Hotbar::~Hotbar()
{
}

void Hotbar::onPreUpdate(const float timestep, const float totalTime)
{
	if (!m_currentElement)
	{
		m_currentElement = SceneManager::getActiveScene()->getEntity("Canvas")->getChildEntity("Hotbar_Current")->getComponent<UI::Element>();
	}
	else
	{
		if (m_updated)
		{
			m_currentElement->setOffset({ -206.f + (103.f * m_currentItem), m_currentElement->getOffset().y });
			m_currentElement->onResize(SceneManager::getActiveScene()->getViewPort(), true);
			m_updated = false;
		}
	}

	if (!m_player)
	{
		m_player = static_cast<Player*>(SceneManager::getActiveScene()->getEntity("Player1")->getComponent<NativeScript>());
	}

	if (m_hotbarElements.size() == 0)
	{
		auto ent = SceneManager::getActiveScene()->getEntity("Canvas")->getChildEntity("Hotbar");
		m_hotbarElements.push_back({ ent->getChildEntity("Item0Text")->getComponent<Text>(), ent->getChildEntity("Item0Image")->getComponent<MeshRender2D>() });
		m_hotbarElements.push_back({ ent->getChildEntity("Item1Text")->getComponent<Text>(), ent->getChildEntity("Item1Image")->getComponent<MeshRender2D>() });
		m_hotbarElements.push_back({ ent->getChildEntity("Item2Text")->getComponent<Text>(), ent->getChildEntity("Item2Image")->getComponent<MeshRender2D>() });
		m_hotbarElements.push_back({ ent->getChildEntity("Item3Text")->getComponent<Text>(), ent->getChildEntity("Item3Image")->getComponent<MeshRender2D>() });
		m_hotbarElements.push_back({ ent->getChildEntity("Item4Text")->getComponent<Text>(), ent->getChildEntity("Item4Image")->getComponent<MeshRender2D>() });
	}
	else
	{
		// Update the hotbar items based on player's inventory
		if (m_player)
		{
			for (int i = 0; i < s_maxCount; i++)
			{
				if (m_player->getItem(i))
				{
					m_hotbarElements.at(i).first->getParent()->setDisplay(true);
					m_hotbarElements.at(i).second->getParent()->setDisplay(true);
					if (m_hotbarElements.at(i).second->getParent()->containsComponent<NativeScript>())
					{
						static_cast<KeycardUI*>(m_hotbarElements.at(i).second->getParent()->getComponent<NativeScript>())->setTint({ getColour(m_player->getItem(i)->getState()), 1.f});
					}
				}
				else
				{
					m_hotbarElements.at(i).first->getParent()->setDisplay(false);
					m_hotbarElements.at(i).second->getParent()->setDisplay(false);
				}
			}
		}
	}
}

void Hotbar::onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar1).first)
		selectHotbar(0);
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar2).first)
		selectHotbar(1);
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar3).first)
		selectHotbar(2);
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar4).first)
		selectHotbar(3);
	else if (e.getKeyCode() == Settings::getKeys(PlayerConfig::Hotbar5).first)
		selectHotbar(4);
}

void Hotbar::onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime)
{
	if (e.getYOffset() == 1)
		scrollLeft();
	else
		scrollRight();

	m_updated = true;
}


void Hotbar::scrollRight()
{
	if (m_currentItem < s_maxCount - 1)
		m_currentItem++;
}

void Hotbar::scrollLeft()
{
	if (m_currentItem > 0)
		m_currentItem--;
}

void Hotbar::selectHotbar(const uint32_t index)
{
	if (index >= s_maxCount) return;
	m_currentItem = index;
	m_updated = true;
}

uint32_t Hotbar::getSelectedItem()
{
	return m_currentItem;
}

uint32_t Hotbar::getMaxCount()
{
	return s_maxCount;
}

bool Hotbar::itemChanged()
{
	bool changed = false;
	if (m_oldItem == m_currentItem) 
		changed = false;
	else
		changed = true;

	m_oldItem = m_currentItem;
	return changed;
}
