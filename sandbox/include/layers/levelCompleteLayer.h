/*! \file levelCompleteLayer.h
*
* \brief The level complete layer containing all entities to do with the level complete menu
*
* \author: Daniel Bullin
*
*/
#ifndef LEVELCOMPLETELAYER_H
#define LEVELCOMPLETELAYER_H

#include "independent/layers/layer.h"

using namespace OwlEngine;

/*! \class LevelCompleteLayer
* \brief A layer for the level complete menu entities
*/
class LevelCompleteLayer : public Layer
{
public:
	LevelCompleteLayer(); //!< Constructor
	~LevelCompleteLayer(); //!< Destructor

	void onAttach() override; //!< Called when attaching to layer manager
	void onDetach() override; //!< Called when detaching from layer manager
	void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
};
#endif