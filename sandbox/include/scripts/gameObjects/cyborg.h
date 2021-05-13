/*! \file cyborg.h
*
* \brief A Cyborg script
*
* \author: Daniel Bullin
*
*/
#ifndef CYBORG_H
#define CYBORG_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/characterController.h"

using namespace OwlEngine;

/*! \class Cyborg
* \brief A cyborg script
*/
class Cyborg : public NativeScript
{
private:
	glm::vec3 m_originalScale;
	CharacterController* m_controller; //!< Pointer to character controller
public:
	Cyborg(); //!< Constructor
	~Cyborg(); //!< Destructor
	void onKeyPress(KeyPressedEvent& e, const float timestep, const float totalTime) override; //!< Called on key press
	nlohmann::ordered_json getAsJSON() override; //!< Get the script in JSON format

#ifndef NG_DEPLOY
	virtual void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif