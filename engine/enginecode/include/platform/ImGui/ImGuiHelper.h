/*! \file ImGuiHelper.h
*
* \brief An ImGui helper class
*
* \author Aaron Taleghani
*
*/
#ifndef NG_DEPLOY
#ifndef IMGUIHELPER_H
#define IMGUIHELPER_H

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace OwlEngine
{
	/*! \class ImGuiHelper
	* \brief A class to help with ImGui elements
	*/
	class ImGuiHelper
	{
	public:
		static void init(); //!< Initialise the helper
		static void begin(); //!< Begin the helper
		static void end(); //!< End the helper
		static void shutdown(); //!< Shutdown the helper
	};
}
#endif
#endif // !NG_DEPLOY