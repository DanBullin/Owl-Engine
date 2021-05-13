/*! \file inspector.cpp
*
* \brief Inspects the scene and displays content to imgui
*
* \author Joshua Simons
*
*/
#include "engine_pch.h"

#ifndef NG_DEPLOY
#include "independent/systems/systems/inspector.h"
#include "independent/systems/systems/sceneManager.h"
#include "independent/systems/systems/timerSystem.h"
#include "independent/systems/systems/resourceManager.h"
#include "independent/systems/systems/fontManager.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/log.h"
#include "independent/utils/nameUtils.h"
#include "independent/systems/systemManager.h"
#include "systems/systems/timerSystem.h"
#include "independent/file/sceneSaver.h"
#include "independent/file/sceneLoader.h"
#include "rendering/materials/material.h"

namespace OwlEngine
{
	bool Inspector::s_enabled = false; //!< Initialise with default value of false
	Entity* Inspector::s_obj = nullptr; //!< Initialise to empty pointer
	Material* Inspector::s_material = nullptr; //!< Initialise to empty pointer
	ResourceType Inspector::s_resourceType = ResourceType::Material; //!< Initialise to material
	ImGuiStyle* Inspector::s_style = nullptr; //!< Initialise to empty pointer
	std::string Inspector::s_name = ""; //!< Initialise with empty string
	std::string Inspector::s_search = ""; //!< Initialise with empty string
	std::string Inspector::s_new = "newResource"; //!< Init with default name
	glm::ivec2 Inspector::s_viewPortSize = glm::ivec2(1);
	bool Inspector::s_viewPortChanged = false;
	bool Inspector::s_reParenting = false;
	bool Inspector::s_cantParent = false;
	Timings* Inspector::s_timings = nullptr; //!< Initialise to nullptr
	float Inspector::s_timingsScale = 0.03f; //!< Initialise to reasonable value 

