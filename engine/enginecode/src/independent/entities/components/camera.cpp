/*! \file camera.cpp
*
* \brief A camera object in the game world
*
* \author Daniel Bullin and www.learnOpenGL.com
*
*/
#include "engine_pch.h"
#include "independent/entities/components/camera.h"
#include "independent/entities/entity.h"
#include "independent/systems/systems/windowManager.h"
#include "independent/systems/systems/log.h"
#include "independent/utils/mathUtils.h"
#include "independent/systems/components/scene.h"

#ifndef NG_DEPLOY
#include "platform/ImGui/ImGuiHelper.h"
#endif

namespace OwlEngine
{
	//! Camera()
	/*!
	\param cameraData a const CameraData& - The camera's data
	*/
	Camera::Camera(const CameraData& cameraData) : EntityComponent(ComponentType::Camera), m_cameraData(cameraData)
	{
		// Update all the applicable camera vectors
		m_mainCamera = false;
		m_skybox = nullptr;
		updateCameraVectors();
	}

	//! ~Camera()
	Camera::~Camera()
	{
		// Skybox uses raw pointers, so delete it
		if(m_skybox)
			delete m_skybox;

		m_skybox = nullptr;
	}

	//! onAttach()
	void Camera::onAttach()
	{
	}

	//! onDetach
	void Camera::onDetach()
	{
	}

	//! getClearColour()
	/*!
	\return a const glm::vec4& - The clear colour
	*/
	const glm::vec4& Camera::getClearColour()
	{
		return m_clearColour;
	}

	//! setClearColour()
	/*!
	\param colour a const glm::vec4& - The clear colour
	*/
	void Camera::setClearColour(const glm::vec4& colour)
	{
		if (colour.r >= 0.f && colour.g >= 0.f && colour.b >= 0.f && colour.a >= 0.f)
			m_clearColour = colour;
		else
			ENGINE_ERROR("Camera::setClearColour] An invalid colour was provided. Cannot set.");
	}

	//! getViewMatrix()
	/*!
	\param perspective a const bool - Use the perspective projection, false for orthographic
	\return a glm::mat4 - The view matrix
	*/
	glm::mat4 Camera::getViewMatrix(const bool perspective)
	{
		Entity* parent = getParent();

		if (parent)
		{
			Transform* trans = parent->getComponent<Transform>();
			if (trans)
			{
				if (perspective)
					return glm::lookAt(trans->getPosition(), trans->getPosition() + m_cameraData.Front, m_cameraData.Up);
				else
					return glm::mat4(1.f);
			}
			else
				ENGINE_ERROR("[Camera::getViewMatrix] The entity this camera is attached to does not have a transform. Entity Name: {0}.", parent->getName());
		}
		else
			ENGINE_ERROR("[Camera::getViewMatrix] The entity the camera is attached to is invalid. Camera Name: {0}.", m_name);
		return glm::mat4(1.f);
	}

	//! getProjectionMatrix()
	/*!
	\param perspective a const bool - Use the perspective projection, false for orthographic
	\return a glm::mat4 - The projection matrix
	*/
	glm::mat4 Camera::getProjectionMatrix(const bool perspective)
	{
		if (perspective)
			return glm::perspective(glm::radians(m_cameraData.Zoom), m_projection.AspectRatio, m_projection.NearPlane, m_projection.FarPlane);
		else
			return glm::ortho(0.f, m_projection.Right, m_projection.Bottom, 0.f, -1.f, 1.f);
	}

	//! getWorldPosition()
	/*!
	\return a const glm::vec3 - The world position of the camera
	*/
	glm::vec3 Camera::getWorldPosition()
	{
		Entity* parent = getParent();

		if (parent)
		{
			Transform* trans = parent->getComponent<Transform>();
			if (trans)
				return trans->getPosition();
			else
				ENGINE_ERROR("[Camera::getWorldPosition] The entity the camera is attached to does not have a transform. Entity Name: {0}.", parent->getName());
		}
		else
			ENGINE_ERROR("[Camera::getWorldPosition] The entity the camera is attached to is invalid. Camera Name: {0}.", m_name);
		return glm::vec3(0.f, 0.f, 0.f);
	}

