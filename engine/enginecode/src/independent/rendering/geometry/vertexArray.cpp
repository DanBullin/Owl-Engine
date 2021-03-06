/*! \file vertexArray.cpp
*
* \brief An API agnostic vertex array
*
* \author DMU Course material
*
*/
#include "engine_pch.h"
#include "independent/rendering/renderAPI.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/geometry/vertexArray.h"
#include "platform/OpenGL/geometry/openGLVertexArray.h"

namespace OwlEngine
{
	//! VertexArray()
	/*!
	\param vertexArrayName a const std::string& - The name of the vertex array
	*/
	VertexArray::VertexArray(const std::string& vertexArrayName) : Resource(vertexArrayName, ResourceType::VertexArray)
	{
	}

	//! ~VertexArray()
	VertexArray::~VertexArray()
	{
	}

	//! create()
	/*!
	\param vertexArrayName a const std::string& - The name of the vertex array
	\return a VertexArray* - The vertex array based on the current graphics API
	*/
	VertexArray* VertexArray::create(const std::string& vertexArrayName)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[VertexArray::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLVertexArray(vertexArrayName);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[VertexArray::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[VertexArray::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}