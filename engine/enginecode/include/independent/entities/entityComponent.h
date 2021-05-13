/*! \file entityComponent.h
*
* \brief An entity component which describes one particular feature of an entity. Follows the ECS architectural pattern
*
* \author Daniel Bullin
*
*/
#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "independent/core/common.h"
#include "independent/systems/systems/resourceManager.h"

namespace OwlEngine
{
	class Entity; //!< Forward declare entity

	/*! \enum ComponentType
	* \brief The different types of entity components
	*/
	enum class ComponentType
	{
		None = 0,				 //!< No type
		Camera = 1,				 //!< A camera in the world
		Transform = 2,			 //!< A 3D transform of an entity
		MeshRender3D = 3,		 //!< The render details for a 3D entity
		MeshRender2D = 4,		 //!< The render details for a 2D entity
		Text = 5,				 //!< A text render component
		CharacterController = 6, //!< A character controller
		RigidBody = 7,			 //!< A physics rigidbody
		BoxCollider = 8,	     //!< A box collider
		DirectionalLight = 9,	 //!< A directional light source
		PointLight = 10,		 //!< A point light source
		SpotLight = 11,			 //!< A spot light source
		NativeScript = 12,		 //!< A native script
		SphereCollider = 13,	 //!< A sphere collider
		CapsuleCollider = 14,	 //!< A capsule collider
		AudioListener = 15,		 //!< An audio listener
		StudioSound = 16,		 //!< A FMOD studio sound
		Canvas = 17,			 //!< Holds the UI elements
		Element = 18,			 //!< Exists on the canvas
		Sound = 19,				 //!< A sound component
		AIMovementNode = 20		 //!< A movement node
	};

	/*! \class EntityComponent
	* \brief An entity component which are attached to entities
	*/
	class EntityComponent
	{
	private:
		EntityComponent(const EntityComponent& other) {} //!< The copy constructor disabled
		void operator=(const EntityComponent& other) {} //!< The assignment operator disabled
	protected:
		std::string m_name; //!< The name of the component
		ComponentType m_componentType; //!< The component type
		Entity* m_parentEntity; //!< The entity this component is attached to
		bool m_changed = false; //!< Has this component been updated
	public:
		EntityComponent(const ComponentType type); //!< Constructor requiring component type
		virtual ~EntityComponent(); //!< Destructor

		void detach(); //!< Detach this component from its parent

		void setParent(Entity* parent); //!< Set the component's parent
		Entity* getParent(); //!< Get the entity this component is attached to

		void setName(const std::string& name); //!< Set the name of the component
		const std::string& getName() const; //!< Get the name of the component

		const ComponentType& getComponentType() const; //!< Get the component type

		void setChanged(const bool change);//!< Allows other things that have the component to let it know its been touched and might need to regenerate certain values
		const bool getChanged(); //!< Get whether this component has been externally changed

		virtual void onAttach() = 0; //!< Occurs when component is attached to an entity
		virtual void onDetach() = 0; //!< Occurs when component is detached from an entity
		virtual void onUpdate(const float timestep, const float totalTime) = 0; //!< Update function for components
			/*!< \param timestep a const float - The timestep
				 \param totalTime a const float - The total runtime of the application */

		virtual nlohmann::ordered_json getAsJSON() = 0; //!< Get the component details as a json object
		virtual void printComponentDetails() = 0; //!< Print component details
#ifndef NG_DEPLOY
		virtual void displayImGui() = 0; //!< The ImGUI display controls for this component
#endif // NG_DEPLOY
	};

	namespace Components
	{
		//! getComponentNumber()
		/*!
		\return an uint32_t - The number of types of components
		*/
		static uint32_t getComponentNumber()
		{
			return 21;
		}

		//! toString()
		/*
		\param type a ComponentType - The component type
		\return a const std::string - The component type as a string
		*/
		static const std::string toString(ComponentType type)
		{
			switch (type)
			{
			case ComponentType::None: return "None";
			case ComponentType::Camera: return "Camera";
			case ComponentType::Transform: return "Transform";
			case ComponentType::MeshRender3D: return "MeshRender3D";
			case ComponentType::MeshRender2D: return "MeshRender2D";
			case ComponentType::Text: return "Text";
			case ComponentType::CharacterController: return "CharacterController";
			case ComponentType::RigidBody: return "RigidBody";
			case ComponentType::BoxCollider: return "BoxCollider";
			case ComponentType::DirectionalLight: return "DirectionalLight";
			case ComponentType::PointLight: return "PointLight";
			case ComponentType::SpotLight: return "SpotLight";
			case ComponentType::NativeScript: return "NativeScript";
			case ComponentType::SphereCollider: return "SphereCollider";
			case ComponentType::CapsuleCollider: return "CapsuleCollider";
			case ComponentType::AudioListener: return "AudioListener";
			case ComponentType::StudioSound: return "StudioSound";
			case ComponentType::Canvas: return "Canvas";
			case ComponentType::Element: return "Element";
			case ComponentType::Sound: return "Sound";
			case ComponentType::AIMovementNode: return "AIMovementNode";
			default: return "Not a valid component type.";
			}
		}

