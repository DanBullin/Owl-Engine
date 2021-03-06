/*! \file GLFWGLGraphicsContext.cpp
*
* \brief A GLFW OpenGL graphics context
*
* \author DMU Course material
*
*/
#include "engine_pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "platform/GLFW/GLFWGLGraphicsContext.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/renderUtils.h"

namespace OwlEngine
{
	//! GLFWGLGraphicsContext()
	/*!
	\param nativeWindow a GLFWwindow* - A pointer to a GLFW window
	*/
	GLFWGLGraphicsContext::GLFWGLGraphicsContext(GLFWwindow* nativeWindow) : m_window(nativeWindow)
	{
	}

	//! init()
	void GLFWGLGraphicsContext::init()
	{
		if (!m_window)
		{
			ENGINE_ERROR("[GLFWGLGraphicsContext::init] Invalid GLFW window.");
			return;
		}

		// Set the current context
		glfwMakeContextCurrent(m_window);

		// Load GL Functions
		auto result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		if (!result)
			ENGINE_ERROR("[GLFWGLGraphicsContext::init] Could not create OpenGL context for this GLFW window: {0}.", result);

		// Enable OpenGL debug with a callback
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(
			[](GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				ENGINE_ERROR(message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				ENGINE_ERROR(message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				ENGINE_ERROR(message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				//Log::trace(message);
				break;
			}
		}, nullptr);
	}

	//! swapBuffers()
	void GLFWGLGraphicsContext::swapBuffers()
	{
		// Swap the front and back buffers
		if(m_window) glfwSwapBuffers(m_window);
		else
			ENGINE_ERROR("[GLFWGLGraphicsContext::swapBuffers] Invalid GLFW window.");
	}

	//! makeCurrent()
	void GLFWGLGraphicsContext::makeCurrent()
	{
		if(m_window) glfwMakeContextCurrent(m_window);
		else
			ENGINE_ERROR("[GLFWGLGraphicsContext::makeCurrent] Invalid GLFW window.");
	}

	//! updateViewport()
	/*!
	\param x a const uint32_t - The lower left corner of the rectangle viewport
	\param y a const uint32_t - The lower left corner of the rectangle viewport
	\param width a const uint32_t - The width of the rectangle viewport
	\param height a const uint32_t - The height of the rectangle viewport
	*/
	void GLFWGLGraphicsContext::updateViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
	{
		if(m_window)
			RenderUtils::setViewport(x, y, width, height);
		else
			ENGINE_ERROR("[GLFWGLGraphicsContext::updateViewport] Invalid GLFW window.");
	}
}