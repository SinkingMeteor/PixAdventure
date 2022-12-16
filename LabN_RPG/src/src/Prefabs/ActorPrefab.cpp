#include "Prefabs/ActorPrefab.h"

namespace vg 
{
	entt::entity ActorPrefab::CreateEntity(entt::registry& registry)
	{
		entt::entity actor = registry.create();

		registry.emplace<TransformComponent>(actor, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 1.0f, 1.0f }, 0.0f);
		registry.emplace<MovementComponent>(actor, sf::Vector2f{ 0.0f, 0.0f }, 100.0f);
		registry.emplace<Possessable>(actor);
		registry.emplace<SpriteComponent>(actor);

		return actor;
	}
}