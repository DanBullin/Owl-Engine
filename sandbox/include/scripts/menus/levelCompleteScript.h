/*! \file levelCompleteScript.h
*
* \brief A script to show the level complete UI
*
* \author: Daniel Bullin
*
*/
#ifndef LEVELCOMPLETESCRIPT_H
#define LEVELCOMPLETESCRIPT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"

using namespace OwlEngine;

/*! \class LevelCompleteScript
* \brief A script to show the level complete UI
*/
class LevelCompleteScript : public NativeScript
{
private:
	bool m_startFade; //!< Whether to start the fade animation
	float m_startFadeTime; //!< When to start the fade
	float m_opacity_background; //!< The opacity value for the background
	MeshRender2D* m_meshRender; //!< The background

	void setInitialSettings(); //!< Set the initial settings
public:
	LevelCompleteScript(); //!< Constructor
	LevelCompleteScript(nlohmann::json scriptData); //!< Constructor
	~LevelCompleteScript(); //!< Destructor
	
	void play();
	void onRender(const Renderers renderer, const std::string& renderState) override; //!< Call upon render
	void onPreUpdate(const float timestep, const float totalTime) override;

	nlohmann::ordered_json getAsJSON() override; //!< The json data for the script

#ifndef NG_DEPLOY
	void displayImGui() override; //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif