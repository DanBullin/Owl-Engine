/*! \file skybox.cpp
*
* \brief A skybox component that is attatched to the main camera
*
* \author Daniel Bullin	and Jamie Hall
*/

#include "engine_pch.h"
#include "independent/entities/components/skybox.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/renderers/renderer3D.h"

namespace OwlEngine 
{
	//! Skybox()
	/*!
	\param model a Model3D* - Pointer to the box model used
	\param material a Material* - Pointer to the sky material used
	*/
	Skybox::Skybox(Model3D* model, Material* material)
	{
		m_material = nullptr;
		m_model = nullptr;
		setModel(model);
		setMaterial(material);
	}

	//! ~Skybox()
	Skybox::~Skybox()
	{
		ENGINE_TRACE("[Skybox::~Skybox] Deleting skybox.");
		m_material, m_model = nullptr;
	}

	//! onRender()
	void Skybox::onRender()
	{
		if (m_material)
		{
			if (m_model)
			{
				Renderer3D::submit("Skybox", m_model->getMeshes().at(0).getGeometry(), m_material, glm::mat4(1.f));
			}
			else
				ENGINE_ERROR("[Skybox::onRender] Cannot render as the model is invalid.");
		}
		else
			ENGINE_ERROR("[Skybox::onRender] Cannot render as the material is invalid.");
	}

	//! setModel()
	/*!
	\param model a Model3D* - A pointer to the skybox model
	*/
	void Skybox::setModel(Model3D* model)
	{
		if (model)
			m_model = model;
		else
			ENGINE_ERROR("[Skybox::setModel] Cannot set the model as it is invalid.");
	}

	//! setMaterial()
	/*!
	\param material a Material* - A pointer to the skybox material
	*/
	void Skybox::setMaterial(Material* material)
	{
		if (material)
			m_material = material;
		else
			ENGINE_ERROR("[Skybox::setMaterial] Cannot set the material as it is invalid.");
	}
}