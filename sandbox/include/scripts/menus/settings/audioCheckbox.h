/*! \file audioCheckbox.h
*
* \brief A script for a checkbox to change audio settings
*
* \author: Aaron Taleghani
*
*/
#ifndef AUDIOCHECKBOX_H
#define AUDIOCHECKBOX_H

#include <json.hpp>
#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"
#include "independent/entities/components/text.h"
#include "settings/settings.h"

using namespace OwlEngine;

/*! \class Checkbox
* \brief A script for a checkbox to change settings
*/
class AudioCheckbox : public NativeScript
{
private:
	MeshRender2D* m_render; //!< The 2D quad
	Text* m_text = nullptr; //!< The text
	PlayerConfig::ConfigData m_function; //!< The function this checkbox changes
	float audioLevels[4] = {0.1, 0.25, 0.5, 1};
	int volumeIt = 0;
	float volume = 0.1;
public:
	AudioCheckbox(); //!< Constructor
	AudioCheckbox(nlohmann::json scriptData); //!< Constructor
	~AudioCheckbox(); //!< Destructor
	void onAttach() override; //!< Called when attached to the entity
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release

	virtual nlohmann::ordered_json getAsJSON(); //!< Get the component details as a json object
#ifndef NG_DEPLOY
	virtual void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif