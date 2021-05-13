/*! \file gameApplication.cpp
*
* \brief The game application
*
* \author Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "gameApplication.h"
#include "loaders/sandboxLoader.h"
#include "independent/systems/systems/scriptManager.h"

//! GameApplication()
GameApplication::GameApplication()
{
	SandboxLoader::setLoadcallback();
	ScriptManager::setNewScriptCallback(std::bind(&SandboxLoader::createNewScript, std::placeholders::_1, std::placeholders::_2));
	SandboxLoader::load("loading", "assets/scenes/loading/");
}

//! ~GameApplication()
GameApplication::~GameApplication()
{
}

//! startApplication()
/*!
\return a Application* - A pointer to the engine instance
*/
Application* OwlEngine::startApplication()
{
	return new GameApplication();
}