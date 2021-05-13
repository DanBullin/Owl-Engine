/*! \file blobber.cpp
*
* \brief A Blobber dummy class
*
* \author: Joshua Simons
*
*/
#include "sandbox_pch.h"
#include "scripts/gameObjects/blobber.h"
#include "independent/entities/entity.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

//! Blobber()
Blobber::Blobber()
{
	setScriptName("Blobber");
}

Blobber::Blobber(nlohmann::json scriptData)
{
	setScriptName("Blobber");
	m_originalScale = glm::vec3(scriptData["originalScale"][0], scriptData["originalScale"][1], scriptData["originalScale"][2]);
	m_scaleAmount = glm::vec3(scriptData["blobbing"][0], scriptData["blobbing"][1], scriptData["blobbing"][2]);
	m_blobSpeed = scriptData["speed"];
}

//! ~Blobber()
Blobber::~Blobber()
{
}

//! onKeyPress()
/*!
\param e a KeyPressedEvent& - A key press event
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void Blobber::onPreUpdate(float timestep, float totalTime)
{
	Transform* transform = m_parentEntity->getComponent<Transform>();
	m_timePassed += timestep;
	
	if (transform)
	{
		m_amountScaled = m_scaleAmount * glm::cos(m_timePassed * m_blobSpeed);
		transform->setScale(m_originalScale + m_amountScaled);
	}
}

//! getAsJSON()
/*!
\return a nlohmann::ordered_json - The component's data in JSON format
*/
nlohmann::ordered_json Blobber::getAsJSON()
{
	nlohmann::ordered_json data;
	data["name"] = m_name;
	data["type"] = Components::toString(m_componentType);
	data["scriptName"] = getScriptName();
	data["originalScale"] = { m_originalScale.x, m_originalScale.y, m_originalScale.z };
	data["blobbing"] = { m_scaleAmount.x, m_scaleAmount.y, m_scaleAmount.z };
	data["speed"] = m_blobSpeed;
	return data;
}

#ifndef NG_DEPLOY
void Blobber::displayImGui()
{
	NativeScript::displayImGui();
	if (ImGui::DragFloat3("original Scale", &m_originalScale.x, 0.1f, 0.f, 0.f, "%.2f")) setChanged(true);
	if (ImGui::DragFloat3("blobbing amount", &m_scaleAmount.x, 0.001f, 0.f, 0.f, "%.3f")) setChanged(true);
	if (ImGui::DragFloat("blobbing speed", &m_blobSpeed, 0.01f, 0.f, 0.f, "%.2f")) setChanged(true);
	if (ImGui::DragFloat3("amount scaled", &m_amountScaled.x, 0.001f, 0.f, 0.f, "%.3f")) setChanged(true);
}
#endif // NG_DEPLOY