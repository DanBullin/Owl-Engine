/*! \file nativeScript.h
*
* \brief A native script (C++) component
*
* \author Daniel Bullin
*
*/
#ifndef NATIVESCRIPT_H
#define NATIVESCRIPT_H

#include "independent/entities/entityComponent.h"
#include "independent/events/events.h"

namespace OwlEngine
{
	/*! \class NativeScript
	* \brief A native script component
	*/
	class NativeScript : public EntityComponent
	{
	private:
		std::string m_scriptName; //!< Name of the script (Aka what type it is cyborg etc)
	public:
		NativeScript(); //!< Constructor
		virtual ~NativeScript(); //!< Destructor

		virtual void onAttach() {} //!< Occurs when component is attached to an entity
		virtual void onDetach() {} //!< Occurs when component is detached from an entity
		virtual void onUpdate(const float timestep, const float totalTime) {} //!< Update function for components
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */

		virtual nlohmann::ordered_json getAsJSON(); //!< Get the component details as a json object
		virtual void printComponentDetails() {}; //!< Print component details
#ifndef NG_DEPLOY
		virtual void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
		virtual std::string getScriptName(); //!< Returns the scriptName
		virtual void setScriptName(const std::string& name); //!< SEts the scriptName

		virtual void onPreUpdate(const float timestep, const float totalTime) {} //!< Call before game update
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */
		virtual void onPostUpdate(const float timestep, const float totalTime) {} //!< Call after game update
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */
		virtual void onWindowResize(WindowResizeEvent& e, const float timestep, const float totalTime) {} //!< Called when a window is resized
			/* \param e a WindowResizeEvent& - Reference to the window resize event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onWindowFocus(WindowFocusEvent& e, const float timestep, const float totalTime) {} //!< Called when a window gains focus
			/* \param e a WindowFocusEvent& - Reference to the window focus event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onWindowLostFocus(WindowLostFocusEvent& e, const float timestep, const float totalTime) {} //!< Called when a window loses focus
			/* \param e a WindowLostFocusEvent& - Reference to the window lost focus event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onWindowMoved(WindowMovedEvent& e, const float timestep, const float totalTime) {} //!< Called when a window is moved
			/* \param e a WindowMovedEvent& - Reference to the window moved event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) {} //!< Call upon key press
			/* \param e a KeyPressedEvent& - Reference to the key pressed event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onKeyRelease(KeyReleasedEvent& e, const float timestep, const float totalTime) {} //!< Call upon key release
			/* \param e a onKeyRelease& - Reference to the key release event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onMousePress(MousePressedEvent& e, const float timestep, const float totalTime) {} //!< Call upon mouse press
			/* \param e a MousePressedEvent& - Reference to the mouse press event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) {} //!< Call upon mouse release
			/* \param e a MouseReleasedEvent& - Reference to the mouse release event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onMouseScrolled(MouseScrolledEvent& e, const float timestep, const float totalTime) {} //!< Call upon mouse scroll
			/* \param e a MouseScrolledEvent& - Reference to the mouse scroll event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onMouseMoved(MouseMovedEvent& e, const float timestep, const float totalTime) {} //!< Call upon mouse move
			/* \param e a MouseMovedEvent& - Reference to the mouse move event
			   \param timestep a const float - The timestep
			   \param totalTime a const float - The total runtime of the application */
		virtual void onSubmit(const Renderers renderer, const std::string& renderState) {} //!< On submit to the renderer
			/* \param renderer a const Renderers  - The renderer currently being used
			   \param renderState a const std::string& - The render state */
		virtual void onRender(const Renderers renderer, const std::string& renderState) {} //!< Call upon rendering
			/* \param renderer a const Renderers  - The renderer currently being used
			   \param renderState a const std::string& - The render state */

		virtual void onContact(Entity* collisionEntity) {} //!< On contact between two RP3D collider objects
			// \param collisionEntity a Entity* - The other entity involved in the collision

		virtual void onTrigger(Entity* overlapEntity) {} //!< On overlap with RP3D trigger
			// \param overlapEntity a Entity* - The other entity involved in the overlap
	};
}
#endif