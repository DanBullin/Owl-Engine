/*! \file player.h
*
* \brief A player script
*
* \author: Daniel Bullin
*
*/
#ifndef PLAYER_H
#define PLAYER_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/characterController.h"
#include "independent/entities/components/sound.h"
#include "systems/systems/randomiser.h"
#include "scripts/gameObjects/player/hotbar.h"
#include "scripts/gameObjects/player/item.h"

using namespace OwlEngine;

/*! \class Player
* \brief A player script
*/
class Player : public NativeScript
{
private:
	CharacterController* m_controller; //!< A character controller
	Hotbar* m_hotbar; //!< The hotbar
	Item* m_items[5]; //!< The items the player has
	StudioSound* m_sound; //!< A pointer to the sound

	bool m_grounded = false; //!< Bool to check if player is grounded
	bool m_moving = false; //!< Is the player moving

	int stepCounter = 0; //!< counter for how long the player has been walking
	float time = 0; //!< A check to play events in intervals
public:
	Player(); //!< Constructor
	~Player(); //!< Destructor

	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onAttach() override; //!< Called when attached to the entity
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
	void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse scroll
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
	void setGrounded(); //!< Set the players grounded state to true
	void onContact(Entity* collisonEntity); //!< Call upon collision with another body

	void setItem(Items::Items item, uint32_t state); //!< The hotbar index to place the item
	void removeItem(Items::Items itemType, const uint32_t state); //!< Remove an item
	Item* getItem(Items::Items itemType); //!< Get the index by type
	Item* getItem(Items::Items itemType, const uint32_t state); //!< Get the index by type
	Item* getItem(const uint32_t hotbarIndex); //!< Get the item by hotbar index
	Hotbar* getHotbar(); //!< Get the player's hotbar
};
#endif