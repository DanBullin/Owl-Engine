/*! \file audioManager.cpp
*
* \brief An audio manager that manages 3D Audio.
*
* \author Aaron Taleghani
*
*/
#include "engine_pch.h"
#include "independent/systems/systems/audioManager.h"
#include "independent/systems/systems/log.h"

namespace OwlEngine
{
	bool AudioManager::s_enabled = false; //!< Initialise with a default value of false
	bool AudioManager::s_mute = false; //!< Initialise with a default value of false
	float AudioManager::s_volume = 0.1f;

	FMOD::Studio::System* AudioManager::s_studioSystem = nullptr; //!< Set studio system to nullptr
	FMOD::Studio::EventInstance* AudioManager::s_instanceSystem = nullptr; //!< Set studio system to nullptr
	FMOD::System* AudioManager::s_lowLevelSystem = nullptr; //!< Set low level system to nullptr

	int32_t AudioManager::s_nextChannelId = 0; //!< Next channel ID

	std::map<std::string, FMOD::Sound*> AudioManager::s_sounds = std::map<std::string, FMOD::Sound*>();
	std::map<int32_t, FMOD::Channel*> AudioManager::s_channnels = std::map<int32_t, FMOD::Channel*>();
	std::map<std::string, FMOD::Geometry*> AudioManager::s_geometry = std::map<std::string, FMOD::Geometry*>();
	std::map<std::string, FMOD::Studio::EventInstance*> AudioManager::s_events = std::map<std::string, FMOD::Studio::EventInstance*>();
	std::map<std::string, FMOD::Studio::Bank*> AudioManager::s_banks = std::map<std::string, FMOD::Studio::Bank*>();

