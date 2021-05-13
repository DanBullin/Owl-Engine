/*! \file canvas.h
*
* \brief Updates UI children
*
* \author Joshua Simons
*
*/
#ifndef CANVAS_H
#define CANVAS_H

#include "independent/entities/entityComponent.h"

namespace OwlEngine
{
	namespace UI
	{
		/*! \class Canvas
		* \brief A canvas for UI
		*/
		class Canvas : public EntityComponent
		{
		private:
			bool m_initialized; //!< Whether the canvas in initialized
			bool m_watching; //!< Whether the canvas is watching for changes in screensize
			glm::vec2 m_screenSize; //!< Size of the screen the canvas is on
		public:
			Canvas(); //!< Constructor
			Canvas(glm::vec2 size, bool watching, bool init = true); //!< Constructor
			~Canvas(); //!< Destructor

			void onAttach() override; //!< Occurs when component is attached to an entity
			void onDetach() override; //!< Occurs when component is detached from an entity
			void onUpdate(const float timestep, const float totalTime) override; //!< Update the component

			void setScreenSize(glm::vec2 size); //!< Sets the screen size
			glm::vec2 getScreenSize(); //!< returns the screen size

			void printComponentDetails() override; //!< Print component details
			nlohmann::ordered_json getAsJSON() override; //!< Get the component's data in JSON format
#ifndef NG_DEPLOY
			void displayImGui() override; //!< Display the component in ImGUI
#endif // NG_DEPLOY
		};
	}
}
#endif