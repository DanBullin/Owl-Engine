/*! \file hotbar.h
*
* \brief The player's hotbar
*
* \author: Daniel Bullin
*
*/
#ifndef HOTBAR_H
#define HOTBAR_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/element.h"
#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/components/text.h"

using namespace OwlEngine;

class Player;

/*! \class Hotbar
* \brief The player's hotbar
*/
class Hotbar : public NativeScript
{
private:
	static const uint32_t s_maxCount = 5; //!< The total number of hotbar slots
	uint32_t m_currentItem = 0; //!< The current hotbar slot
	uint32_t m_oldItem = 0; //!< The old hotbar slot
	UI::Element* m_currentElement; //!< The element component of the current slot
	bool m_updated; //!< Has the hotbar been updated
	std::vector<std::pair<Text*, MeshRender2D*>> m_hotbarElements; //!< The text and image pairs for the different hotbar items
	Player* m_player; //!< A pointer to the player script
public:
	Hotbar(); //!< Constructor
	~Hotbar(); //!< Destructor

	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key press
	void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse scroll

	void scrollRight(); //!< Scroll right on the hotbar
	void scrollLeft(); //!< Scroll left on the hotbar
	void selectHotbar(const uint32_t index);

	uint32_t getSelectedItem(); //!< Get the selected item index
	static uint32_t getMaxCount(); //!< Get the total number of items on the hotbar
	bool itemChanged();
};
#endif