/*! \file levelCompleteText.h
*
* \brief A script for all the interactable text in the level complete UI
*
* \author: Daniel Bullin
*
*/
#ifndef LEVELCOMPLETETEXT_H
#define LEVELCOMPLETETEXT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/sound.h"

using namespace OwlEngine;

/*! \class LevelCompleteText
* \brief A script for all the interactable text in the level complete menu
*/
class LevelCompleteText : public NativeScript
{
private:
	Text* m_text; //!< A pointer to the text
	void loadScene(const std::string& sceneName); //!< Load a scene
public:
	LevelCompleteText(); //!< Constructor
	~LevelCompleteText(); //!< Destructor
	void onAttach() override; //!< On attach
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Call before game update
	void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move

	void setTimeText(); //!< Set time text
};
#endif