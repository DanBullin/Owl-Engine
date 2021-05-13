/*! \file gameOverLayer.h
*
* \brief The game over layer containing all entities to do with the game over screen
*
* \author: Jamie Hall
*
*/
#ifndef GAMEOVERLAYER_H
#define GAMEOVERLAYER_H

#include "independent/layers/layer.h"

using namespace OwlEngine;

/*! \class GameOverLayer
* \brief A layer for the game over screen entities
*/
class GameOverLayer : public Layer
{
public:
	GameOverLayer(); //!< Constructor
	~GameOverLayer(); //!< Destructor

	void onAttach() override; //!< Called when attaching to layer manager
	void onDetach() override; //!< Called when detaching from layer manager
	void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
};
#endif
