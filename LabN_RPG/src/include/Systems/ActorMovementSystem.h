#pragma once
#include "Systems/ISystem.h"
#include "Components/TransformComponent.h"
#include "Components/MovementComponent.h"
namespace vg 
{
	class ActorMovementSystem : public ISystem
	{
		virtual void Tick(entt::registry& registry, sf::Time deltaTime) override;
	};
}