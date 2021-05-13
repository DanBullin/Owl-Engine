/*! \file ladder.h
*
* \brief A ladder script
*
* \author: Joshua Simons
*
*/
#ifndef LADDER_H
#define LADDER_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/transform.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/sound.h"

using namespace OwlEngine;

/*! \class Ladder
* \brief A cyborg script
*/
class Ladder : public NativeScript
{
private:
	float m_ascendRate; //!< How fast the player goes up
	float m_goalHeight; //!< How high the player has to be before the ladded is finished
	float m_anglePrecision; //!< The angle precision to unlock the door
	float m_minDistance; //!< The minimum distance between the lock and the player
	bool m_allowAscend; //!< A bool to track whether to let the player up
	bool m_ascendAnim; //!< A bool saying the animation is playing
	bool m_showText; //!< Show the open text

	Transform* m_playerTransform; //!< The player's transform
	Transform* m_ladderTransform; //!< The door's transform
	Entity* m_ascendTextEntity; //!< The entity for the lock text
	StudioSound* m_sound; //!< A pointer to the sound
	
public:
	Ladder(); //!< Constructor
	Ladder(nlohmann::json scriptData); //!< Constructor
	~Ladder(); //!< Destructor

	void onAttach() override; //!< On Attach
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon key release

	void reset(); //!< Reset the door

	nlohmann::ordered_json getAsJSON() override; //!< Get the script in JSON format

#ifndef NG_DEPLOY
	virtual void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif