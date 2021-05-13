/*! \file settingsScript.cpp
*
* \brief A script to show the settings menu
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/settingsScript.h"
#include "independent/entities/entity.h"
#include "independent/systems/components/scene.h"
#include "loaders/sandboxLoader.h"

//! SettingsScript()
SettingsScript::SettingsScript()
{
	setScriptName("SettingsScript");
}

//! ~SettingsScript()
SettingsScript::~SettingsScript()
{
}

//! onAttach()
void SettingsScript::onAttach()
{
	//SandboxLoader::loadEntities(getParent()->getParentScene(), nullptr, "assets/scenes/common/settingsMenu.json");
}