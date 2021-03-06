/*! \file inputPoller.cpp
*
* \brief An input poller class
*
* \author DMU Course material
*
*/
#include "engine_pch.h"
#include "independent/events/inputPoller.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/GLFW/GLFWInputPoller.h"
#endif

namespace OwlEngine
{
	//! getMouseX()
	/*!
	\return a float - The mouse x position
	*/
	float InputPoller::getMouseX()
	{
		return getMousePosition().x;
	}

	//! getMouseY()
	/*!
	\return a float - The mouse y position
	*/
	float InputPoller::getMouseY()
	{
		return getMousePosition().y;
	}

#ifdef NG_PLATFORM_WINDOWS

	//! isAnyKeyPressed()
	/*!
	\return a std::vector<uint32_t> - A list of all keys currently pressed
	*/
	std::vector<uint32_t> InputPoller::isAnyKeyPressed()
	{
		return GLFWInputPoller::isAnyKeyPressed();
	}

	//! isAnyMouseButtonPressed()
	/*!
	\return a std::vector<uint32_t> - A list of all mouse buttons currently pressed
	*/
	std::vector<uint32_t> InputPoller::isAnyMouseButtonPressed()
	{
		return GLFWInputPoller::isAnyMouseButtonPressed();
	}

	//! isKeyPressed()
	/*!
	\param keyCode a const int32_t - The key pressed
	\return a bool - Is the key pressed
	*/
	bool InputPoller::isKeyPressed(const int32_t keyCode)
	{
		return GLFWInputPoller::isKeyPressed(keyCode);
	}

	//! isMousePressed()
	/*!
	\param mouseButton a const int32_t - The mouse button pressed
	\return a bool - Is the mouse button pressed
	*/
	bool InputPoller::isMousePressed(const int32_t mouseButton)
	{
		return GLFWInputPoller::isMousePressed(mouseButton);
	}

	//! getMousePosition()
	/*!
	\return a glm::vec2 - The mouse position
	*/
	glm::vec2 InputPoller::getMousePosition()
	{
		return GLFWInputPoller::getMousePosition();
	}
	
#endif

}