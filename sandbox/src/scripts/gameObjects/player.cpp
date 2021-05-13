/*! \file player.cpp
*
* \brief A player script
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/player.h"
#include "scripts/gameObjects/player/item.h"
#include "settings/settings.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/eventManager.h"
#include "independent/file/sceneSaver.h"

//! Player()
Player::Player()
{
	m_controller = nullptr;
	m_sound = nullptr;
	setScriptName("Player");
}

//! ~Player()
Player::~Player()
{
	if (m_hotbar) delete m_hotbar;
	m_hotbar = nullptr;

	for (auto& item : m_items)
	{
		if (item)
			delete item;
	}
}

void Player::onPreUpdate(const float timestep, const float totalTime)
{
	m_hotbar->onPreUpdate(timestep, totalTime);
	auto torch = getParent()->getChildEntity("PlayerCam")->getChildEntity("Torch");

	if (m_items[m_hotbar->getSelectedItem()])
	{
		if (m_items[m_hotbar->getSelectedItem()]->getType() == Items::Torch)
		{
			if (m_hotbar->itemChanged())
			{
				torch->setDisplay(true);
				getParent()->getChildEntity("flashlight")->getComponent<StudioSound>()->playSound();
				torch->getComponent<SpotLight>()->setLinear(0.006f);
				torch->getComponent<SpotLight>()->setQuadratic(0.003f);
				torch->getComponent<SpotLight>()->setCutOff(3.55f);
				torch->getComponent<SpotLight>()->setOuterCutOff(18.75f);
			}
		}
		else if (m_items[m_hotbar->getSelectedItem()]->getType() == Items::Map)
		{
			if (m_hotbar->itemChanged())
			{
				torch->setDisplay(true);
				getParent()->getChildEntity("paper_russle")->getComponent<StudioSound>()->playSound();
				torch->getComponent<SpotLight>()->setLinear(0.25f);
				torch->getComponent<SpotLight>()->setQuadratic(0.02f);
				torch->getComponent<SpotLight>()->setCutOff(6.45f);
				torch->getComponent<SpotLight>()->setOuterCutOff(17.1f);
			}
		}
		else
		{
			if (m_hotbar->itemChanged())
			{
				torch->setDisplay(false);
			}
		}
	}
	else
	{
		if(m_hotbar->itemChanged())
		{
			torch->setDisplay(false);
			getParent()->getChildEntity("flashlight")->getComponent<StudioSound>()->playSound();
		}
	}

	if (torch)
	{
		torch->getComponent<SpotLight>()->setDirection(getParent()->getParentScene()->getMainCamera()->getCameraData().Front);
	}
}

//! onAttach()
void Player::onAttach()
{
	m_controller = getParent()->getComponent<CharacterController>();
	m_hotbar = new Hotbar;

	for (uint32_t i = 0; i < Hotbar::getMaxCount(); i++)
	{
		m_items[i] = nullptr;
	}

	setItem(Items::Hand, 0);
	setItem(Items::Map, 0);
	setItem(Items::Torch, 0);
}

//! onKeyPress()
/*!
\param e a KeyPressedEvent& - A key press event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime)
{
	m_hotbar->onKeyPress(e, timestep, totalTime);

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveForward).first)
	{
		if (m_controller) {
			m_moving = true;
			stepCounter++;
			m_controller->move(FORWARD, timestep);
		}
	}
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveLeft).first)
	{
		if (m_controller) {
			m_moving = true;
			stepCounter++;
			m_controller->move(LEFT, timestep);
		}
	}
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveBack).first)
	{
		if (m_controller) {
			m_moving = true;
			stepCounter++;
			m_controller->move(BACKWARD, timestep);
		}
	}
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveRight).first)
	{
		if (m_controller) {
			m_moving = true;
			stepCounter++;
			m_controller->move(RIGHT, timestep);
		}
	}
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveUp).first)
	{
		if(m_controller)
			if (m_grounded)
			{
				m_controller->move(UP, timestep);
				m_grounded = false;
			}
	}
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveDown).first)
	{
		if (m_controller)
			m_controller->move(DOWN, timestep);
	}

	if (m_moving == true)
	{
		if (stepCounter == 1)
		{
			time = totalTime;
		}
		if (totalTime >= time + 0.7 && totalTime <= time + 0.8)
		{
			int soundNum = Randomiser::uniformIntBetween(1, 12);
			switch (soundNum) {
			case 1:
				getParent()->getChildEntity("walk1")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 2:
				getParent()->getChildEntity("walk2")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 3:
				getParent()->getChildEntity("walk3")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 4:
				getParent()->getChildEntity("walk4")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 5:
				getParent()->getChildEntity("walk5")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 6:
				getParent()->getChildEntity("walk6")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 7:
				getParent()->getChildEntity("walk7")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 8:
				getParent()->getChildEntity("walk8")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 9:
				getParent()->getChildEntity("walk9")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 10:
				getParent()->getChildEntity("walk10")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 11:
				getParent()->getChildEntity("walk11")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			case 12:
				getParent()->getChildEntity("walk12")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			default:
				getParent()->getChildEntity("walk1")->getComponent<StudioSound>()->playSound();
				time = totalTime;
				break;
			}
		}
	}
}

//! onKeyRelease()
/*!
\param e a KeyReleasedEvent& - A key release event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::MoveForward).first || Settings::getKeys(PlayerConfig::MoveLeft).first
		|| Settings::getKeys(PlayerConfig::MoveBack).first || Settings::getKeys(PlayerConfig::MoveRight).first)
	{
		if (m_controller)
		{
			m_moving = false;
			stepCounter = 0;
		}
	}

#ifndef NG_DEPLOY
	if (e.getKeyCode() == Keys::J && InputPoller::isKeyPressed(Keys::LEFT_CONTROL))
	{
		if (!getParent()->containsComponent<RigidBody>())
		{
			getParent()->attach<RigidBody>("RigidBody1", getParent()->getParentScene()->getPhysicsWorld(), BodyTypes::PLAYER);
			getParent()->attach<SphereCollider>("SphereCollider1", 0.5f, glm::vec3(0.f, 0.f, 0.f));
		}
		else
		{
			getParent()->detach<RigidBody>();
			getParent()->detach<SphereCollider>();
		}
	}

	if (e.getKeyCode() == Settings::getKeys(PlayerConfig::FreezeCamera).first)
	{
		if (m_controller)
		{
			bool freeze = !m_controller->getFrozen();
			m_controller->setFrozen(freeze);
		}
	}
#endif

}

//! onMouseScrolled()
/*!
\param e a MouseScrolledEvent& - A mouse scrolled event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime)
{
	m_hotbar->onMouseScrolled(e, timestep, totalTime);
}

//! onMouseMoved()
/*!
\param e a MouseMovedEvent& - A mouse moved event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Player::onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime)
{
	if (m_controller)
		m_controller->rotate(EventManager::getEventData().mouseOffset.x, EventManager::getEventData().mouseOffset.y);
}

//! setGrounded()
void Player::setGrounded()
{
	m_grounded = true;
}

//! onContact()
/*!
\param collisionEntity an Entity* - A pointer to the entity
*/
void Player::onContact(Entity* collisonEntity)
{
	setGrounded();
}

