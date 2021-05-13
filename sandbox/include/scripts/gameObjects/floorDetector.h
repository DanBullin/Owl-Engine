/*! \file floorDetector.h
*
* \brief A floor detector script to check if the player can jump
*
* \author: Jamie Hall
*
*/
#ifndef FLOORDETECTOR_H
#define FLOORDETECTOR_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"

using namespace OwlEngine;
/*! \class FloorDetector
* \brief A floor detector script
*/
class FloorDetector : public NativeScript
{
private:
	Text* m_text1;
	bool m_show;
public:
	FloorDetector(); //!< Constructor
	~FloorDetector(); //!< Destructor
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onRender(const Renderers renderer, const std::string& renderState) override; //!< Call upon render
	void onTrigger(Entity* overlapEntity) override; //!< On trigger
};
#endif