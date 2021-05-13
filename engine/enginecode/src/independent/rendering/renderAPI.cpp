/*! \file renderAPI.cpp
*
* \brief The rendering API
*
* \author DMU Course material
*
*/
#include "engine_pch.h"
#include "independent/rendering/renderAPI.h"

namespace OwlEngine
{
	GraphicsAPI RenderAPI::s_API = GraphicsAPI::OpenGL; //!< Initialise the graphics API
}