	//! menuBar()
	void Inspector::menuBar()
	{
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Return", "ESC");
			
			if (ImGui::MenuItem("Open", "F1"))
			{
				SceneLoader::load("level1", "assets/scenes/level1/");
				bool inspector = SceneManager::getActiveScene()->getInspectorEnabled();
				SceneManager::setActiveScene("level1", true);
				SceneManager::getActiveScene()->enableInspector(inspector);
			}
			
			if (ImGui::MenuItem("Save", "F2"))
			{
				SceneSaver::save(SceneManager::getActiveScene());
			}
			
			if (ImGui::MenuItem("New"))
			{
				ENGINE_INFO("Creating new scene");
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Add Entity"))
			{
				addEntity();
			}
			if (ImGui::BeginMenu("Add Component"))
			{
				for (uint32_t i = 0; i < Components::getComponentNumber(); i++)
				{
					std::string str = Components::toString(ComponentType(i));
					if (ImGui::MenuItem(str.c_str()))
					{
						ComponentType componentType = ComponentType(i);
						if(s_obj) addComponent(componentType);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//! sceneView()
	void Inspector::sceneView()
	{
		Scene* scene = SceneManager::getActiveScene();

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse;
		ImGui::Begin("Sceneview", NULL, flags);
		glm::ivec2 viewSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
		if (viewSize != s_viewPortSize)
		{
			s_viewPortChanged = true;
			s_viewPortSize = viewSize;
			scene->setViewPort(viewSize);
		}
		else if (scene->getViewPort() != viewSize)
		{
			scene->setViewPort(viewSize);
		}

		//Get default framebuffer
		ResourceManager::getDefaultFrameBuffer()->bind();
		FrameBuffer* fbo = scene->getFinalFrameBuffer();

		if (fbo != nullptr)
		{
			Texture2D* screen = fbo->getSampledTarget("Colour0");
			uint32_t fps = static_cast<uint32_t>(TimerSystem::getFPS());
			ImVec4 color = ImVec4(1, 0, 0, 1);
			if (fps < 30) color = ImVec4(1, 0, 0, 1);
			else if (fps < 60) color = ImVec4(1, 1, 0, 1);
			else if (fps < 90) color = ImVec4(0, 1, 0, 1);
			else if (fps < 120) color = ImVec4(0, 1, 1, 1);
			else color = ImVec4(0, 0.5f, 1, 1);

			ImVec2 p = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);

			glm::vec2 newP = { p.x, p.y };
			newP -= WindowManager::getFocusedWindow()->getProperties().getPosition();
			scene->setViewPos(newP);
			if (screen) ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)screen->getID(), p, ImVec2(p.x + viewSize.x, p.y + viewSize.y), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::GetWindowDrawList()->AddText(ImVec2(p.x + 5, p.y + 5), IM_COL32(255, 255, 255, 255), "FPS ");
			ImGui::GetWindowDrawList()->AddText(ImVec2(p.x + 35, p.y + 5), IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255), std::to_string(fps).c_str());
		}
		ImGui::End();
	}

	//! heirarchy()
	void Inspector::heirarchy()
	{
		Scene* scene = SceneManager::getActiveScene();
		std::string title = "Heirarchy";
		if (scene) title = scene->getName() + "###" + title;
		ImGui::Begin(title.c_str(), NULL);
		if (ImGui::IsItemActivated() && ImGui::IsItemClicked())
		{
			if (s_reParenting && s_obj)
			{
				s_reParenting = false;
				s_cantParent = false;
				scene->addEntity(s_obj->getName(), s_obj);
			}
			else if (s_obj != nullptr)
			{
				s_obj->setSelected(false);
				s_obj = nullptr;
			}
		}

		if (scene)
		{
			if (scene->getRefreshInspector(true)) s_obj = nullptr;

			//auto roots = ;
			for (auto root : scene->getRootEntities())
			{
				drawNestedEnt(root.first, root.second, 25.0f);
			}

			//Add button
			if (ImGui::Button("+"))
			{
				addEntity();
			}
		}
		ImGui::End();
	}

	//! selectedEnt()
	void Inspector::selectedEnt()
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysVerticalScrollbar;
		ImGui::Begin("Inspector", NULL);
		if (s_obj != nullptr)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4 col = style.Colors[ImGuiCol_Button];

			//Name and hide/show
			char buf[128];
			strcpy(buf, s_obj->getName().c_str());
			ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() - 65);
			if (ImGui::InputText(std::string("###NameTop").c_str(), buf, IM_ARRAYSIZE(buf))) s_obj->setName(std::string(buf));
			ImGui::PopItemWidth();

			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 50);
			style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.7f, 0.1f, 1.f);
			if (s_obj->getDisplay())
			{
				if (ImGui::Button("Hide")) s_obj->setDisplay(false);
			}
			else if (ImGui::Button("Show")) s_obj->setDisplay(true);


			//Destroy button
			ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 10);
			style.Colors[ImGuiCol_Button] = ImVec4(1.f, 0.2f, 0.2f, 1.f);
			if (ImGui::Button("X###CloseTop", ImVec2(20, 0)))
			{
				style.Colors[ImGuiCol_Button] = col;
				s_obj->setSelected(false);
				s_obj->destroy();
				s_obj = nullptr;
				ImGui::End();
				return;
			}
			style.Colors[ImGuiCol_Button] = col;

			//Layers
			ImGui::BeginGroup();
			ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() - 105);
			if (ImGui::BeginCombo("layer###entLayerTop", s_obj->getLayer()->getLayerName().c_str(), ImGuiComboFlags_PopupAlignLeft))
			{
				for (auto layer : s_obj->getParentScene()->getLayerManager()->getLayers())
				{
					if (ImGui::Selectable(layer->getLayerName().c_str()))
					{
						s_obj->setLayer(layer);
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::EndGroup();

			// Save button
			ImGui::NewLine();
			ImGui::PushID("Save Entity");
			bool saved = s_obj->getSaved();
			if (ImGui::Checkbox("###saved1", &saved)) s_obj->setSaved(saved);
			ImGui::SameLine();
			ImGui::Text("Save Entity");
			ImGui::PopID();

			//Go to parent button
			Entity* parent = s_obj->getParentEntity();
			if (parent != nullptr)
			{
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 50);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.70f, 0.15f, 1.f));
				if (ImGui::Button("Parent", ImVec2(60, 0)))
				{
					if (s_obj != nullptr)s_obj->setSelected(false);
					s_name = parent->getName();
					s_obj = parent;
					s_obj->setSelected(true);
				}
				ImGui::PopStyleColor();
			}

			//dispaly all the components
			auto comps = s_obj->getAllComponents();
			int count = 0;
			for (auto it = comps.begin(); it != comps.end(); ++it)
			{
				ImGui::NewLine();
				ImGui::Text(Components::toString((*it)->getComponentType()).c_str());
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 10);
				if (detachButton((*it))) break;

				//Name and hide/show
				char compNameBuf[128];
				strcpy(compNameBuf, (*it)->getName().c_str());
				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth() - 65);
				if (ImGui::InputText(std::string("name###" + std::to_string(count)).c_str(), compNameBuf, IM_ARRAYSIZE(compNameBuf))) (*it)->setName(std::string(compNameBuf));
				ImGui::PopItemWidth();

				(*it)->displayImGui();
				count++;
			}

			//add new components
			ImGui::NewLine();
			if (ImGui::BeginCombo("###AddComponent", "Add Component", ImGuiComboFlags_PopupAlignLeft))
			{
				for (uint32_t i = 0; i < Components::getComponentNumber(); i++)
				{
					std::string str = Components::toString(ComponentType(i));
					if (ImGui::Selectable(str.c_str()))
					{
						ComponentType componentType = ComponentType(i);
						addComponent(componentType);
					}
				}

				ImGui::EndCombo();
			}
		}
		ImGui::End();
	}

	//! sceneInfo()
	void Inspector::sceneInfo()
	{
		ImGui::Begin("Scene Data", NULL);
		Scene* scene = SceneManager::getActiveScene();

		ImGui::Text("Name: %s", scene->getName().c_str());
		ImGui::Text("Folder: %s", scene->getFolderPath().c_str());
		ImGui::Text("Main Camera: %s", scene->getMainCamera()->getParent()->getName().c_str());


		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.4f, 1.0f), "Physics");
		bool pause = scene->getPhysicsPaused();
		if (ImGui::Checkbox("Pause Physics", &pause)) scene->setPhysicsPaused(pause);
		float scale = scene->getTimeScale();
		if (ImGui::DragFloat("Time Scale", &scale, 0.01f, 0.00001f, 999.f)) scene->setTimeScale(scale);

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Layers");
		ImGui::Text("Number of Layers: %i", scene->getLayerManager()->getLayers().size());
		ImGui::Text("(active)");
		ImGui::SameLine();
		ImGui::Text("(displayed)");
		for (auto layer : scene->getLayerManager()->getLayers())
		{
			ImGui::PushID(layer->getLayerName().c_str());
			bool active = layer->getActive();
			bool displayed = layer->getDisplayed();

			if (ImGui::Checkbox("###active1", &active)) layer->setActive(active);
			ImGui::SameLine(60);
			if (ImGui::Checkbox("###display1", &displayed)) layer-> setDisplayed(displayed);
			ImGui::SameLine();
			ImGui::Text(layer->getLayerName().c_str());
			ImGui::PopID();
		}

		ImGui::NewLine();
		ImGui::TextColored(ImVec4(0.4f, 0.6f, 1.0f, 1.0f), "Entities");
		ImGui::Text("Number of Entities: %i", scene->getEntities().size());
		ImGui::Text("Number of Render Passes: %i", scene->getRenderPasses().size());

		ImGui::End();
	}

	//! profiler()
	void Inspector::profiler()
	{
		ImGui::Begin("Profiler", NULL);

		ImGui::DragFloat("Timing Scale", &s_timingsScale, 0.0001f, 0.0001f, 10.0f, "%.4f");

		float fpsTime = TimerSystem::getStoredTime("FPS");

		s_timings->setValue(fpsTime);

		if(s_timings) ImGui::PlotHistogram("###Example Graph", s_timings->getValues() , s_timings->getCount(), 0, "Frame Time", 0.0f, s_timingsScale, ImVec2(ImGui::GetContentRegionAvailWidth(), 150), sizeof(float));
		ImGui::ProgressBar(TimerSystem::getStoredTime("onUpdateP") / fpsTime, ImVec2(-1, 0), "Update");
		ImGui::ProgressBar(TimerSystem::getStoredTime("onRenderP") / fpsTime, ImVec2(-1, 0), "Render");
		ImGui::ProgressBar(TimerSystem::getStoredTime("onInspectP") / fpsTime, ImVec2(-1, 0), "Inspect");
		
		ImGui::End();
	}

	void Inspector::search()
	{
		ImGui::Begin("Search", NULL);

		char buf[128];
		strcpy(buf, s_search.c_str());
		ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
		if (ImGui::InputText(std::string("###searchText00").c_str(), buf, IM_ARRAYSIZE(buf))) s_search = std::string(buf);
		ImGui::PopItemWidth();
		// Demonstrate keeping auto focus on the input box
		//if (ImGui::IsItemHovered() || (ImGui::IsItemFocused() && ImGui::IsItemActive() && ImGui::IsMouseClicked(0)))
			//ImGui::SetKeyboardFocusHere(); // Auto focus previous widget

		Scene* scene = SceneManager::getActiveScene();
		for (auto ent : scene->getEntities())
		{
			if (ent->getName().find(s_search) != std::string::npos)
			{
				if (ent->getSelected())
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.75f, 0.0f, 1.0f));
					if (ImGui::Button(ent->getName().c_str()))
					{
						if (s_obj != nullptr)s_obj->setSelected(false);
						s_name = ent->getName();
						s_obj = ent;
						s_obj->setSelected(true);
					}
					ImGui::PopStyleColor(3);
				}
				else
				{
					if (ImGui::Button(ent->getName().c_str()))
					{
						if (s_obj != nullptr)s_obj->setSelected(false);
						s_name = ent->getName();
						s_obj = ent;
						s_obj->setSelected(true);
					}
				}
			}
		}
		ImGui::End();
	}

	//! resourceProperties()
	void Inspector::resourceProperties()
	{
		ImGui::Begin("Properties", NULL, ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Type"))
		{
			if (ImGui::MenuItem("Materials")) s_resourceType = ResourceType::Material;
			if (ImGui::MenuItem("Models")) s_resourceType = ResourceType::Model3D;
			if (ImGui::MenuItem("SubTextures")) s_resourceType = ResourceType::SubTexture;
			if (ImGui::MenuItem("Textures")) s_resourceType = ResourceType::Texture;
			if (ImGui::MenuItem("Shaders")) s_resourceType = ResourceType::ShaderProgram;
			ImGui::MenuItem("");
			if (ImGui::MenuItem("Vertex Buffer")) s_resourceType = ResourceType::VertexBuffer;
			if (ImGui::MenuItem("Index Buffer")) s_resourceType = ResourceType::IndexBuffer;
			if (ImGui::MenuItem("Vertex Array")) s_resourceType = ResourceType::VertexArray;
			if (ImGui::MenuItem("Indirect Buffer")) s_resourceType = ResourceType::IndirectBuffer;
			if (ImGui::MenuItem("Uniform Buffer")) s_resourceType = ResourceType::UniformBuffer;
			if (ImGui::MenuItem("Frame Buffer")) s_resourceType = ResourceType::FrameBuffer;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) {}; //Save the current resource type
		}

		std::string typeName = toString(s_resourceType);
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - ImGui::CalcTextSize(typeName.c_str()).x);
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), typeName.c_str());
		ImGui::EndMenuBar();

		auto resources = ResourceManager::getResourcesOfType<Resource>(s_resourceType);

		for (auto resource : resources)
		{
			if (ImGui::CollapsingHeader(resource->getName().c_str()))
			{
				switch (s_resourceType)
				{
				case ResourceType::Material: 
				{
					Material* material = static_cast<Material*>(resource);
					ImGui::Text("Subtextures:");
					for (auto subTex : material->getSubTextures())
					{
						if (ImGui::TreeNode( std::string("Subtex###" + subTex->getName()).c_str(), subTex->getName().c_str()))
						{
							ImGui::Image((void*)(intptr_t)subTex->getBaseTexture()->getID(), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth() * (subTex->getHeightf() / subTex->getWidthf())), ImVec2(subTex->getUVStart().x, subTex->getUVStart().y), ImVec2(subTex->getUVEnd().x, subTex->getUVEnd().y), ImVec4(material->getTint().x, material->getTint().y, material->getTint().z, material->getTint().w));
							ImGui::TreePop();
						}
					}
					for (auto cubeMap : material->getCubemapTextures())
					{
						ImGui::Text("CubeMap: %s", cubeMap->getName().c_str());
					}
					ImGui::Text("Shader: %s", material->getShader()->getName().c_str());
					ImGui::ColorPicker4("tint", &material->getTint().x);
					float shine = material->getShininess();
					if (ImGui::DragFloat("shininess", &shine, 0.125f, 0.0f, 1024.0f, "%.3f", 2.0f)) material->setShininess(shine);
					break;
				}
				case ResourceType::Model3D:
				{
					Model3D* model = static_cast<Model3D*>(resource);
					for (auto mesh : model->getMeshes())
					{
						Geometry3D geometry = mesh.getGeometry();
						ImGui::Text("Geometry ID: %i", geometry.ID);
						ImGui::Text("First Index: %i", geometry.FirstIndex);
						ImGui::Text("Index Count: %i", geometry.IndexCount);
						ImGui::Text("First Vertex: %i", geometry.FirstVertex);

						Material* material = mesh.getMaterial();
						if (ImGui::BeginCombo(std::string("material###" + geometry.ID).c_str(), material->getName().c_str(), ImGuiComboFlags_PopupAlignLeft))
						{
							for (auto mat : ResourceManager::getResourcesOfType<Material>(ResourceType::Material))
							{
								if (ImGui::Selectable(mat->getName().c_str()))
								{
									mesh.setMaterial(mat);
								}
							}
							ImGui::EndCombo();
						}
					}
					break;
				}
				}
			}
		}
		if (ImGui::CollapsingHeader("Add new"))
		{
			//Add new type of resource
			ImGui::NewLine();
			char buf[256];
			strcpy(buf, s_new.c_str());
			if (ImGui::InputText(std::string("name###newName" + s_new).c_str(), buf, IM_ARRAYSIZE(buf))) s_new = std::string(buf);
			if (ImGui::IsItemHovered() || (ImGui::IsItemFocused() && ImGui::IsItemActive()))
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

			if (ImGui::Button("Create"))
			{
				//Make sure the name isnt taken
				while (ResourceManager::resourceExists(s_new)) 
				{
					NameUtils::rename(s_new);
				}

				Resource* newResource = nullptr;

				switch (s_resourceType)
				{
				case ResourceType::Material:
					//newResource = new Material(s_new, std::vector<SubTexture>())
					break;
				default:
					break;
				}

				ResourceManager::registerResource(s_new, newResource);

				s_new = "new" + toString(s_resourceType);
			}
		}
		else s_new = "new" + toString(s_resourceType);
		ImGui::End();
	}

	//! detachButton()
	/*!
	\param component an EntityComponent* - A pointer to the component to delete
	\return a bool - Success flag
	*/
	bool Inspector::detachButton(EntityComponent* component)
	{
		auto col = s_style->Colors[ImGuiCol_Button];
		s_style->Colors[ImGuiCol_Button] = ImVec4(1.f, 0.3f, 0.3f, 1.f);
		auto pad = s_style->FramePadding;
		s_style->FramePadding = ImVec2(0.f, 0.f);
		if (ImGui::Button(std::string("X###" + component->getName()).c_str(), ImVec2(20, 15)))
		{
			//s_obj->detach(type);
			component->detach();
			s_style->Colors[ImGuiCol_Button] = col;
			s_style->FramePadding = pad;
			return true;
		}
		s_style->Colors[ImGuiCol_Button] = col;
		s_style->FramePadding = pad;
		return false;
	}

	//! drawNestedEnt()
	/*!
	\param name a const std::string - The name of the entity
	\param ent an Entity* - A pointer to the entity
	\param offset a float - The offset
	\return a bool - Success flag
	*/
	bool Inspector::drawNestedEnt(const std::string name, Entity* ent, float offset)
	{
		ImGui::PushID(name.c_str());
		if (ent != nullptr)
		{
			int popAmt = 0;
			bool selected = ent->getSelected();
			if (selected && s_obj != ent)
			{
				ent->setSelected(false);
				selected = false;
			}
			std::string display = ent->getName();
			if (selected)
			{
				if (s_reParenting)
				{
					display = "Cancel move";
					ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(1.0f, 0.25f, 0.25f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
					popAmt += 3;
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(1.0f, 0.6f, 0.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(1.0f, 0.75f, 0.0f, 1.0f));
					popAmt += 3;
				}
			}
			else if (s_cantParent)
			{
				if (s_reParenting)
				{
					ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(0.9f, 0.7f, 0.7f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(0.9f, 0.75f, 0.75f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(0.9f, 0.8f, 0.8f, 1.0f));
					popAmt += 3;
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(0.7f, 0.65f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(0.75f, 0.75f, 0.25f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(0.8f, 0.85f, 0.3f, 1.0f));
					popAmt += 3;
				}
			}

			if (s_reParenting && s_cantParent && !selected)
			{
				ImGui::TextColored(ImVec4(0.95f, 0.90f, 0.15f, 1.f), "<");
				ImGui::SameLine();
			}

			if (ImGui::Button(display.c_str())) {
				if (s_reParenting) {
					if (selected)
					{
						//We are on original item, cancel operation
						s_reParenting = false;
						s_cantParent = false;
					}
					else if (s_cantParent)
					{
						//We're on a child, send them upwards
						s_reParenting = false;
						s_cantParent = false;

						//Set obj to be selected
						if (s_obj != nullptr)s_obj->setSelected(false);
						s_name = ent->getName();
						s_obj = ent;
						s_obj->setSelected(true);

						//Add ent to the gradparent (or scene)
						Entity* grandParent = ent->getParentEntity()->getParentEntity();
						if (grandParent == nullptr) ent->getParentScene()->addEntity(ent->getName(), ent);
						else grandParent->addChildEntity(ent->getName(), ent);

						ImGui::PopStyleColor(popAmt);
						ImGui::PopID();
						return true;
					}
					else
					{
						//It's possible so we do reparent
						s_reParenting = false;
						s_cantParent = false;
						ent->addChildEntity(s_obj->getName(), s_obj);
						ImGui::PopStyleColor(popAmt);
						ImGui::PopID();
						return true;
					}
				}
				else {
					if (selected) 
					{
						s_reParenting = true;
					}
					else {
						if (s_obj != nullptr)s_obj->setSelected(false);
						s_name = ent->getName();
						s_obj = ent;
						s_obj->setSelected(true);
					}
				}
			}

			if (s_reParenting && !s_cantParent && !selected)
			{
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.1f, 0.70f, 0.15f, 1.f), "<");
			}
			
			//Remove styling for next buttons
			ImGui::PopStyleColor(popAmt);
			
			//Indent children
			ImGui::Indent(offset);
			if (selected) s_cantParent = true;
			if (ent->getChildEntities().size() > 0)
			{
				for (auto child : ent->getChildEntities()) 
				{ 
					if (drawNestedEnt(child.first, child.second, offset))
					{
						ImGui::PopID();
						return true;
					}
				}
			}
			ImGui::Unindent(offset);
			if (selected) s_cantParent = false;

			if (s_obj == ent)
			{
				if (ImGui::BeginPopupContextWindow())
				{

					if (ImGui::MenuItem("Add Entity"))
					{
						addEntity();
					}
					if (ImGui::MenuItem("Remove Entity"))
					{
						s_obj->setSelected(false);
						s_obj->destroy();
						s_obj = nullptr;
						ent = nullptr;
					}
					if (ImGui::BeginMenu("Add Component"))
					{
						for (uint32_t i = 0; i < Components::getComponentNumber(); i++)
						{
							std::string str = Components::toString(ComponentType(i));
							if (ImGui::MenuItem(str.c_str()))
							{
								ComponentType componentType = ComponentType(i);
								if (s_obj) addComponent(componentType);
							}
						}
						ImGui::EndMenu();
					}
					ImGui::EndPopup();
				}
			}
			else if (s_obj == nullptr)
			{
				if (ImGui::BeginPopupContextWindow())
				{

					if (ImGui::MenuItem("Add Entity"))
					{
						addEntity();
					}
					ImGui::EndPopup();
				}
			}
		}
		else
		{
			//Draw as grey imgui unclickable button
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
			//Draw as default style button
			if (ImGui::Button(name.c_str()))
			{
				if (s_obj != nullptr)s_obj->setSelected(false);
				s_obj = ent;
				s_name = "";
				ENGINE_ERROR("[Inspector::Heirarchy] Scene has null entities in its parent child hierarchy!");
			}
			//Remove styling for next buttons
			ImGui::PopStyleColor(3);
		}
		ImGui::PopID();
		return false;
	}

	//! Inspector()
	Inspector::Inspector() : System(SystemType::Inspector)
	{
	}

	//! ~Inspector()
	Inspector::~Inspector()
	{
	}

	//! start()
	void Inspector::start()
	{
		// Start system if its disabled
		if (!s_enabled)
		{
			ENGINE_INFO("[Inspector::start] Starting the inspector system.");
			s_obj = nullptr;
			s_name = "";
			s_enabled = true;
			Inspector::initialise();
			s_timings = new Timings;
		}
	}

	//! stop()
	void Inspector::stop()
	{
		// Stop system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[Inspector::stop] Stopping the inspector system.");
			ImGuiHelper::shutdown();
			s_enabled = false;
			delete s_timings;
		}
	}

	//! initialise()
	void Inspector::initialise()
	{
		ImGuiHelper::init();
		s_style = &ImGui::GetStyle();
		s_style->WindowRounding = 0.3f;
		s_style->FrameRounding = 1.3f;
		//style.WindowPadding = ImVec2(15, 15);
	}

	//! inspect()
	void Inspector::inspect()
	{
		ImGuiHelper::begin();

		//ImGui::ShowDemoWindow();
		menuBar();
		sceneView();
		search();
		heirarchy();
		resourceProperties();
		selectedEnt();
		sceneInfo();
		profiler();
		ImGuiHelper::end();
	}

	//! getViewPortSize()
	/*!
	\return a glm::vec2 - The viewport size
	*/
	glm::vec2 Inspector::getViewPortSize()
	{
		return s_viewPortSize;
	}

	//! getViewPortChanged()
	/*!
	\param reset a bool - Reset the viewport
	\return a bool - Has the viewport changed
	*/
	bool Inspector::getViewPortChanged(bool reset)
	{
		if (s_viewPortChanged)
		{
			if (reset) s_viewPortChanged = false;
			return true;
		}
		else
		{
			return false;
		}
		
	}

	//! getSelectedEntity()
	/*!
	\return an Entity* - A pointer to the selected entity in the inspector
	*/
	Entity * Inspector::getSelectedEntity()
	{
		return s_obj;
	}

	//! deselectEntity
	void Inspector::deselectEntity()
	{
		s_obj = nullptr;
	}

	void Inspector::addComponent(ComponentType type)
	{
		std::string compName = "Component ";
		compName += std::to_string(s_obj->getAllComponents().size());
		switch (type)
		{
		case ComponentType::Camera:
		{
			CameraData camData({ 0, 0, -1 }, { 0, 1, 0 }, { 0, 1, 0 }, 0, 0, 45);
			s_obj->attach<Camera>(compName.c_str(), camData);
			s_obj->getComponent<Camera>()->setClearColour({ 0, 0 , 0 , 0 });
			break;
		}
		case ComponentType::Transform:
		{
			s_obj->attach<Transform>(compName.c_str(), 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f);
			break;
		}
		case ComponentType::Text:
		{
			glm::vec4 tint = { 1.f, 1.f, 1.f, 1.f };
			s_obj->attach<Text>(compName.c_str(), "text", tint, "Arial");
			break;
		}
		case ComponentType::CharacterController:
		{
			s_obj->attach<CharacterController>(compName.c_str(), 5.f, 0.15f, false);
			break;
		}
		case ComponentType::MeshRender3D:
		{
			// Add mesh render component
			// [LocalPosition, LocalRotation, LocalScale, Model3D, Material]
			s_obj->attach<MeshRender3D>(compName.c_str(),
				ResourceManager::getResourcesOfType<Model3D>(ResourceType::Model3D).at(0),
				ResourceManager::getResource<Material>("defaultMaterial3D")
				);

			break;
		}
		case ComponentType::MeshRender2D:
		{
			// Add mesh render component
			// [LocalPosition, LocalRotation, LocalScale, Material]
			s_obj->attach<MeshRender2D>(compName.c_str(),
				ResourceManager::getResource<Material>("defaultMaterial2D")
				);

			break;
		}
		case ComponentType::RigidBody:
		{
			// Add rigidbody component
			// [rigidBodyType]
			s_obj->attach<RigidBody>(compName.c_str(),
				s_obj->getParentScene()->getPhysicsWorld(),
				BodyTypes::RigidBodyType::KINEMATIC
				);
			break;
		}
		case ComponentType::BoxCollider:
		{
			// Add box collider component
			// [halfExtents, localPosition, trigger]
			s_obj->attach<BoxCollider>(compName.c_str(),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f)
				);
			break;
		}
		case ComponentType::PointLight:
		{
			s_obj->attach<PointLight>(compName.c_str(),
				glm::vec3(1.f, 1.f, 1.f),
				glm::vec3(1.f, 1.f, 1.f),
				glm::vec3(1.f, 1.f, 1.f),
				1.f, 0.5f, 0.1f);
			break;
		}
		case ComponentType::SpotLight:
		{
			s_obj->attach<SpotLight>(compName.c_str(),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec3(1.f, 1.f, 1.f),
				glm::vec3(1.f, 1.f, 1.f),
				glm::vec3(1.f, 1.f, 1.f),
				1.f, 1.f, 1.f, 0.5f, 0.1f);
			break;
		}
		case ComponentType::DirectionalLight:
		{
			s_obj->attach<DirectionalLight>(compName.c_str(),
				glm::vec3(0.f, -1.f, 0.f),
				glm::vec3(1.f, 1.f, 1.f),
				glm::vec3(1.f, 1.f, 1.f),
				glm::vec3(1.f, 1.f, 1.f));
			break;
		}
		case ComponentType::Canvas:
		{
			s_obj->attach<UI::Canvas>(compName.c_str());
			break;
		}
		case ComponentType::Element:
		{
			s_obj->attach<UI::Element>(compName.c_str());
			break;
		}
		case ComponentType::NativeScript:
		{
			s_obj->attach<NativeScript>(compName.c_str());
			break;
		}
		case ComponentType::AudioListener:
		{
			s_obj->attach<AudioListener>(compName.c_str(), glm::vec3(0.f, 0.f, 0.f));
			break;
		}
		case ComponentType::StudioSound:
		{
			s_obj->attach<StudioSound>(compName.c_str(), "Event1", glm::vec3(0.f, 0.f, 0.f));
			break;
		}
		case ComponentType::Sound:
		{
			s_obj->attach<Sound>(compName.c_str(), "Sound1", false);
			break;
		}
		}
	}

	void Inspector::addEntity()
	{
		Entity* ent = new Entity;
		Scene* scene = SceneManager::getActiveScene();
		std::string name = "New Entity";

		if (s_obj == nullptr) {
			while (scene->checkRootEntityNameTaken(name.c_str()))
			{
				name = NameUtils::rename(name);
			}
			scene->addEntity(name, ent);
		}
		else
		{
			while (s_obj->checkChildEntityNameTaken(name.c_str()))
			{
				name = NameUtils::rename(name);
			}
			s_obj->addChildEntity(name, ent);
		}


		// Set the entity's layer
		ent->setLayer(scene->getLayerManager()->getLayer("Default"));
		ent->setDisplay(true);

		ent->attach<Transform>("Transform", 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f);

		if (s_obj != nullptr)s_obj->setSelected(false);
		s_name = ent->getName();
		s_obj = ent;
		s_obj->setSelected(true);
	}

}
#endif // !NG_DEPLOY