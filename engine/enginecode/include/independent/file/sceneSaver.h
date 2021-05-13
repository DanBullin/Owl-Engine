/*! \file sceneSaver.h
*
* \brief A scene saver class which saves the scene to a JSON file.
*
* \author Joshua Simons
*
*/
#ifndef SCENESAVER_H
#define SCENESAVER_H

#include "independent/systems/components/scene.h"

namespace OwlEngine
{
	/*! \class SceneSaver
	* \brief A loader class which saves scenes
	*/
	class SceneSaver
	{
	public:
		static void save(Scene* scene); //!< Save the scene
	};
}

#endif