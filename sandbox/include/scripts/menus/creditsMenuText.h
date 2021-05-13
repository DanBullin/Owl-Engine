/*! \file creditsMenuText.h
*
* \brief A script for all the interactable text in the credits menu
*
* \author: Daniel Bullin
*
*/
#ifndef CREDITSMENUTEXT_H
#define CREDITSMENUTEXT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/sound.h"

using namespace OwlEngine;

/*! \class CreditsMenuText
* \brief A script for all the interactable text in the credits menu
*/
class CreditsMenuText : public NativeScript
{
private:
	Text* m_text; //!< A pointer to the text
	void loadScene(const std::string& sceneName); //!< Load a scene
public:
	CreditsMenuText(); //!< Constructor
	~CreditsMenuText(); //!< Destructor
	void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif