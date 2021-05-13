/*! \file gameOverScript.h
*
* \brief A script to show the game over display
*
* \author: Jamie Hall
*
*/
#ifndef GAMEOVERSCRIPT_H
#define GAMEOVERSCRIPT_H

#include "independent/entities/components/nativeScript.h"

using namespace OwlEngine;

/*! \class PauseScript
* \brief A script which shows the pause menu
*/
class GameOverScript : public NativeScript
{
public:
	GameOverScript(); //!< Constructor
	~GameOverScript(); //!< Destructor
	void onAttach() override; //!< On attach

	void activateGameOver();	//!< Called when player gets caught, displays the game over screen
};
#endif