	//! getFrontWorldPosition()
	/*!
	\param frontMultiplied a const float - The multiplier for the front vector
	\return a glm::vec3 - The world position along the camera's front vector
	*/
	glm::vec3 Camera::getFrontWorldPosition(const float frontMultiplied)
	{
		return getParent()->getParentScene()->getMainCamera()->getWorldPosition() + (getParent()->getParentScene()->getMainCamera()->getCameraData().Front * frontMultiplied);
	}

	//! setProjection()
	/*!
	\param projection a const Projection& - The new projection data
	*/
	void Camera::setProjection(const Projection& projection)
	{
		m_projection = projection;
	}

	//! getProjection()
	/*!
	\return a Projection& - The projection data
	*/
	Projection& Camera::getProjection()
	{
		return m_projection;
	}

	//! setCameraData()
	/*!
	\param data a const CameraData& - The new camera data
	*/
	void Camera::setCameraData(const CameraData& data)
	{
		m_cameraData = data;
	}

	//! getCameraData()
	/*!
	\return a CameraData& - The camera data
	*/
	CameraData& Camera::getCameraData()
	{
		return m_cameraData;
	}

	//! setMainCamera()
	/*!
	\param mainCamera a const bool - Is this the main camera
	*/
	void Camera::setMainCamera(const bool mainCamera)
	{
		m_mainCamera = mainCamera;
	}

	//! getMainCamera()
	/*!
	\return a const bool - Is this the main camera
	*/
	const bool Camera::getMainCamera()
	{
		return m_mainCamera;
	}

	//! setSkybox()
	/*!
	\param skybox a Skybox* - Pointer to attatched skybox
	*/
	void Camera::setSkybox(Skybox* skybox)
	{
		Entity* parent = getParent();

		if (skybox)
			m_skybox = skybox;
		else
		{
			if (parent)
				ENGINE_ERROR("[Camera::setSkybox] An invalid skybox was provided. Entity Name: {0}", parent->getName());
			else
				ENGINE_ERROR("[Camera::setSkybox] An invalid skybox was provided. No entity attached.");
		}
	}
	
