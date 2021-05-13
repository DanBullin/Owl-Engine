/*! \file startingMessage.h
*
* \brief A script to handle the starting message
*
* \author: Daniel Bullin
*
*/
#include "sandbox_pch.h"
#include "scripts/UI/startingMessage.h"
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/timerSystem.h"
#include "independent/rendering/renderers/renderer2D.h"
#include "independent/utils/mathUtils.h"
#include "scripts/layerControl.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

//! setInitialSettings()
void StartingMessage::setInitialSettings()
{
	LayerControl::hide("UI", getParent()->getParentScene());
	LayerControl::deactivate("UI", getParent()->getParentScene());
	LayerControl::show("StartingMsg", getParent()->getParentScene());
	m_opacity_text1 = 0.f;
	m_opacity_text2 = 0.f;
	m_opacity_background = 1.f;
	m_startFade = false;
}

//! StartingMessage
StartingMessage::StartingMessage()
{
	m_startFadeTime = 3.f;
	m_startFade = false;
	m_text1 = new Text("There is a         chasing you...", { 1.0, 1.0, 1.0, 0.0 }, "Ubuntu40B");
	m_text2 = new Text("monster", { 1.0, 0.0, 0.0, 0.0 }, "Ubuntu40B");
	m_text3 = new Text("RUN!", { 0.0, 0.5, 1.0, 0.0 }, "Ubuntu40B");
	m_meshRender = new MeshRender2D(ResourceManager::getResource<Material>("blackBackground"));
	m_opacity_text1 = 1.f;
	m_opacity_text2 = 1.f;
	m_opacity_background = 1.f;
	setScriptName("StartingMessage");
}

//! StartingMessage()
/*!
\param scriptData a nlohmann::json - The script data in a json object
*/
StartingMessage::StartingMessage(nlohmann::json scriptData)
{
	m_startFadeTime = scriptData["fadeStartTime"];
	m_startFade = false;
	m_text1 = new Text("There is a         chasing you...", { 1.0, 1.0, 1.0, 0.0 }, "Ubuntu40B");
	m_text2 = new Text("monster", { 1.0, 0.0, 0.0, 0.0 }, "Ubuntu40B");
	m_text3 = new Text("RUN!", { 0.0, 0.5, 1.0, 0.0 }, "Ubuntu40B");
	m_meshRender = new MeshRender2D(ResourceManager::getResource<Material>("blackBackground"));
	m_opacity_text1 = 1.f;
	m_opacity_text2 = 1.f;
	m_opacity_background = 1.f;
	setScriptName("StartingMessage");
}

//! ~StartingMessage()
StartingMessage::~StartingMessage()
{
	if (m_text1) delete m_text1; m_text1 = nullptr;
	if (m_text2) delete m_text2; m_text2 = nullptr;
	if (m_text3) delete m_text3; m_text3 = nullptr;
	if (m_meshRender) delete m_meshRender; m_meshRender = nullptr;
}

//! play()
void StartingMessage::play()
{
	setInitialSettings();
	TimerSystem::startTimer("StartMsgPlayTime");
	TimerSystem::startTimer("StartMsgFadeTime");
}

//! onRender()
/*
\param renderer a const Renderers - The renderer being used
\param renderState a const std::string& - The render state
*/
void StartingMessage::onRender(const Renderers renderer, const std::string& renderState)
{
	if (renderer == Renderers::Renderer2D)
	{
		Material* mat = m_meshRender->getMaterial();
		glm::vec4 tint1 = mat->getTint();
		glm::vec4 tint2 = m_text1->getColour();
		glm::vec4 tint3 = m_text2->getColour();
		glm::vec4 tint4 = m_text3->getColour();
		m_text1->setColour({ tint2.r, tint2.g, tint2.b, m_opacity_text1});
		m_text2->setColour({ tint3.r, tint3.g, tint3.b, m_opacity_text1});
		m_text3->setColour({ tint4.r, tint4.g, tint4.b, m_opacity_text2});

		auto viewPort = getParent()->getParentScene()->getViewPort();
		Renderer2D::submit(mat->getShader(), mat->getSubTextures(), MathUtils::getModelMatrix({ viewPort.x/2, viewPort.y/2, -1.f }, { viewPort.x, viewPort.y, 1.f }), { tint1.r, tint1.g, tint1.b, m_opacity_background });
		Renderer2D::submitText(m_text1, MathUtils::getModelMatrix({ (viewPort.x / 2)-336, (viewPort.y / 2), 0.5f }));
		Renderer2D::submitText(m_text2, MathUtils::getModelMatrix({ (viewPort.x / 2)-110, (viewPort.y / 2), 1.f }));
		Renderer2D::submitText(m_text3, MathUtils::getModelMatrix({ (viewPort.x / 2)+350, (viewPort.y / 2)+50, 0.5f }));
	}
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void StartingMessage::onPostUpdate(const float timestep, const float totalTime)
{
	float playTime = TimerSystem::getTime("StartMsgPlayTime", false, true);
	float fadeTime = TimerSystem::getTime("StartMsgFadeTime", false, true);

	if (playTime > m_startFadeTime)
	{
		m_startFade = true;
	}

	if (m_startFade)
	{
		if (fadeTime > 0.1)
		{
			if (playTime < m_startFadeTime + 2.f)
			{
				if (m_opacity_text1 < 1.f) m_opacity_text1 += 0.05f;
			}
			else if (playTime > m_startFadeTime + 2.f && playTime < m_startFadeTime + 4.f)
			{
				if (m_opacity_text2 < 1.f) m_opacity_text2 += 0.05f;
			}
			else if (playTime > m_startFadeTime + 4.f)
			{
				if (m_opacity_background > 0.f) m_opacity_background -= 0.05f;
				m_opacity_text1 = m_opacity_background;
				m_opacity_text2 = m_opacity_background;
			}
			TimerSystem::startTimer("StartMsgFadeTime");
		}
	}

	if (m_opacity_background <= 0.f && m_startFade)
	{
		LayerControl::hide("StartingMsg", getParent()->getParentScene());
		LayerControl::show("UI", getParent()->getParentScene());
		LayerControl::activate("UI", getParent()->getParentScene());
		m_startFade = false;
		TimerSystem::stopTimer("StartMsgPlayTime", true, false);
		TimerSystem::stopTimer("StartMsgFadeTime", true, false);
	}
}

//! getAsJSON()
/*!
\return a nlohmann::ordered_json - The component's data in JSON format
*/
nlohmann::ordered_json StartingMessage::getAsJSON()
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
void StartingMessage::displayImGui()
{
	ImGui::DragFloat("fade start time###pos1", &m_startFadeTime, 0.2f, 0.f, 15.f, "%.2f");
}
#endif // NG_DEPLOY