/*! \file meshRender3D.cpp
*
* \brief A 3D mesh render which contains the 3D geometry to render and the material to apply to the geometry.
*
* \author Daniel Bullin
*
*/
#include "engine_pch.h"
#include "independent/entities/components/meshRender3D.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/log.h"
#include "independent/rendering/renderers/renderer3D.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! MeshRender3D()
	/*!
	\param model a Model3D* - A pointer to the 3D model
	\param material a Material* - A pointer to the material
	*/
	MeshRender3D::MeshRender3D(Model3D* model, Material* material) 
		: EntityComponent(ComponentType::MeshRender3D), m_model(model), m_material(material)
	{
	}

	//! ~MeshRender3D()
	MeshRender3D::~MeshRender3D()
	{
		m_material = nullptr;
		m_model = nullptr;
	}

	//! onAttach()
	void MeshRender3D::onAttach()
	{
	}

	//! onDetach
	void MeshRender3D::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void MeshRender3D::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void MeshRender3D::printComponentDetails()
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[MeshRender3D::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("MeshRender3D Details for Entity: {0}", parent->getName());
		ENGINE_TRACE("==========================================");
		if(m_model) ENGINE_TRACE("Model Name: {0}", m_model->getName());
		if(m_material) ENGINE_TRACE("Material Name: {0}", m_material->getName());
		ENGINE_TRACE("==========================================");
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json MeshRender3D::getAsJSON()
	{
		nlohmann::ordered_json data;
		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		data["modelName"] = m_model->getName();
		if (m_material) data["materialName"] = m_material->getName();
		else data["materialName"] = "";

		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void MeshRender3D::displayImGui()
	{
		ImGui::Text(std::string("Model: " + m_model->getName()).c_str());
		if(m_material) ImGui::Text(std::string("Material: " + m_material->getName()).c_str());
		if (m_material) ImGui::Text(std::string("Shader: " + m_material->getShader()->getName()).c_str());

		//Model
		ImGui::BeginGroup();
		if (ImGui::BeginCombo(std::string("model###model01" + m_name).c_str(), m_model->getName().c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			auto models = ResourceManager::getResourcesOfType<Model3D>(ResourceType::Model3D);
			for (auto& model : models)
			{
				if (ImGui::Selectable(model->getName().c_str()))
				{
					m_model = model;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::EndGroup();

		std::string matName = "None";
		if (m_material != nullptr) matName = m_material->getName();
		if (ImGui::BeginCombo(std::string("material###mater00" + m_name).c_str(), matName.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			auto materials = ResourceManager::getResourcesOfType<Material>(ResourceType::Material);
			for (auto& material : materials)
			{
				if (m_material != nullptr)
				{
					std::string shaderName = m_material->getShader()->getName();
					if (material->getShader()->getName() == shaderName)
					{
						if (ImGui::Selectable(material->getName().c_str()))
						{
							m_material = material;
						}
					}
				}
				else
				{
					if (ImGui::Selectable(material->getName().c_str()))
					{
						m_material = material;
					}
				}
			}
			if (ImGui::Selectable("None"))
			{
				m_material = nullptr;
			}

			ImGui::EndCombo();
		}

		for (auto& mesh : m_model->getMeshes())
		{
			ImGui::PushID(mesh.getGeometry().ID);

			//Material
			ImGui::BeginGroup();
			std::string shaderName = mesh.getMaterial()->getShader()->getName();
			if (ImGui::BeginCombo(std::string("material###mater01" + m_name).c_str(), mesh.getMaterial()->getName().c_str(), ImGuiComboFlags_PopupAlignLeft))
			{
				auto materials = ResourceManager::getResourcesOfType<Material>(ResourceType::Material);
				for (auto& material : materials)
				{
					if (material->getShader()->getName() == shaderName)
					{
						if (ImGui::Selectable(material->getName().c_str()))
						{
							//mesh.setMaterial(material);
							mesh.setMaterial(material);
						}
					}
				}
				ImGui::EndCombo();
			}
			

			
			ImGui::EndGroup();
			
			glm::vec4& col1 = mesh.getMaterial()->getTint();
			ImGui::DragFloat3("###tintmesh", &col1.x, 0.005f, 0.f, 10.f);
			ImGui::SameLine();
			ImGui::ColorButton("###currentmesh", ImVec4(col1.r, col1.g, col1.b, col1.a), ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(20, 20));
			ImGui::SameLine();
			ImGui::Text("tint");
			ImGui::PopID();
		}
	}
#endif // NG_DEPLOY

	//!	setModel()
	/*!
	\param model a Model3D* - A pointer to the 3D model
	*/
	void MeshRender3D::setModel(Model3D* model)
	{
		if (model)
			m_model = model;
		else
			ENGINE_ERROR("[MeshRender3D::setModel] An invalid model was provided.");
	}

	//!	getModel()
	/*!
	\return a Model3D* - A pointer to the 3D model
	*/
	Model3D* MeshRender3D::getModel()
	{
		return m_model;
	}

	//!	setMaterial()
	/*!
	\param material a Material* - A pointer to the material
	*/
	void MeshRender3D::setMaterial(Material* material)
	{
		if (material)
			m_material = material;
		else
			ENGINE_ERROR("[MeshRender3D::setMaterial] An invalid material was provided.");
	}

	//!	getMaterial()
	/*!
	\return a Material* - A pointer to the material
	*/
	Material* MeshRender3D::getMaterial()
	{
		return m_material;
	}

	//! onRender()
	void MeshRender3D::onRender()
	{
		if (getParent()->containsComponent<Transform>())
		{
			if (m_material)
			{
				if (getParent()->containsComponent<NativeScript>())
				{
					getParent()->getComponent<NativeScript>()->onSubmit(Renderers::Renderer3D, "Default");
				}

				for (auto& mesh : m_model->getMeshes())
					Renderer3D::submit(getParent()->getName(), mesh.getGeometry(), m_material, getParent()->getComponent<Transform>()->getModelMatrix());
			}
			else
			{
				if (getParent()->containsComponent<NativeScript>())
				{
					getParent()->getComponent<NativeScript>()->onSubmit(Renderers::Renderer3D, "Default");
				}

				for (auto& mesh : m_model->getMeshes())
					Renderer3D::submit(getParent()->getName(), mesh.getGeometry(), mesh.getMaterial(), getParent()->getComponent<Transform>()->getModelMatrix());
			}
		}
		else
			ENGINE_ERROR("[MeshRender3D::onRender] The entity this mesh render is attached to does not have a valid transform.");
	}
}