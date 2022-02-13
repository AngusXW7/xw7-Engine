#pragma once

#include "CameraService.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Service.h"

namespace Storm
{
	class GameWorld final
	{
	public:
		void Initialize(uint32_t capacity);
		void Terminate();

		void Update(float deltaTime);
		void Render();
		void DebugUI();

		template<class ServiceType>
		ServiceType* AddService()
		{
			ASSERT(!mInitialized, "GameWorld - Can only add service before world is initialized.");
			auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
			newService->mWorld = this;
			return static_cast<ServiceType*>(newService.get());
		}

		template <class ServiceType>
		ServiceType* GetService()
		{
			for (auto& service : mServices)
			{
				if (service->GetTypeId() == ServiceType::StaticGetTypeId())
					return static_cast<ServiceType*>(service.get());
			}
			return nullptr;
		}

		GameObject* CreateGameObject(std::filesystem::path templateFile);
		GameObject* FindGameObject(const std::string& name);
		GameObject* GetGameObject(GameObjectHandle handle);
		void DestoryGameObject(GameObjectHandle handle);

	private:
		bool IsValid(GameObjectHandle handle) const;
		void ProcessDestoryList();

		struct Slot
		{
			std::unique_ptr<GameObject> gameObject;
			uint32_t generation = 0;
		};

		using Services = std::vector<std::unique_ptr<Service>>;
		using GameObjectSlots = std::vector<Slot>;
		using GameObjectList = std::vector<GameObject*>;

		Services mServices;

		GameObjectSlots mGameObjectSlots;
		std::vector<uint32_t> mFreeSlots;
		std::vector<uint32_t> mToBeDestoryed;


		GameObjectList mUpdateList;

		bool mInitialized = false;
		bool mUpdating = false;
	};
}