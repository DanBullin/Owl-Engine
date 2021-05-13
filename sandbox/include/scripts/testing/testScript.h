/*! \file testScript.h
*
* \brief A script used in testing features
*
*/
#ifndef TESTSCRIPT_H
#define TESTSCRIPT_H

#include "independent/entities/components/nativeScript.h"

using namespace OwlEngine;

/*! \class TestScript
* \brief A script used in testing features
*/
class TestScript : public NativeScript
{
public:
	TestScript(); //!< Constructor
	~TestScript(); //!< Destructor
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onTrigger(Entity* overlapEntity) override; //!< On trigger
	void onContact(Entity* collidingEntity) override; //!< On trigger
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release
};
#endif