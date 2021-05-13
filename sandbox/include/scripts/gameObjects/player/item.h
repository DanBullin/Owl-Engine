/*! \file item.h
*
* \brief An item object
*
* \author: Daniel Bullin
*
*/
#ifndef ITEM_H
#define ITEM_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/element.h"

using namespace OwlEngine;

namespace Items
{
	/*! \enum Items
	* \brief The different items
	*/
	enum Items
	{
		Hand, Torch, Map, Keycard
	};
}

/*! \class Item
* \brief An item object
*/
class Item : public NativeScript
{
protected:
	Items::Items m_type; //!< Item type
	uint32_t m_state; //!< The item's state
public:
	Item(Items::Items type, uint32_t state); //!< Constructor
	~Item(); //!< Destructor

	inline Items::Items getType() { return m_type; } //!< Get the item type
	inline uint32_t getState() { return m_state; } //!< Get the items state
	void setState(const uint32_t state); //!< Set the items state
};
#endif