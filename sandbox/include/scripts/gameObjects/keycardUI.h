/*! \file keycardUI.h
*
* \brief A UI script for the keycards
*
* \author: Daniel Bullin
*
*/
#ifndef KEYCARDUI_H
#define KEYCARDUI_H

#include "independent/entities/components/nativeScript.h"
#include "independent/entities/components/meshRender2D.h"

using namespace OwlEngine;

/*! \class KeycardUI
* \brief A UI script for the keycards
*/
class KeycardUI : public NativeScript
{
private:
	glm::vec4 m_tint; //!< The keycard's tint
public:
	KeycardUI(); //!< Constructor
	~KeycardUI(); //!< Destructor
	void onSubmit(const Renderers renderer, const std::string& renderState) override; //!< Call upon render

	void setTint(const glm::vec4& tint); //!< Set the tint
};
#endif