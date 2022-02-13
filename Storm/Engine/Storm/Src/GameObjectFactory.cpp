#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "GameObject.h"

#include "AnimatorComponent.h"
#include "ColliderComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraControllerComponent.h"

#include<rapidjson/document.h>
#include<rapidjson/filereadstream.h>

using namespace Storm;
namespace rj = rapidjson;

void GameObjectFactory::Make(std::filesystem::path templateFile, GameObject& gameObject)
{
	//auto transformComponent = gameObject.AddComponent<TransformComponent>();
	//transformComponent->position = { 5.0f,1.0f,2.0f };

	//auto colliderComponent = gameObject.AddComponent<ColliderComponent>();
	//colliderComponent->Set
	FILE* file = nullptr;
	auto err = fopen_s(&file, templateFile.u8string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameObjectFactory -- Failed to open template file '%s'", templateFile.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));


	fclose(file);

	rj::Document document;
	document.ParseStream(readStream);

	auto components = document["Component"].GetObj();
	for (auto& component : components)
	{
		const char* componentName = component.name.GetString();
		if (strcmp(componentName, "AnimatorComponent") == 0)
		{
			auto  animatorComponent = gameObject.AddComponent<AnimatorComponent>();

			if (component.value.HasMember("AnimSets"))
			{
				const auto & animSets= component.value["AnimSets"].GetArray();
				for (auto& animSet : animSets)
					animatorComponent->AddAnimation(animSet.GetString());
			}
		}
		else if (strcmp(componentName, "CameraComponent") == 0)
		{
			auto  cameraComponent = gameObject.AddComponent<CameraComponent>();
		}
		else if (strcmp(componentName, "ColliderComponent") == 0)
		{
			auto  colliderComponent = gameObject.AddComponent<ColliderComponent>();
			if (component.value.HasMember("Center"))
			{
				const auto& center = component.value["Center"].GetArray();
				const float x = center[0].GetFloat();
				const float y = center[1].GetFloat();
				const float z = center[2].GetFloat();
				colliderComponent->SetCenter({ x, y, z });
			}
			if (component.value.HasMember("Extend"))
			{
				const auto& extend = component.value["Extend"].GetArray();
				const float x = extend[0].GetFloat();
				const float y = extend[1].GetFloat();
				const float z = extend[2].GetFloat();
				colliderComponent->SetExtend({ x, y, z });
			}
		}
		else if (strcmp(componentName, "FPSCameraControllerComponent") == 0) 
		{ 
			auto fpsCameraControllerComponent = gameObject.AddComponent<FPSCameraControllerComponent>();
		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			auto  modelComponent = gameObject.AddComponent<ModelComponent>();
			if (component.value.HasMember("FileName"))
			{
				const auto& fileName = component.value["FileName"].GetString();
				modelComponent->SetFileName(fileName);
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& rotation = component.value["Rotation"].GetArray();
				const float x = rotation[0].GetFloat() * Math::Constants::DegToRad;
				const float y = rotation[1].GetFloat() * Math::Constants::DegToRad;
				const float z = rotation[2].GetFloat() * Math::Constants::DegToRad;
				modelComponent->SetRotation({ x,y,z });
			}
		}
		else if (strcmp(componentName, "TransformComponet") == 0)
		{
			auto transformComponent = gameObject.AddComponent<TransformComponent>();
			if (component.value.HasMember("Position"))
			{
				const auto& position = component.value["Position"].GetArray();
				const float x = position[0].GetFloat();
				const float y = position[1].GetFloat();
				const float z = position[2].GetFloat();
				transformComponent->position = { x, y, z };
			}
			if (component.value.HasMember("Rotation"))
			{
				const auto& position = component.value["Rotation"].GetArray();
				const float x = position[0].GetFloat() * Math::Constants::DegToRad;
				const float y = position[1].GetFloat() * Math::Constants::DegToRad;
				const float z = position[2].GetFloat() * Math::Constants::DegToRad;
				transformComponent->rotation = Math::Quaternion::RotationEuler({ x, y, z });
			}
			if (component.value.HasMember("Scale"))
			{
				const auto& position = component.value["Scale"].GetArray();
				const float x = position[0].GetFloat();
				const float y = position[1].GetFloat();
				const float z = position[2].GetFloat();
				transformComponent->scale = { x, y, z };
			}
		}
		// ... more component here...
	}
}
