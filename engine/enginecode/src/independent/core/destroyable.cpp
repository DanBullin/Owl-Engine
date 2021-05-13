/*! \file destroyable.cpp
*
* \brief A class which defines whether the object which inherits it is to be deleted
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/core/destroyable.h"

namespace OwlEngine
{
	//! destroy()
	void Destroyable::destroy()
	{
		m_destroy = true;
	}

	//! undestroy()
	void Destroyable::undestroy()
	{
		m_destroy = false;
	}

	//! getDestroyed()
	/*!
	\return a const bool - Is this object to be destroyed
	*/
	const bool Destroyable::getDestroyed() const
	{
		return m_destroy;
	}
}