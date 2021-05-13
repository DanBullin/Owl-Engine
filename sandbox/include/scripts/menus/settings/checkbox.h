/*! \file checkbox.h
*
* \brief A script for a checkbox to change settings
*
* \author: Daniel Bullin
*
*/
#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <json.hpp>
#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"
#include "settings/settings.h"

using namespace OwlEngine;

/*! \class Checkbox
* \brief A script for a checkbox to change settings
*/
class Checkbox : public NativeScript
{
private:
	MeshRender2D* m_render; //!< The 2D quad
	PlayerConfig::ConfigData m_function; //!< The function this checkbox changes
public:
	Checkbox(); //!< Constructor
	Checkbox(nlohmann::json scriptData); //!< Constructor
	~Checkbox(); //!< Destructor
	void onAttach() override; //!< Called when attached to the entity
	void onPostUpdate(const float timestep, const float totalTime) override; //!< Call after game update
	void onMouseRelease(MouseReleasedEvent& e, const float timestep, const float totalTime) override; //!< Call upon mouse release

	virtual nlohmann::ordered_json getAsJSON(); //!< Get the component details as a json object
#ifndef NG_DEPLOY
	virtual void displayImGui(); //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
};
#endif