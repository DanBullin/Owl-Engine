/*! \file element.h
*
* \brief A child component to a canvas
*
* \author Joshua Simons
*
*/
#ifndef ELEMENT_H
#define ELEMENT_H

#include "independent/entities/entityComponent.h"

namespace OwlEngine
{
	namespace UI
	{
		/*! \class Element
		* \brief An element to be attached to a canvas
		*/
		class Element : public EntityComponent
		{
		private:
			glm::vec2 m_anchor; //!< Anchor of the element
			glm::vec2 m_screenSize; //!< stored screensize
			glm::vec2 m_offset; //!< offset from the anchor
			glm::vec2 m_scaleSize; //!< The size of the scale
			bool m_useAbsoluteSize; //!< Use the absolute scale for the element
		public:
			Element(); //!< Constructor
			Element(glm::vec2 anchor); //!< Constructor
			~Element(); //!< Destructor

			void onAttach() override; //!< Occurs when component is attached to an entity
			void onDetach() override; //!< Occurs when component is detached from an entity
			void onUpdate(const float timestep, const float totalTime) override; //!< Update the component

			glm::vec2 getAnchor(); //!< Returns the anchor value
			void setAnchor(glm::vec2 anchor); //!< sets  a new anchor value
			void onResize(glm::vec2 size, bool whole = false); //!< function for passing resize down to 

			glm::vec2 getOffset(); //!< Returns the offset
			void setOffset(glm::vec2 offset); //!< Adjusts the offset

			glm::vec2 getScaleSize(); //!< Returns the scale size
			void setScaleSize(glm::vec2 size); //!< Adjusts the scale size

			bool getAbsoluteScale(); //!< Get whether to use absolute sizes
			void setAbsoluteScale(bool use); //!< Set whether to use absolute sizes

			glm::vec2 getScreenSize(); //!< Returns the screensize
			void setScreenSize(glm::vec2 screenSize); //!< Adjusts the screensize

			void printComponentDetails() override; //!< Print component details
			nlohmann::ordered_json getAsJSON() override; //!< Get the component's data in JSON format
#ifndef NG_DEPLOY
			void displayImGui() override; //!< Display the component in ImGUI
#endif // NG_DEPLOY
		};
	}
}
#endif