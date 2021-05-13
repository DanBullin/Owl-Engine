/*! \file levelCompleteScript.cpp
*
* \brief A script to show the level complete UI
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/menus/levelCompleteScript.h"
#include "settings/settings.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/timerSystem.h"
#include "independent/entities/entity.h"
#include "loaders/sandboxLoader.h"
#include "scripts/layerControl.h"
#include "scripts/gameTimer.h"
#include "scripts/menus/levelCompleteText.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/utils/mathUtils.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

void LevelCompleteScript::setInitialSettings()
{
	LayerControl::hide("UI", getParent()->getParentScene());
	LayerControl::deactivate("UI", getParent()->getParentScene());
	LayerControl::show("LevelCompleteLayer", getParent()->getParentScene());
	LayerControl::activate("LevelCompleteLayer", getParent()->getParentScene());
	m_opacity_background = 0.f;
	getParent()->getParentScene()->getEntity("LevelCompleteCanvas")->getChildEntity("LevelCompleteText")->setDisplay(false);
	getParent()->getParentScene()->getEntity("LevelCompleteCanvas")->getChildEntity("LevelQuit")->setDisplay(false);
	getParent()->getParentScene()->getEntity("LevelCompleteCanvas")->getChildEntity("LevelTime")->setDisplay(false);
	m_startFade = false;
}

LevelCompleteScript::LevelCompleteScript()
{
	m_startFadeTime = 1.f;
	m_startFade = false;
	m_opacity_background = 1.f;
	m_meshRender = new MeshRender2D(ResourceManager::getResource<Material>("blackBackground"));
	setScriptName("LevelCompleteScript");
}

LevelCompleteScript::LevelCompleteScript(nlohmann::json scriptData)
{
	m_startFadeTime = scriptData["fadeStartTime"];
	m_startFade = false;
	m_opacity_background = 0.f;
	m_meshRender = new MeshRender2D(ResourceManager::getResource<Material>("blackBackground"));
	setScriptName("LevelCompleteScript");
}

LevelCompleteScript::~LevelCompleteScript()
{
	if (m_meshRender) delete m_meshRender; m_meshRender = nullptr;
}

void LevelCompleteScript::play()
{
	setInitialSettings();
	TimerSystem::startTimer("EndMsgPlayTime");
	TimerSystem::startTimer("EndMsgFadeTime");
	static_cast<GameTimer*>(getParent()->getParentScene()->getEntity("TimerEntity")->getComponent<NativeScript>())->stop();
	static_cast<LevelCompleteText*>(getParent()->getParentScene()->getEntity("LevelCompleteCanvas")->getChildEntity("LevelTime")->getComponent<NativeScript>())->setTimeText();
}

void LevelCompleteScript::onRender(const Renderers renderer, const std::string & renderState)
{
	if (renderer == Renderers::Renderer2D)
	{
		Material* mat = m_meshRender->getMaterial();
		glm::vec4 tint1 = mat->getTint();
		
		auto viewPort = getParent()->getParentScene()->getViewPort();
		Renderer2D::submit(mat->getShader(), mat->getSubTextures(), MathUtils::getModelMatrix({ viewPort.x / 2, viewPort.y / 2, -1.f }, { viewPort.x, viewPort.y, 1.f }), { tint1.r, tint1.g, tint1.b, m_opacity_background });
	}
}

void LevelCompleteScript::onPreUpdate(const float timestep, const float totalTime)
{
	float playTime = TimerSystem::getTime("EndMsgPlayTime", false, true);
	float fadeTime = TimerSystem::getTime("EndMsgFadeTime", false, true);

	if (playTime > m_startFadeTime)
	{
		m_startFade = true;
	}

	if (m_startFade)
	{
		if (fadeTime > 0.1)
		{
			if (playTime > m_startFadeTime + 2.f)
			{
				if (m_opacity_background < 1.f) m_opacity_background += 0.05f;
			}
			TimerSystem::startTimer("EndMsgFadeTime");
		}
	}

	if (m_opacity_background >= 1.f && m_startFade)
	{
		getParent()->getParentScene()->getEntity("LevelCompleteCanvas")->getChildEntity("LevelCompleteText")->setDisplay(true);
		getParent()->getParentScene()->getEntity("LevelCompleteCanvas")->getChildEntity("LevelQuit")->setDisplay(true);
		getParent()->getParentScene()->getEntity("LevelCompleteCanvas")->getChildEntity("LevelTime")->setDisplay(true);
		SceneManager::getActiveScene()->getEntity("Player1")->getComponent<CharacterController>()->setFrozen(true);
		WindowManager::getFocusedWindow()->setCursorInputMode(CursorInputMode::Visible);
		SceneManager::getActiveScene()->setPhysicsPaused(true);
		m_startFade = false;
		TimerSystem::stopTimer("EndMsgPlayTime", true, false);
		TimerSystem::stopTimer("EndMsgFadeTime", true, false);
	}
}

//! getAsJSON()
/*!
\return a nlohmann::ordered_json - The component's data in JSON format
*/
nlohmann::ordered_json LevelCompleteScript::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["fadeStartTime"] = m_startFadeTime;
	return data;
}

#ifndef NG_DEPLOY
//! displayImGui()
void LevelCompleteScript::displayImGui()
{
	ImGui::DragFloat("fade start time###pos1", &m_startFadeTime, 0.2f, 0.f, 15.f, "%.2f");
}
#endif // NG_DEPLOY