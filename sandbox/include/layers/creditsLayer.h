/*! \file creditsLayer.h
*
* \brief The credits layer containing all entities to do with the credits menu
*
* \author: Daniel Bullin
*
*/
#ifndef CREDITSLAYER_H
#define CREDITSLAYER_H

#include "independent/layers/layer.h"

using namespace OwlEngine;

/*! \class CreditsLayer
* \brief The credits layer containing all entities to do with the credits menu
*/
class CreditsLayer : public Layer
{
public:
	CreditsLayer(); //!< Constructor
	~CreditsLayer(); //!< Destructor

	void onAttach() override; //!< Called when attaching to layer manager
	void onDetach() override; //!< Called when detaching from layer manager
	void onUpdate(const float timestep, const float totalTime) override; //!< Called one per frame
};
#endif