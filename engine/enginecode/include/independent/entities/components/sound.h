/*! \file sound.h
*
* \brief A light component which defines sounds
*
* \author Aaron Taleghani
*
*/
#ifndef SOUND_H
#define SOUND_H

#include "independent/entities/entityComponent.h"
#include "systems/systems/audioManager.h"

namespace OwlEngine
{
	/*! \class AudioListener
	* \brief A 3D sound listener object
	*/
	class AudioListener : public EntityComponent
	{
	private:
		glm::vec3 m_velocity; //!< Velocity of the audio listener
	public:
		AudioListener(glm::vec3& velocity); //!< Constructor
		~AudioListener(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the audio listener component
		void printComponentDetails() override;	//!< Print component details
		nlohmann::ordered_json getAsJSON() override; //!< Get the component's data as JSON for saving
#ifndef NG_DEPLOY
		void displayImGui() override; //!< Get the component's data displayed in ImGui form
#endif
	};

	/*! \class StudioSound
	* \brief A studio sound object
	*/
	class StudioSound : public EntityComponent
	{
	private:
		std::string m_eventName; //!< Name of event
		glm::vec3 m_velocity; //!< Velicity of the event
	public:
		StudioSound(const std::string& strEventName, glm::vec3& velocity); //!< Constructor
		~StudioSound(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the studio sound component
		void printComponentDetails() override;	//!< Print component details
		nlohmann::ordered_json getAsJSON() override; //!< Get the component's data as JSON for saving
#ifndef NG_DEPLOY
		void displayImGui() override; //!< Get the component's data displayed in ImGui form
#endif
		void playSound(); //!< Gets event name and plays sound file
		void stopSound(); //!< Gets event name and stops sound file
	};

	/*! \class Sound
	* \brief A sound object
	*/
	class Sound : public EntityComponent
	{
	private:
		std::string m_soundName; //!< The name of the sound
		int32_t m_channel; //!< The channel the sound will be playing on
		bool m_isPlaying; //!< Is the sound currently playing
		bool m_setToLoop; //!< Loop the sound
	public:
		Sound(const std::string& soundName, const bool loop); //!< Constructor
		~Sound(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the studio sound component
		void printComponentDetails() override;	//!< Print component details
		nlohmann::ordered_json getAsJSON() override; //!< Get the component's data as JSON for saving
#ifndef NG_DEPLOY
		void displayImGui() override; //!< Get the component's data displayed in ImGui form
#endif
		void playSound(); //!< Plays the sound
		void pause(); //!< Pause
		bool isPlaying(); //!< Is this sound playing
		bool isLoop(); //!< Is this sound looping
		void setLoop(bool loop); //!< Set this sound to loop
	};
}
#endif