	//! getSkybox()
	/*!
	\return a Skybox* - Pointer to the attached skybox
	*/
	Skybox* Camera::getSkybox()
	{
		return m_skybox;
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void Camera::onUpdate(const float timestep, const float totalTime)
	{
		if (m_changed)
		{
			updateCameraVectors();
			m_changed = false;
		}
	}

	//! printComponentDetails()
	void Camera::printComponentDetails()
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[Camera::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		glm::mat4 perpViewMatrix = getViewMatrix(true);
		glm::mat4 perpProjMatrix = getProjectionMatrix(true);
		glm::mat4 orthoViewMatrix = getViewMatrix(false);
		glm::mat4 orthoProjectionMatrix = getProjectionMatrix(false);
		auto dArray = MathUtils::convertMat4ToArray(perpViewMatrix);
		auto dArray1 = MathUtils::convertMat4ToArray(perpProjMatrix);
		auto dArray2 = MathUtils::convertMat4ToArray(orthoViewMatrix);
		auto dArray3 = MathUtils::convertMat4ToArray(orthoProjectionMatrix);

		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("Camera Details for Entity: {0}", parent->getName());
		ENGINE_TRACE("==========================================");
		ENGINE_TRACE("World Position: {0}, {1}, {2}.", getWorldPosition().x, getWorldPosition().y, getWorldPosition().z);
		ENGINE_TRACE("Front: {0}, {1}, {2}.", m_cameraData.Front.x, m_cameraData.Front.y, m_cameraData.Front.z);
		ENGINE_TRACE("Yaw: {0}.", m_cameraData.Yaw);
		ENGINE_TRACE("Pitch: {0}.", m_cameraData.Pitch);
		ENGINE_TRACE("Zoom: {0}.", m_cameraData.Zoom);
		ENGINE_TRACE("Main Camera: {0}.", m_mainCamera);
		ENGINE_TRACE("Clear Colour: {0}, {1}, {2}, {3}.", m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);
		
		if (m_skybox)
		{
			Material* mat = m_skybox->getMaterial();
			Model3D* model = m_skybox->getModel();

			if(mat && model)
				ENGINE_TRACE("Skybox: Model: {0}, Material: {1}.", model->getName(), mat->getName());
			else
				ENGINE_ERROR("[Camera::printComponentDetails] This skybox has an invalid material or model. Entity Name: {0}.", parent->getName());
		}
		else
			ENGINE_TRACE("Skybox: No Skybox attached.");

		ENGINE_TRACE("Perspective View Matrix: {0}, {1}, {2}, {3}", dArray[0], dArray[1], dArray[2], dArray[3]);
		ENGINE_TRACE("Perspective View Matrix: {0}, {1}, {2}, {3}", dArray[4], dArray[5], dArray[6], dArray[7]);
		ENGINE_TRACE("Perspective View Matrix: {0}, {1}, {2}, {3}", dArray[8], dArray[9], dArray[10], dArray[11]);
		ENGINE_TRACE("Perspective View Matrix: {0}, {1}, {2}, {3}", dArray[12], dArray[13], dArray[14], dArray[15]);
		ENGINE_TRACE("Perspective Projection Matrix: {0}, {1}, {2}, {3}", dArray1[0], dArray1[1], dArray1[2], dArray1[3]);
		ENGINE_TRACE("Perspective Projection Matrix: {0}, {1}, {2}, {3}", dArray1[4], dArray1[5], dArray1[6], dArray1[7]);
		ENGINE_TRACE("Perspective Projection Matrix: {0}, {1}, {2}, {3}", dArray1[8], dArray1[9], dArray1[10], dArray1[11]);
		ENGINE_TRACE("Perspective Projection Matrix: {0}, {1}, {2}, {3}", dArray1[12], dArray1[13], dArray1[14], dArray1[15]);
		ENGINE_TRACE("Ortho View Matrix: {0}, {1}, {2}, {3}", dArray2[0], dArray2[1], dArray2[2], dArray2[3]);
		ENGINE_TRACE("Ortho View Matrix: {0}, {1}, {2}, {3}", dArray2[4], dArray2[5], dArray2[6], dArray2[7]);
		ENGINE_TRACE("Ortho View Matrix: {0}, {1}, {2}, {3}", dArray2[8], dArray2[9], dArray2[10], dArray2[11]);
		ENGINE_TRACE("Ortho View Matrix: {0}, {1}, {2}, {3}", dArray2[12], dArray2[13], dArray2[14], dArray2[15]);
		ENGINE_TRACE("Ortho Projection Matrix: {0}, {1}, {2}, {3}", dArray3[0], dArray3[1], dArray3[2], dArray3[3]);
		ENGINE_TRACE("Ortho Projection Matrix: {0}, {1}, {2}, {3}", dArray3[4], dArray3[5], dArray3[6], dArray3[7]);
		ENGINE_TRACE("Ortho Projection Matrix: {0}, {1}, {2}, {3}", dArray3[8], dArray3[9], dArray3[10], dArray3[11]);
		ENGINE_TRACE("Ortho Projection Matrix: {0}, {1}, {2}, {3}", dArray3[12], dArray3[13], dArray3[14], dArray3[15]);
		ENGINE_TRACE("==========================================");
	}

	//! getAsJSON()
	/*!
	\return a nlohmann::ordered_json - The component's data in JSON format
	*/
	nlohmann::ordered_json Camera::getAsJSON()
	{
		nlohmann::ordered_json data;
		data["name"] = m_name;
		data["type"] = Components::toString(m_componentType);
		if(m_skybox) data["skybox"][0]["model"] = m_skybox->getModel()->getName();
		if(m_skybox) data["skybox"][0]["material"] = m_skybox->getMaterial()->getName();
		data["clearColour"] = { m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a };
		data["front"] = { m_cameraData.Front.x, m_cameraData.Front.y, m_cameraData.Front.z };
		data["up"] = { m_cameraData.Up.x, m_cameraData.Up.y, m_cameraData.Up.z };
		data["worldUp"] = { m_cameraData.WorldUp.x, m_cameraData.WorldUp.y, m_cameraData.WorldUp.z };
		data["Yaw"] =  m_cameraData.Yaw;
		data["Pitch"] = m_cameraData.Pitch;
		data["Zoom"] = m_cameraData.Zoom;
		data["setMainCamera"] = m_mainCamera;
		return data;
	}

#ifndef NG_DEPLOY
	//! displayImGui()
	void Camera::displayImGui()
	{
		Entity* parent = getParent();

		if (parent)
		{
			ImGui::DragFloat4("clear colour", &m_clearColour.r, 0.005f, 0.f, 1.f);
			if (m_skybox)
			{
				if (m_skybox->getModel())
					ImGui::Text(std::string("Skybox model: " + m_skybox->getModel()->getName()).c_str());
				else
					ENGINE_ERROR("[Camera::displayImGui] Cannot detect the model for the skybox.");
			}
			if (m_skybox)
			{
				if (m_skybox->getMaterial())
					ImGui::Text(std::string("Skybox material: " + m_skybox->getMaterial()->getName()).c_str());
				else
					ENGINE_ERROR("[Camera::displayImGui] Cannot detect the material for the skybox.");
			}
			if (
				ImGui::DragFloat("zoom", &m_cameraData.Zoom, 0.1f) ||
				ImGui::DragFloat("pitch", &m_cameraData.Pitch, 1.f) ||
				ImGui::DragFloat("yaw", &m_cameraData.Yaw, 1.f)
				) setChanged(true);
			if (ImGui::Checkbox("Main Camera", &m_mainCamera) && m_mainCamera)
			{
				Scene* scene = parent->getParentScene();
				if(scene)
					parent->getParentScene()->setMainCamera(this);
				else
					ENGINE_ERROR("[Camera::displayImGui] The entity this component is attached to does not belong to a valid scene. Entity Name: {0}.", parent->getName());
			}
		}
		else
			ENGINE_ERROR("[Camera::displayImGui] This component does not have a valid parent entity. Component Name: {0}.", m_name);
	}
#endif // NG_DEPLOY

	//! updateCameraVectors()
	void Camera::updateCameraVectors()
	{
		// Recalculate all camera vectors
		glm::vec3 front;
		front.x = cos(glm::radians(m_cameraData.Yaw)) * cos(glm::radians(m_cameraData.Pitch));
		front.y = sin(glm::radians(m_cameraData.Pitch));
		front.z = sin(glm::radians(m_cameraData.Yaw)) * cos(glm::radians(m_cameraData.Pitch));
		m_cameraData.Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_cameraData.Right = glm::normalize(glm::cross(m_cameraData.Front, m_cameraData.WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_cameraData.Up = glm::normalize(glm::cross(m_cameraData.Right, m_cameraData.Front));

		if (m_parentEntity != nullptr)
		{
			Transform* trans = m_parentEntity->getComponent<Transform>();
			if (trans != nullptr)
			{
				glm::vec3 rot = trans->getOrientation();
				rot = glm::vec3(rot.x, -m_cameraData.Yaw, rot.z);
				trans->setOrientation(rot);
			}
		}
	}

	//! updateProjection()
	void Camera::updateProjection()
	{
		glm::ivec2 size = WindowManager::getFocusedWindow()->getProperties().getSize();
		m_projection.Right = static_cast<float>(size.x);
		m_projection.Bottom = static_cast<float>(size.y);
		m_projection.AspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
	}

	//! updateProjection()
	/*!
	\param size a glm::ivec2 - The new viewport size
	*/
	void Camera::updateProjection(glm::ivec2 size)
	{
		m_projection.Right = static_cast<float>(size.x);
		m_projection.Bottom = static_cast<float>(size.y);
		m_projection.AspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);
	}
}