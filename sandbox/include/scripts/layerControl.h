/*! \file layerControl.h
*
* \brief A script to help control layers more easily
*
* \author: Daniel Bullin
*
*/
#ifndef LAYERCONTROL_H
#define LAYERCONTROL_H

#include "independent/entities/components/nativeScript.h"
#include "independent/systems/components/scene.h"

using namespace OwlEngine;

/*! \class LayerControl
* \brief A script to help control layers more easily
*/
class LayerControl : public NativeScript
{
public:
	LayerControl(); //!< Constructor
	~LayerControl(); //!< Destructor
	static void hide(const std::string& layerName, Scene* scene); //!< Hide a layer
	static void show(const std::string& layerName, Scene* scene); //!< Hide a layer
	static void activate(const std::string& layerName, Scene* scene); //!< Activate a layer
	static void deactivate(const std::string& layerName, Scene* scene); //!< Deactivate a layer

	static bool isDisplayed(const std::string& layerName, Scene* scene); //!< Is a layer displayed
};
#endif