//! setItem()
/*!
\param item an Items::Item - The item
\param state an uint32_t - The item state
*/
void Player::setItem(Items::Items item, uint32_t state)
{
	// Delete the item currently there
	if (getItem(item, state)) return;

	uint32_t index = 0;
	switch (item)
	{
		case Items::Hand:
		{
			index = 0;
			break;
		}
		case Items::Map:
		{
			index = 1;
			break;
		}
		case Items::Torch:
		{
			index = 2;
			break;
		}
		case Items::Keycard:
		{
			if (m_items[3])
				index = 4;
			else
				index = 3;

			break;
		}
	}

	m_items[index] = new Item(item, state);
}

//! removeItem()
/*!
\param itemType an Items::Item - The item
\param state a const uint32_t - The item state
*/
void Player::removeItem(Items::Items itemType, const uint32_t state)
{
	for (uint32_t i = 0; i < Hotbar::getMaxCount(); i++)
	{
		if (m_items[i])
		{
			if (m_items[i]->getType() == itemType && m_items[i]->getState() == state)
			{
				delete m_items[i];
				m_items[i] = nullptr;
			}
		}
	}
}

//! getItem()
/*!
\return an Item* - A pointer to the item
*/
Item* Player::getItem(Items::Items itemType)
{
	for (auto& item : m_items)
	{
		if (item)
		{
			if (item->getType() == itemType)
				return item;
		}
	}
	return nullptr;
}

//! getItem()
/*!
\param itemType an Items::Item - The item
\param state a const uint32_t - The item state
\return an Item* - A pointer to the item
*/
Item* Player::getItem(Items::Items itemType, const uint32_t state)
{
	for (auto& item : m_items)
	{
		if (item)
		{
			if (item->getType() == itemType && item->getState() == state)
				return item;
		}
	}
	return nullptr;
}

//! getItem()
/*!
\param hotbarIndex a const uint32_t
\return an Item* - A pointer to the item
*/
Item* Player::getItem(const uint32_t hotbarIndex)
{
	return m_items[hotbarIndex];
}

//! getHotbar()
/*!
\return a Hotbar* - A pointer to the player's hotbar
*/
Hotbar* Player::getHotbar()
{
	return m_hotbar;
}