/*! \file keycard.h
*
* \brief A script class for the keycards
*
* \author: Daniel Bullin
*
*/
#ifndef KEYCARD_H
#define KEYCARD_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/transform.h"
#include "independent/entities/components/text.h"
#include "scripts/gameObjects/doorLock.h"
#include "independent/entities/components/sound.h"

using namespace OwlEngine;

/*! \class Keycard
* \brief A script for the keycards
*/
class Keycard : public NativeScript
{
private:
	DoorLockColour m_lockColour; //!< The colour of the lock this keycard unlocks
	bool m_showText; //!< Show the open text

	Transform* m_playerTransform; //!< The player's transform
	Entity* m_pickUpText; //!< The entity for the pickup text
public:
	Keycard(); //!< Constructor
	Keycard(nlohmann::json scriptData); //!< Constructor
	~Keycard(); //!< Destructor

	void onAttach() override; //!< On Attach
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release

	nlohmann::ordered_json getAsJSON() override; //!< Get the script in JSON format

#ifndef NG_DEPLOY
	virtual void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif