/*! \file doorLock.h
*
* \brief A script class for the door locks
*
* \author: Daniel Bullin
*
*/
#ifndef DOORLOCK_H
#define DOORLOCK_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/transform.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/sound.h"

using namespace OwlEngine;

enum DoorLockColour
{
	RED, BLUE, GREEN, YELLOW
};

static glm::vec3 getColour(int colour)
{
	switch (colour)
	{
		case DoorLockColour::RED:
		{
			return { 3.f, 0.f, 0.f };
		}
		case DoorLockColour::BLUE:
		{
			return { 0.f, 0.f, 3.f };
		}
		case DoorLockColour::GREEN:
		{
			return { 0.f, 3.f, 0.f };
		}
		case DoorLockColour::YELLOW:
		{
			return { 3.f, 3.f, 0.f };
		}
		default:
		{
			return { 0.f, 0.f, 0.f };
		}
	}
}

static std::string getString(DoorLockColour colour)
{
	switch (colour)
	{
		case DoorLockColour::RED:
		{
			return "RED";
		}
		case DoorLockColour::BLUE:
		{
			return "BLUE";
		}
		case DoorLockColour::GREEN:
		{
			return "GREEN";
		}
		case DoorLockColour::YELLOW:
		{
			return "YELLOW";
		}
		default:
		{
			return "NONE";
		}
	}
}

/*! \class DoorLock
* \brief A script for the door locks
*/
class DoorLock : public NativeScript
{
private:
	DoorLockColour m_lockColour; //!< The colour of the lock
	float m_turnInterval; //!< The turn interval
	float m_goalRotation; //!< The goal rotation
	float m_originalRotation; //!< The original rotation
	float m_anglePrecision; //!< The angle precision to unlock the door
	float m_minDistance; //!< The minimum distance between the lock and the player
	bool m_allowDoorOpen; //!< A bool to track whether to open the door
	bool m_doorOpenAni; //!< A bool saying the animation is playing
	std::string m_doorName; //!< The name of the door entity this lock connects with
	bool m_unlocked; //!< Is the door in an unlocked state
	bool m_showText; //!< Show the open text

	Transform* m_playerTransform; //!< The player's transform
	Transform* m_doorTransform; //!< The door's transform
	glm::vec3 m_colliderHalfExtents; //!< The collider's half extents
	glm::vec3 m_localPos; //!< The position of the collider
	Entity* m_lockTextEntity; //!< The entity for the lock text
	Entity* m_successText; //!< The entity for the success text
	Entity* m_failText; //!< The entity for the fail text
	bool m_stateTextTimer; //!< A timer for the state text
	StudioSound* m_sound; //!< A pointer to the sound

public:
	DoorLock(); //!< Constructor
	DoorLock(nlohmann::json scriptData); //!< Constructor
	~DoorLock(); //!< Destructor

	void onAttach() override; //!< On Attach
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release

	void onSubmit(const Renderers renderer, const std::string& renderState) override; //!< Call upon render if mesh render component attached
	void onRender(const Renderers renderer, const std::string& renderState) override; //!< Call upon render if mesh render component attached

	void reset(); //!< Reset the door

	nlohmann::ordered_json getAsJSON() override; //!< Get the script in JSON format

#ifndef NG_DEPLOY
	virtual void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif