/*! \file gameOverText.h
*
* \brief A script for all the interactable text in the game over screen
*
* \author: Jamie Hall
*
*/
#ifndef GAMEOVERTEXT_H
#define GAMEOVERTEXT_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/text.h"
#include "independent/entities/components/sound.h"

using namespace OwlEngine;

/*! \class GameOverText
* \brief A script for all the interactable text in the game over screen
*/
class GameOverText : public NativeScript
{
private:
	Text* m_text; //!< A pointer to the text
	void loadScene(const std::string& sceneName, const std::string& sceneFolderPath); //!< Load a scene
	public:
	GameOverText(); //!< Constructor
	~GameOverText(); //!< Destructor
	void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse press
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release
	void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse move
};
#endif