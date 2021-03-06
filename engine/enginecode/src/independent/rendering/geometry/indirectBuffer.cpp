/*! \file indirectBuffer.cpp
*
* \brief An API agnostic Indirect buffer
*
* \author DMU Course material
*
*/
#include "engine_pch.h"
#include "independent/rendering/renderAPI.h"
#include "independent/rendering/geometry/indirectBuffer.h"
#include "platform/OpenGL/geometry/openGLIndirectBuffer.h"
#include "independent/systems/systems/log.h"

namespace OwlEngine
{
	//! IndirectBuffer()
	/*!
	\param indirectBufferName a const std::string& - The name of the indirect buffer
	*/
	IndirectBuffer::IndirectBuffer(const std::string& indirectBufferName) : Resource(indirectBufferName, ResourceType::IndirectBuffer)
	{
	}

	//! ~IndirectBuffer
	IndirectBuffer::~IndirectBuffer()
	{
	}

	//! create()
	/*!
	\param indirectBufferName a const std::string& - The name of the indirect buffer
	\param commands a DrawElementsIndirectCommand* - The commands info
	\param count a const uint32_t - The number of commands
	\return an IndirectBuffer* - The indirect buffer based on the current graphics API
	*/
	IndirectBuffer* IndirectBuffer::create(const std::string& indirectBufferName, DrawElementsIndirectCommand* commands, const uint32_t count)
	{
		switch (RenderAPI::getAPI())
		{
			case GraphicsAPI::None:
			{
				ENGINE_ERROR("[IndirectBuffer::create] No rendering API selected.");
				break;
			}
			case GraphicsAPI::OpenGL:
			{
				return new OpenGLIndirectBuffer(indirectBufferName, commands, count);
			}
			case GraphicsAPI::Direct3D:
			{
				ENGINE_ERROR("[IndirectBuffer::create] Direct3D not supported.");
				break;
			}
			case GraphicsAPI::Vulkan:
			{
				ENGINE_ERROR("[IndirectBuffer::create] Vulkan not supported.");
				break;
			}
		}
		return nullptr;
	}
}