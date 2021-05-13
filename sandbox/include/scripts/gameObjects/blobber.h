/*! \file blobber.h
*
* \brief A Blobber script
*
* \author: Joshua Simons
*
*/
#ifndef BLOBBER_H
#define BLOBBER_H

#include "independent/entities/components/nativeScript.h"

using namespace OwlEngine;

/*! \class Blobber
* \brief A Blobber script
*/
class Blobber : public NativeScript
{
private:
	glm::vec3 m_originalScale = glm::vec3(1);
	glm::vec3 m_scaleAmount = glm::vec3(1);
	float m_blobSpeed = 0.1f;
	float m_timePassed = 0.0f;
	glm::vec3 m_amountScaled = glm::vec3(1);
public:
	Blobber(); //!< Constructor
	Blobber(nlohmann::json scriptData); //!< Constructor
	~Blobber(); //!< Destructor
	void onPreUpdate(const float timestep, const float totalTime) override; //!< Called on key press
	nlohmann::ordered_json getAsJSON() override; //!< Get the script in JSON format

#ifndef NG_DEPLOY
	virtual void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif