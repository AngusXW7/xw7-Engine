#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace Storm;

MEMORY_POOL_DEFINE(ColliderComponent, 1000);

void ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void ColliderComponent::DebugUI()
{
	const auto& aabb = GetAABB();
	Graphics::SimpleDraw::AddAABB(aabb.center, aabb.extend, Graphics::Colors::Red);
}

Math::AABB ColliderComponent::GetAABB() const
{
	Math::AABB aabb;
	aabb.center = mTransformComponent->position + mCenter;
	aabb.extend = mExtend;
	return aabb;
}
