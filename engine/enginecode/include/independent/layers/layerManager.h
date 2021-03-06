/*! \file layerManager.h
*
* \brief A layer manager which manages layers
*
* \author Daniel Bullin
*
*/
#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "independent/core/common.h"
#include "independent/layers/layer.h"

namespace OwlEngine
{
	class Scene; //!< Forward declare scene

	/*! \class LayerManager
	* \brief A layer manager which manages layers
	*/
	class LayerManager 
	{
	private:
		Scene* m_attachedScene; //!< The scene this manager is attached to
		std::vector<Layer*> m_layers; //!< A list of all layers
	public:
		LayerManager(Scene* parentScene); //!< Constructor
		~LayerManager(); //!< Destructor

		void attachLayer(Layer* newLayer); //!< Add a layer to the layer list
		Layer* getLayer(const std::string& layerName); //!< Get a layer by name
		std::vector<Layer*>& getLayers(); //!< Get all layers
		bool layerExists(Layer* layer); //!< Check if the layer exists in the manager

		Scene* getParentScene(); //!< Get the scene this manager is attached to

		void onUpdate(const float timestep, const float totalTime); //!< Called once every frame

		void printLayerManagerDetails(); //!< Print the layer manager details
		void printLayerDetails(); //!< Print the layer details
	};
}
#endif