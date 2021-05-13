/*! \file audioManager.h
*
* \brief an audio manager that manages 3D audio.
*
* \author Aaron Taleghani
*
*/
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "independent/core/common.h"
#include "independent/systems/system.h"

#include <fmod_studio.hpp>
#include <fmod.hpp>

namespace OwlEngine
{
	/*! \struct AudioGeometryDefinition
	* \brief A set of audio geometry definitions
	*/
	struct AudioGeometryDefinition {
		std::vector<glm::vec3>* vertices; //!< Default vertices of audio geometry
		std::vector<uint32_t>* indices; //!< Default indices of audio geometry
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f); //!< Default position of audio geometry
		glm::vec3 forward = glm::vec3(0.f, 0.f, -1.f); //!< Default forward position of audio geometry
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f); //!< Default up position of audio geometry
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f); //!< Default scale of audio geometry
		float directOcculstion = 1.0f; //!< Default vertices of audio geometry
		float reverbOcculstion = 0.3f; //!< Default vertices of audio geometry
	};

	enum class RollOff { Inverse, InverseTapered, Linear, LinearSquared };
	
	/*! \class AudioManager
	* \brief audio manager to handle 3D audio recources.
	*/
	class AudioManager : public System
	{
	private:
		static bool s_enabled; //!< Is the system enabled?
		static bool s_mute; //!< Is audio muted
		static float s_volume; //!< Is audio muted

		static FMOD::Studio::System* s_studioSystem; //!< Studio system object
		static FMOD::Studio::EventInstance* s_instanceSystem; //!< Studio system object
		static FMOD::System* s_lowLevelSystem; //!< Low level system object

		static const int32_t s_maxChannels = 32; //!< Max number of channel IDs
		static int32_t s_nextChannelId; //!< Next channel ID

		static std::map<std::string, FMOD::Sound*> s_sounds; //!< List of low level sounds
		static std::map<int32_t, FMOD::Channel*> s_channnels; //!< List of audio channels
		static std::map<std::string, FMOD::Geometry*> s_geometry; //!< List of audio geometry
		static std::map<std::string, FMOD::Studio::EventInstance*> s_events; //!< List of studio events
		static std::map<std::string, FMOD::Studio::Bank*> s_banks; //!< List of studio banks
		const static int32_t errorCheck(FMOD_RESULT result); //!< Returns FMOD error codes
		static FMOD_VECTOR GLMVecToFmod(const glm::vec3& vec); //!< Converts GLM vectors to FMOD vectors
	public:
		AudioManager(); //!< Constructor
		~AudioManager(); //!< Deconstructor

		void start() override; //!< Start audio manager
		void stop() override; //!< Stop audio manager
		static void onUpdate(); //!< Called once every frame

		static void loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags); //!< Loads studio banks
		static void loadEvent(const std::string& strEventName, const std::string& eventPath); //!< Loads studio events
		static void loadSound(const std::string& strSoundName, const std::string& soundFilePath, bool b3d = true, bool blooping = false, bool bStream = false, float minDist = 0.25f, float maxDist = 1000.f, RollOff rollOff = RollOff::Inverse); //!< Loads sound files
		static void unLoadSound(const std::string& strSoundName); //!< Unloads sound files

		static void set3dListenerAndOrientation(glm::mat4& transform, glm::vec3& velocity); //!< Creates and sets 3D listener
		static void addGeometry(const std::string& label, const AudioGeometryDefinition& def); //!< Adds audio geometry
		static void moveGeometry(const std::string& label, const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up, const glm::vec3& scale); //!< Move a piece of asudio geometry

		static int32_t playSound(const std::string& strSoundName, const glm::vec3& positon); //!< Plays loaded audio files
		static void stopSound(const int32_t channel); //!< Stop a sound
		static void playEvent(const std::string& strEventName); //!< Plays studio events
		static void toggleChannelPause(int32_t nChannelID); //!< Pause a channel
		static void stopEvent(const std::string& strEventName, bool bImmediate = false); //!< Stops playing studio event
		static void setEventParameter(const std::string& strEventName, const std::string& strParamName, float value); //!< Sets event parameters
		static void setEvent3DAttributes(const std::string& strEventName, const glm::mat4& transform, const glm::vec3 velocity); //!< Sets event 3D attributes
		static void setChannel3dAttributes(int32_t nChannelID, const glm::vec3& pos, const glm::vec3& velocity); //!< Set a channel's attributes

		static bool isPlaying(int32_t nChannelID); //!< Is a channel being played
		static bool isEventPlaying(const std::string& strEventName); //!< Is a channel being played

		static void muteAudio(bool mute); //!< Mute the audio
		static bool isAudioMuted(); //!< Get whether audio is muted

		static void setEventVolume(const std::string& strEventName);
		static void getVolumeSettings(const float& volume);

		static std::map<std::string, FMOD::Sound*>& getSounds(); //!< Get a list of all loaded sounds
		static std::map<std::string, FMOD::Studio::EventInstance*>& getEvents(); //!< Get a list of all loaded events
		
	};

	//! toType()
	/*!
	\param rollOff a const uint32_t - The rolloff type
	\return a const RollOff - The rolloff
	*/
	static const RollOff toType(const uint32_t rollOff)
	{
		switch (rollOff)
		{
			case 0: return RollOff::Inverse;
			case 1: return RollOff::InverseTapered;
			case 2: return RollOff::Linear;
			case 3: return RollOff::LinearSquared;
			default: return RollOff::Inverse;
		}
	}
}
#endif