		//! toType()
		/*!
		\param classType a const std::string& - The class type
		\return a const ComponentType - The type of component
		*/
		static const ComponentType toType(const std::string& classType)
		{
			// Class types should be in the format [class OwlEngine::{CLASS_NAME}]
			if (classType == "class OwlEngine::Camera" || classType == "Camera")
				return ComponentType::Camera;
			else if (classType == "class OwlEngine::Transform" || classType == "Transform")
				return ComponentType::Transform;
			else if (classType == "class OwlEngine::MeshRender3D" || classType == "MeshRender3D")
				return ComponentType::MeshRender3D;
			else if (classType == "class OwlEngine::MeshRender2D" || classType == "MeshRender2D")
				return ComponentType::MeshRender2D;
			else if (classType == "class OwlEngine::Text" || classType == "Text")
				return ComponentType::Text;
			else if (classType == "class OwlEngine::CharacterController" || classType == "CharacterController")
				return ComponentType::CharacterController;
			else if (classType == "class OwlEngine::PointLight" || classType == "PointLight")
				return ComponentType::PointLight;
			else if (classType == "class OwlEngine::SpotLight" || classType == "SpotLight")
				return ComponentType::SpotLight;
			else if (classType == "class OwlEngine::DirectionalLight" || classType == "DirectionalLight")
				return ComponentType::DirectionalLight;
			else if (classType == "class OwlEngine::RigidBody" || classType == "RigidBody")
				return ComponentType::RigidBody;
			else if (classType == "class OwlEngine::BoxCollider" || classType == "BoxCollider")
				return ComponentType::BoxCollider;
			else if (classType == "class OwlEngine::NativeScript" || classType == "NativeScript")
				return ComponentType::NativeScript;
			else if (classType == "class OwlEngine::SphereCollider" || classType == "SphereCollider")
				return ComponentType::SphereCollider;
			else if (classType == "class OwlEngine::CapsuleCollider" || classType == "CapsuleCollider")
				return ComponentType::CapsuleCollider;
			else if (classType == "class OwlEngine::AudioListener" || classType == "AudioListener")
				return ComponentType::AudioListener;
			else if (classType == "class OwlEngine::StudioSound" || classType == "StudioSound")
				return ComponentType::StudioSound;
			else if (classType == "class OwlEngine::Canvas" || classType == "Canvas")
				return ComponentType::Canvas;
			else if (classType == "class OwlEngine::Element" || classType == "Element")
				return ComponentType::Element;
			else if (classType == "class OwlEngine::Sound" || classType == "Sound")
				return ComponentType::Sound;
			else if (classType == "class OwlEngine::AIMovementNode" || classType == "AIMovementNode")
				return ComponentType::AIMovementNode;
			else
				return ComponentType::None;
		}

		//! getComponentDependencies()
		/*!
		\param component a const ComponentType - The type of component to check for dependencies
		\return a std::vector<ComponentType> - A vector of component types the component given depends on
		*/
		static std::vector<ComponentType> getComponentDependencies(const ComponentType component)
		{
			std::vector<ComponentType> components;

			switch (component)
			{
				case ComponentType::Camera:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::Transform:
				{
					components.push_back(ComponentType::None);
					break;
				}
				case ComponentType::MeshRender3D:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::MeshRender2D:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::Text:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::CharacterController:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::RigidBody:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::BoxCollider:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::DirectionalLight:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::PointLight:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::SpotLight:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::NativeScript:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::SphereCollider:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::CapsuleCollider:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::AudioListener:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::StudioSound:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::Canvas:
				{
					components.push_back(ComponentType::Canvas);
					break;
				}
				case ComponentType::Element:
				{
					components.push_back(ComponentType::Element);
					break;
				}
				case ComponentType::Sound:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
				case ComponentType::AIMovementNode:
				{
					components.push_back(ComponentType::Transform);
					break;
				}
			}

			return components;
		}
	}
}
#endif