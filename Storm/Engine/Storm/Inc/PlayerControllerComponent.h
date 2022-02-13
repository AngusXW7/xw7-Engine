#pragma once

#include "Component.h"

namespace Storm
{
	class AnimatorComponent;
	class TransformComponent;

	class PlayerControllerComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::PlayerController);
		MEMORY_POOL_DECLARE

		enum AnimState
		{
			Idle = 1,
			Walk = 2
		};

		void Initialize() override;

		void Update(float deltaTime) override;
		void DebugUI() override;

		void SetWalkSpeed(float walkSpeed) { mWalkSpeed = walkSpeed; }
		void SetRunSpeed(float runSpeed) { mRunSpeed = runSpeed; }

	private:
		TransformComponent* mTransformComponent = nullptr;
		AnimatorComponent* mAnimatorComponent = nullptr;
		float mWalkSpeed = 1.0f;
		float mRunSpeed = 1.0f;
	};
}