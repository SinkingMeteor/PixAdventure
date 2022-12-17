#pragma once
#include <iostream>
#include <cassert>

#include "World/World.h"
#include "Systems/Systems.h"
#include "Components/Components.h"
#include "ResourceManagement/TextureProvider.h"
#include "ResourceManagement/AnimationProvider.h"
#include "Prefabs/ActorPrefab.h"
#include "entt/entt.hpp"
#include "Database.h"
namespace vg
{
	class GameWorld final : public World 
	{
	public:
		GameWorld(Window* window);
		virtual void Initialize() override;
		virtual void Tick(sf::Time deltaTime) override;
		virtual void Render() override;

	private:
		std::vector<std::unique_ptr<System>> m_systems;
		std::vector<std::unique_ptr<IRenderSystem>> m_renderSystems;
		TextureProvider m_textureProvider;
		AnimationProvider m_animationProvider;

		void LoadResources();
		void InitializeSystems();
	};
}