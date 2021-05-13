/*! \file skybox.h
*
* \brief A skybox component that is attatched to the main camera
*
* \author Daniel Bullin	and Jamie Hall
*/

#ifndef SKYBOX_H
#define SKYBOX_H

#include "independent/entities/entityComponent.h"

namespace OwlEngine
{
	/*! \class Skybox
	* \brief A skybox class
	*/
	class Skybox
	{
	private:
		Model3D* m_model; //!< Pointer to the skybox's 3d model
		Material* m_material; //!< Pointer to the skybox's material
	public:
		Skybox(Model3D* model, Material* material); //!< Constructor, sets pointers to model and material
		~Skybox(); //!< Deconstructor, removes pointers

		void onRender(); //!< Submit skybox to renderer
		void setModel(Model3D* model); //!< Setter for 3D model
		void setMaterial(Material* material); //!< Setter for material

		Model3D* getModel() { return m_model; } //!< Getter for 3D model
			/*!< \return a Model3D* - Pointer to the skybox material*/
		Material* getMaterial() { return m_material; } //!< Getter for material
			/*!< \return a Material* - Pointer to the skybox material*/
	};
}
#endif