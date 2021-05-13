/*! \file startingMessage.h
*
* \brief A script to handle the starting message
*
* \author: Daniel Bullin
*
*/
#ifndef STARTINGMESSAGE_H
#define STARTINGMESSAGE_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/meshRender2D.h"

using namespace OwlEngine;

/*! \class StartingMessage
* \brief A script to handle the starting message
*/
class StartingMessage : public NativeScript
{
private:
	Text* m_text1; //!< The 'There's a __ following you' text
	Text* m_text2; //!< The 'monster' text
	Text* m_text3; //!< The 'RUN' text
	MeshRender2D* m_meshRender; //!< The background

	bool m_startFade; //!< Whether to start the fade animation
	float m_startFadeTime; //!< When to start the fade
	float m_opacity_text1; //!< The opacity value for the 1st lot of text
	float m_opacity_text2; //!< The opacity value for the 2nd lot of text
	float m_opacity_background; //!< The opacity value for the background

	void setInitialSettings(); //!< Set the initial settings
public:
	StartingMessage(); //!< Constructor
	StartingMessage(nlohmann::json scriptData); //!< Constructor
	~StartingMessage(); //!< Destructor

	void play(); //!< Play the starting message

	void onRender(const Renderers renderer, const std::string& renderState) override; //!< Call upon render
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update

	nlohmann::ordered_json getAsJSON() override; //!< The json data for the script

#ifndef NG_DEPLOY
	void displayImGui() override; //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif