/*! \file testScript.cpp
*
* \brief A script used in testing features
*
*/
#include "sandbox_pch.h"
#include "scripts/testing/testScript.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"

//! TestScript()
TestScript::TestScript()
{
	setScriptName("TestScript");
}

//! ~TestScript()
TestScript::~TestScript()
{
}

//! onPostUpdate()
/*!
\param timestep a const float - The timestep
\param totalTime a const float - The total time of the application
*/
void TestScript::onPostUpdate(const float timestep, const float totalTime)
{
}


void TestScript::onTrigger(Entity * overlapEntity)
{
	if (overlapEntity->getName() == "Player1")
	{
		ENGINE_INFO("TEEHEE");
	}
}

void TestScript::onContact(Entity * collidingEntity)
{
	if (collidingEntity->getName() == "Player1")
	{
		ENGINE_INFO("BOOM");
	}
}

void TestScript::onKeyRelease(KeyReleasedEvent & e, const float timestep, const float totalTime)
{
	if (e.getKeyCode() == Keys::J)
	{
		//GetParent()->getChildEntity("click")->getComponent<StudioSound>()->playSound();
	}
}