	//! errorCheck()
	/*!
	\param result a FMOD_RESULT - The result of an FMOD check
	\return a const uint_32t - The error flag
	*/
	const int32_t AudioManager::errorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			ENGINE_ERROR("FMOD ERROR: {0}", result);
			return 1;
		}
		return 0;
	}

	//! GLMToFmod()
	/*!
	\param vec a const glm::vec3& - A reference to GLM::vec3
	\return a FMOD_VECTOR - The vector
	*/
	FMOD_VECTOR AudioManager::GLMVecToFmod(const glm::vec3 & vec)
	{
		FMOD_VECTOR result;
		result.x = vec.x;
		result.y = vec.y;
		result.z = vec.z;
		
		return result;
	}

	//! AudioManager()
	AudioManager::AudioManager() : System(SystemType::Audio)
	{
	}

	//! ~AudioManager()
	AudioManager::~AudioManager()
	{
	}

	//! start()
	void AudioManager::start()
	{
		if (!s_enabled)
		{
			ENGINE_INFO("[AudioManager::start] Starting the audio manager.");
			s_enabled = true;

			errorCheck(FMOD::Studio::System::create(&s_studioSystem));
			errorCheck(s_studioSystem->initialize(s_maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL));

			unsigned int version;
			errorCheck(FMOD::System_Create(&s_lowLevelSystem));
			errorCheck(s_lowLevelSystem->getVersion(&version));

			if (version < FMOD_VERSION)
			{
				ENGINE_ERROR("FMOD lib version {0} doesn't match header version", version, FMOD_VERSION);
			}

			errorCheck(s_lowLevelSystem->init(s_maxChannels, FMOD_INIT_NORMAL, NULL));
			errorCheck(s_lowLevelSystem->set3DSettings(1.f, 1.f, 1.f));
			errorCheck(s_lowLevelSystem->setGeometrySettings(150.f));
		}
	}

	//! stop()
	void AudioManager::stop()
	{
		if (s_enabled)
		{
			ENGINE_INFO("[AudioManger::stop] Stopping the audio manager.");

			for (auto& pair : s_events) errorCheck(pair.second->release());
			for (auto& pair : s_banks) errorCheck(pair.second->unload());

			errorCheck(s_studioSystem->unloadAll()); // Clean up loaded banks
			errorCheck(s_studioSystem->flushCommands());
			errorCheck(s_studioSystem->release());

			for (auto& pair : s_sounds) errorCheck(pair.second->release());
			errorCheck(s_lowLevelSystem->close());
			errorCheck(s_lowLevelSystem->release());
			s_enabled = false;
		}
	}

	//! onUpdate()
	void AudioManager::onUpdate()
	{
		std::vector<std::map<int32_t, FMOD::Channel*>::iterator > l_stoppedChannels;

		for (auto it = s_channnels.begin(); it != s_channnels.end(); ++it)
		{
			bool isPlaying = false;
			it->second->isPlaying(&isPlaying);

			if (!isPlaying)
			{
				l_stoppedChannels.push_back(it);
			}
		}
		for (auto& it : l_stoppedChannels)
		{
			s_channnels.erase(it);
		}

		errorCheck(s_lowLevelSystem->update());
		errorCheck(s_studioSystem->update());
	}

	//! loadSound()
	/*!
	\param strSoundName a const std::string& - A reference the sound filename
	\param b3d a bool - Sets b3d
	\param blooping a bool - Sets blooping
	\param bStream a bool - Sets bStream
	\param minDist a float - The min distance
	\param maxDist a float - The max distance
	\param rollOff a RollOff - The rolloff
	*/
	void AudioManager::loadSound(const std::string & strSoundName, const std::string& soundFilePath, bool b3d, bool blooping, bool bStream, float minDist, float maxDist, RollOff rollOff)
	{
		auto it = s_sounds.find(strSoundName);
		if (it != s_sounds.end())
			return;
		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= b3d ? FMOD_3D : FMOD_2D;
		eMode |= blooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

		switch (rollOff)
		{
		case RollOff::Linear:
			eMode |= FMOD_3D_LINEARROLLOFF;
			break;
		case RollOff::LinearSquared:
			eMode |= FMOD_3D_LINEARSQUAREROLLOFF;
			break;
		case RollOff::InverseTapered:
			eMode |= FMOD_3D_INVERSETAPEREDROLLOFF;
			break;
		}

		FMOD::Sound* sound = nullptr;
		errorCheck(s_lowLevelSystem->createSound(soundFilePath.c_str(), eMode, 0, &sound));

		if (b3d)
		{
			sound->set3DMinMaxDistance(minDist, maxDist);
		}
		if (sound != nullptr)
		{
			s_sounds[strSoundName] = sound;
			ENGINE_TRACE("[AudioManager::loadSound] Loaded sound: {0}.", strSoundName);
		}
	}

	//! unLoadSound()
	/*!
	\param strSoundName a const std::string& - A reference the sound filename
	*/
	void AudioManager::unLoadSound(const std::string& strSoundName)
	{
		auto it = s_sounds.find(strSoundName);
		if (it == s_sounds.end())
			return;
		errorCheck(it->second->release());
		s_sounds.erase(it);
	}

	//! playSound()
	/*!
	\param strSoundName a const std::string& - A reference the sound filename
	\param position a const glm::vec3& - A reference to the sound's position
	*/
	int32_t AudioManager::playSound(const std::string & strSoundName, const glm::vec3 & position)
	{
		if (!s_mute)
		{
			int32_t channelID = s_nextChannelId++;
			auto it = s_sounds.find(strSoundName);

			if (it == s_sounds.end())
			{
				return -1;
			}

			FMOD::Channel* channel = nullptr;
			errorCheck(s_lowLevelSystem->playSound(it->second, 0, true, &channel));
			if (channel)
			{
				FMOD_MODE currentMode;
				it->second->getMode(&currentMode);

				if (currentMode & FMOD_3D)
				{
					FMOD_VECTOR FVposition = GLMVecToFmod(position);
					FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
					errorCheck(channel->set3DAttributes(&FVposition, &vel));
				}
				errorCheck(channel->setPaused(false));
				s_channnels[channelID] = channel;
			}

			return channelID;
		}
		return 0;
	}

	//! stopSound()
	/*!
	\param channel a const int32_t - The channel to stop
	*/
	void AudioManager::stopSound(const int32_t channel)
	{
		auto it = s_channnels.find(channel);
		if (it == s_channnels.end())
			return;

		errorCheck(it->second->stop());
	}

	//! addGeometry()
	/*!
	\param label a const std::string& - A reference to the geometry's label
	\param def a const AudioGeometryDefinition& - A reference to the geometry definition
	*/
	void AudioManager::addGeometry(const std::string & label, const AudioGeometryDefinition & def)
	{
		FMOD::Geometry *geometry;

		int32_t numPolys = static_cast<int32_t>(def.indices->size() / 3);

		errorCheck(s_lowLevelSystem->createGeometry(numPolys, static_cast<int>(def.vertices->size()), &geometry));

		s_geometry[label] = geometry;
		FMOD_VECTOR triangle[3];
		FMOD_VECTOR vert = { 0,0,0 };
		int32_t polygonIndex;

		for (int32_t i = 0, j = 0; i < def.vertices->size(); i++)
		{
			vert = GLMVecToFmod(def.vertices->at(i));
			triangle[j] = vert;
			j++;

			if (j == 3)
			{
				geometry->addPolygon(def.directOcculstion, def.reverbOcculstion, true, 3, triangle, &polygonIndex);
				j = 0;
			}
		}

		geometry->setScale(&GLMVecToFmod(def.scale));
		geometry->setPosition(&GLMVecToFmod(def.position));
		geometry->setRotation(&GLMVecToFmod(def.forward), &GLMVecToFmod(def.up));
	}

	void AudioManager::moveGeometry(const std::string & label, const glm::vec3 & pos, const glm::vec3 & forward, const glm::vec3 & up, const glm::vec3 & scale)
	{
		s_geometry[label]->setScale(&GLMVecToFmod(scale));
		s_geometry[label]->setPosition(&GLMVecToFmod(pos));
		s_geometry[label]->setRotation(&GLMVecToFmod(forward), &GLMVecToFmod(up));
	}

	//! loadBank()
	/*!
	\param strBankName a const std::string& - A reference the bank name
	\param flags a FMOD_STUDIO_FLAGS - The studio flags
	*/
	void AudioManager::loadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
	{
		auto it = s_banks.find(strBankName);
		if (it != s_banks.end())
			return;
		FMOD::Studio::Bank* bank;

		errorCheck(s_studioSystem->loadBankFile(strBankName.c_str(), flags, &bank));
		if (bank) {
			s_banks[strBankName] = bank;
			ENGINE_TRACE("[AudioManager::loadBank] Loaded bank: {0}.", strBankName);
		}
	}

	//! loadEvent()
	/*!
	\param strEventName a const std::string& - A reference the Event name
	*/
	void AudioManager::loadEvent(const std::string & strEventName, const std::string& eventPath)
	{
		auto it = s_events.find(strEventName);
		if (it != s_events.end())
			return;
		FMOD::Studio::EventDescription* eventDescription = NULL;

		errorCheck(s_studioSystem->getEvent(eventPath.c_str(), &eventDescription));

		if (eventDescription)
		{
			FMOD::Studio::EventInstance* eventInstance = NULL;
			errorCheck(eventDescription->createInstance(&eventInstance));
			if (eventInstance)
			{
				s_events[strEventName] = eventInstance;
				ENGINE_TRACE("[AudioManager::loadEvent] Loaded event: {0}.", strEventName);
			}
		}
	}

	//! playEvent()
	/*!
	\param strEventName a const std::string& - A reference the Event name
	*/
	void AudioManager::playEvent(const std::string & strEventName)
	{
		if (!s_mute)
		{
			auto it = s_events.find(strEventName);
			if (it == s_events.end())
			{
				return;
			}
			setEventVolume(strEventName);
			it->second->start();
		}
	}

	void AudioManager::setEventVolume(const std::string& strEventName)
	{
		auto it = s_events.find(strEventName);
		if (it == s_events.end())
		{
			return;
		}
		it->second->setVolume(s_volume);
	}

	void AudioManager::getVolumeSettings(const float& volume)
	{
		s_volume = volume;
	}

	//! toggleChannelPause()
	/*!
	\param nChannelID an int32_t - The channel to pause
	*/
	void AudioManager::toggleChannelPause(int32_t nChannelID)
	{
		auto it = s_channnels.find(nChannelID);
		if (it == s_channnels.end())
			return;

		bool paused;
		errorCheck(it->second->getPaused(&paused));
		paused = !paused;
		errorCheck(it->second->setPaused(paused));
	}

	//! stopEvent()
	/*!
	\param strEventName a const std::string& - A reference the Event name
	\param bImmediate a bool - Sets if the event stops immediately
	*/
	void AudioManager::stopEvent(const std::string & strEventName, bool bImmediate)
	{
		auto it = s_events.find(strEventName);
		if (it == s_events.end())
			return;

		FMOD_STUDIO_STOP_MODE eMode;
		eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
		errorCheck(it->second->stop(eMode));
	}

	//! setEventParameter()
	/*!
	\param strEventName a const std::string& - A reference the Event name
	\param strParamName a const std::string& - A reference the parameter name
	\param value a float - The value of the parameter
	*/
	void AudioManager::setEventParameter(const std::string & strEventName, const std::string & strParamName, float value)
	{
		auto it = s_events.find(strEventName);
		if (it == s_events.end())
			return;
		errorCheck(it->second->setParameterByName(strParamName.c_str(), value));
	}

	//! set3dListenerAndOrientation()
	/*!
	\param transform a glm::mat4& - A reference to the listener's transform
	\param velocity a glm::vec3& - A reference to the listener's velocity
	*/
	void AudioManager::set3dListenerAndOrientation(glm::mat4 & transform, glm::vec3 & velocity)
	{
		glm::vec3 up = { transform[1][0], transform[1][1], transform[1][2] };
		glm::vec3 forward = { transform[2][0], transform[2][1], transform[2][2] };
		glm::vec3 position = { transform[3][0], transform[3][1], transform[3][2] };

		auto listenerPos = GLMVecToFmod(position);
		auto listenerForward = GLMVecToFmod(forward);
		auto listenerUp = GLMVecToFmod(up);
		auto listenerVelocity = GLMVecToFmod(velocity);

		FMOD_3D_ATTRIBUTES f;
		f.position = listenerPos;
		f.forward = listenerForward;
		f.up = listenerUp;
		f.velocity = listenerVelocity;

		errorCheck(s_lowLevelSystem->set3DListenerAttributes(0, &listenerPos, &listenerVelocity, &listenerForward, &listenerUp));
		errorCheck(s_studioSystem->setListenerAttributes(0, &f));
	}

	//! setEvent3DAttributes()
	/*!
	\param strEventName a const std::string& - A reference the Event name
	\param transform a const glm::mat4& - A reference to the event's transform
	\param velocity a const glm::vec3& - A reference to the event's velocity
	*/
	void AudioManager::setEvent3DAttributes(const std::string& strEventName, const glm::mat4& transform, const glm::vec3 velocity)
	{
		auto it = s_events.find(strEventName);
		if (it == s_events.end())
			return;

		glm::vec3 up = { transform[1][0], transform[1][1], transform[1][2] };
		glm::vec3 forward = { transform[2][0], transform[2][1], transform[2][2] };
		glm::vec3 position = { transform[3][0], transform[3][1], transform[3][2] };

		FMOD_3D_ATTRIBUTES f;
		f.position = GLMVecToFmod(position);
		f.forward = GLMVecToFmod(forward);
		f.up = GLMVecToFmod(up);
		f.velocity = GLMVecToFmod(velocity);

		errorCheck(it->second->set3DAttributes(&f));

	}

	//! setChannel3dAttributes()
	/*!
	\param nChannelID an int32_t - The channel ID
	\param pos a const glm::vec3& - The position to set
	\param velocity a const glm::vec3& - The velocity to set
	*/
	void AudioManager::setChannel3dAttributes(int32_t nChannelID, const glm::vec3 & pos, const glm::vec3 & velocity)
	{
		auto it = s_channnels.find(nChannelID);
		if (it == s_channnels.end())
			return;

		FMOD_VECTOR position = GLMVecToFmod(pos);
		FMOD_VECTOR vel = GLMVecToFmod(velocity);
	
		errorCheck(it->second->set3DAttributes(&position, &vel));
	}

	//! isPlaying()
	/*!
	\param nChannelID an int32_t - The channel ID
	\return a bool - Is the channel currently playing
	*/
	bool AudioManager::isPlaying(int32_t nChannelID)
	{
		auto it = s_channnels.find(nChannelID);
		if (it == s_channnels.end())
			return false;

		bool playing;
		errorCheck(it->second->isPlaying(&playing));
		return playing;
	}

	//! isEventPlaying()
	/*!
	\param strEventName a const std::string& - The event name to check if it's playing
	\return a bool - Is the event currently playing
	*/
	bool AudioManager::isEventPlaying(const std::string & strEventName)
	{
		auto it = s_events.find(strEventName);
		if (it == s_events.end())
			return false;

		FMOD_STUDIO_PLAYBACK_STATE state = FMOD_STUDIO_PLAYBACK_FORCEINT;
		it->second->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_PLAYING)
			return true;

		return false;
	}

	//! muteAudio()
	/*!
	\param mute a bool - Mute the audio
	*/
	void AudioManager::muteAudio(bool mute)
	{
		s_mute = mute;
	}

	//! isAudioMuted()
	/*!
	\return a bool - Is the audio muted
	*/
	bool AudioManager::isAudioMuted()
	{
		return s_mute;
	}

	//! getSounds()
	/*
	\return a std::map<std::string, FMOD::Sound*>& - The map of the sounds
	*/
	std::map<std::string, FMOD::Sound*>& AudioManager::getSounds()
	{
		return s_sounds;
	}

	//! getEvents()
	/*
	\return a std::map<std::string, FMOD::Studio::EventInstance*>& - The map of the events
	*/
	std::map<std::string, FMOD::Studio::EventInstance*>& AudioManager::getEvents()
	{
		return s_events;
	}
}