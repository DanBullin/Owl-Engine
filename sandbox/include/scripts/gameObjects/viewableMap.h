/*! \file viewableMap.h
*
* \brief A script to display a 3D map
*
* \author: Daniel Bullin
*
*/
#ifndef VIEWABLEMAP_H
#define VIEWABLEMAP_H

#include "independent/entities/components/nativeScript.h"
#include "scripts/gameObjects/player.h"

using namespace OwlEngine;

/*! \class ViewableMap
* \brief A script to display a 3D map
*/
class ViewableMap : public NativeScript
{
private:
	Player* m_player;
public:
	ViewableMap(nlohmann::json scriptData); //!< Constructor
	~ViewableMap(); //!< Destructor

	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onAttach() override; //!< Called when attached to the entity
};
#endif