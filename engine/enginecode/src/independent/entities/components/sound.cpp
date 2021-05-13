/*! \file sound.cpp
*
* \brief A sound component which defines sound
*
* \author Aaron Taleghani
*
*/
#include "engine_pch.h"
#include "independent/entities/components/sound.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systemManager.h"
#include "independent/entities/entity.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! AudioListener()
	/*!
	\param velocity a glm::vec3& - A reference to the listener's velocity
	*/
	AudioListener::AudioListener(glm::vec3& velocity) : EntityComponent(ComponentType::AudioListener)
	{
		m_velocity = velocity;
	}

	//! AudioListener()
	AudioListener::~AudioListener()
	{
	}

	//! onAttach()
	void AudioListener::onAttach()
	{
	}

	//! onDetach()
	void AudioListener::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void AudioListener::onUpdate(const float timestep, const float totalTime)
	{
		AudioManager::set3dListenerAndOrientation(getParent()->getComponent<Transform>()->getAudioMatrix(), m_velocity);
	}

	//! printComponentDetails()
	void AudioListener::printComponentDetails()
	{
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json AudioListener::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["velocity"] = { m_velocity.x, m_velocity.y, m_velocity.z };

		return data;
	}

#ifndef NG_DEPLOY
//! displayImGui()
	void AudioListener::displayImGui()
	{
		ImGui::DragFloat3("velocity###listenerVelocity", &m_velocity.x, 0.05f, 0.f, 0.f, "%.2f");
	}
#endif

	//! StudioSound()
	/*!
	\param strEventName a const std::string& - A reference the Event name
	\param velocity a glm::vec3& - A reference to the event's velocity
	*/
	StudioSound::StudioSound(const std::string& strEventName, glm::vec3& velocity) : EntityComponent(ComponentType::StudioSound)
	{
		m_eventName = strEventName;
		m_velocity = velocity;
	}

	//! ~StudioSound()
	StudioSound::~StudioSound()
	{
	}

	//! onAttach()
	void StudioSound::onAttach()
	{
	}

	//! onDetach()
	void StudioSound::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void StudioSound::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void StudioSound::printComponentDetails()
	{
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json StudioSound::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["eventName"] = m_eventName;
		data["velocity"] = {m_velocity.x, m_velocity.y, m_velocity.z};

		return data;
	}
  
#ifndef NG_DEPLOY
	//! displayImGui()
	void StudioSound::displayImGui()
	{
		ImGui::DragFloat3("velocity###studioVelocity", &m_velocity.x, 0.05f, 0.f, 0.f, "%.2f");

		if (ImGui::BeginCombo(std::string("event###event01" + m_name).c_str(), m_eventName.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			auto eventNames = AudioManager::getEvents();

			for (auto& event : eventNames)
			{
				if (ImGui::Selectable(event.first.c_str()))
				{
					m_eventName = event.first.c_str();
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::Button(std::string("Play Event###playEvent001").c_str(), ImVec2(150, 25)))
		{
			playSound();
		}
	}
#endif

	//! playSound()
	void StudioSound::playSound()
	{
		AudioManager::setEvent3DAttributes(m_eventName, getParent()->getComponent<Transform>()->getAudioMatrix(), m_velocity);
		AudioManager::playEvent(m_eventName);
	}

	//! stopSound()
	void StudioSound::stopSound()
	{
		AudioManager::stopEvent(m_eventName, true);
	}

	//! Sound()
	/*!
	\param strEventName a const std::string& - A reference the Event name
	\param loop a const bool - Loop this sound
	*/
	Sound::Sound(const std::string& soundName, const bool loop) : EntityComponent(ComponentType::StudioSound)
	{
		m_soundName = soundName;
		m_setToLoop = loop;
		m_channel = -1;
	}

	//! ~Sound()
	Sound::~Sound()
	{
	}

	//! onAttach()
	void Sound::onAttach()
	{
	}

	//! onDetach()
	void Sound::onDetach()
	{
		AudioManager::stopSound(m_channel);
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void Sound::onUpdate(const float timestep, const float totalTime)
	{
		AudioManager::setChannel3dAttributes(m_channel, getParent()->getComponent<Transform>()->getPosition(), { 0.f, 0.f, 0.f });
		m_isPlaying = AudioManager::isPlaying(m_channel);
	}

	//! printComponentDetails()
	void Sound::printComponentDetails()
	{
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json Sound::getAsJSON()
	{
		nlohmann::ordered_json data;

		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["soundName"] = m_soundName;

		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void Sound::displayImGui()
	{
		if (ImGui::BeginCombo(std::string("sound###sound01" + m_name).c_str(), m_soundName.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			auto soundNames = AudioManager::getSounds();

			for (auto& sound : soundNames)
			{
				if (ImGui::Selectable(sound.first.c_str()))
				{
					m_soundName = sound.first.c_str();
				}
			}

			ImGui::EndCombo();
		}

		if (ImGui::Button(std::string("Play Sound###playSound001").c_str(), ImVec2(150, 25)))
		{
			playSound();
		}
	}
#endif

	//! playSound()
	void Sound::playSound()
	{
		if (m_channel > -1)
		{
			if (!AudioManager::isPlaying(m_channel))
				AudioManager::toggleChannelPause(m_channel);
		}
		else
			m_channel = AudioManager::playSound(m_soundName, getParent()->getComponent<Transform>()->getPosition());
	}

	//! pause()
	void Sound::pause()
	{
		if (m_channel > -1)
		{
			AudioManager::toggleChannelPause(m_channel);
		}
	}

	//! isPlaying()
	/*!
	\return a bool - Is this sound currently playing
	*/
	bool Sound::isPlaying()
	{
		return m_isPlaying;
	}

	//! isLoop()
	/*!
	\return a bool - Is this sound looped
	*/
	bool Sound::isLoop()
	{
		return m_setToLoop;
	}

	//! setLoop()
	/*!
	\param loop a bool - Loop this sound
	*/
	void Sound::setLoop(bool loop)
	{
		m_setToLoop = loop;
	}
}