/*! \file inspector.h
*
* \brief A system which inspects data through ImGui
*
* \author Joshua Simons
*
*/
#ifndef NG_DEPLOY
#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "independent/systems/system.h"
#include "platform/ImGui/ImGuiHelper.h"
#include "independent/entities/entity.h"
#include "systems/components/timings.h"
#include "resourceManager.h"

namespace OwlEngine
{
	/*! \class Inspector
	* \brief A system which loads various ImGui elements
	*/
	class Inspector : public System
	{
	private:
		static bool s_enabled; //!< Is this system enabled
		static std::string s_search; //!< Name of whats being searched for
		static std::string s_name; //!< Name of object being inspected
		static std::string s_new; //!< Name of new obj to be created
		static ImGuiStyle* s_style; //!< Pointer to the main imgui style 
		static Entity* s_obj; //!< Pointer to obj being inspected
		static Timings* s_timings; //!< Pointer to Timings
		static ResourceType s_resourceType; //!< Type of resources we're looking at with the Properties Editor
		static Material* s_material; //!< The selected material
		static float s_timingsScale; //!< How the timings should be scaled
		static glm::ivec2 s_viewPortSize; //!< Stores the internal size of the viewport
		static bool s_viewPortChanged; //!< Stores whether the viewport is a different size
		static bool s_reParenting; //!< Whether the selected entity is being re-parented (affects rending of hierarchy)
		static bool s_cantParent; //!< Can't parent the current thing we're trying to reparent

		static void menuBar(); //!< Lets the user set up and siaply a menu bar to the top of the screen
		static void sceneView(); //!< Allows us to see the visual output of the rendering
		static void heirarchy(); //!< Lets us see the list of entities in scene and select which to inspect
		static void selectedEnt(); //!< Shows the details of the selected entity
		static void sceneInfo(); //!< Shows the data about layers, scene config etc
		static void profiler(); //!< Gives extra data about FPS etc
		static void search(); //!< Allows the player to search for an entity by name
		static void resourceProperties(); //!< Lets the user edit materials (and possibly add new ones?)
		static bool detachButton(EntityComponent* component); //!< Simple button for detaching components (used a lot)
		static bool drawNestedEnt(const std::string name, Entity* ent, float offset); //!< Draws a nested entity at the offset (recurses through child entities)

	public:
		Inspector(); //!< Constructor
		~Inspector(); //!< Destructor
		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static void initialise(); //!< Initialise the inspector
		static void inspect(); //!< Draws the current stuff ebing inspected

		static glm::vec2 getViewPortSize(); //!< Gets the size of the viewing area of the inspector
		static bool getViewPortChanged(bool reset = false); //!< Gets whethere the viewport has changed size

		static Entity* getSelectedEntity(); //!< Get the currently selected entity
		static void deselectEntity(); //!< Deselect selected entity

		static void addComponent(ComponentType type); //!< Add a component to the selected entity
		static void addEntity(); //!< Add an entity
	};
}
#endif
#endif