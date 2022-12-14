#include "Systems/SpriteRenderSystem.h"
#include "MathUtils.h"
namespace vg
{
	void SpriteRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
	{
		registry.sort<TransformComponent>([](const TransformComponent& lhs, const TransformComponent& rhs)
		{
			return (lhs.Transform * lhs.Origin).y < (rhs.Transform * rhs.Origin).y;
		});

		auto groundView = registry.view<TransformComponent, DrawableComponent, GroundSortingLayer>().use<TransformComponent>();
		DrawLayer(groundView, window);

		auto onGroundView = registry.view<TransformComponent, DrawableComponent, OnGroundSortingLayer>().use<TransformComponent>();
		DrawLayer(onGroundView, window);

		auto aboveGroundView = registry.view<TransformComponent, DrawableComponent, AboveGroundSortingLayer>().use<TransformComponent>();
		DrawLayer(aboveGroundView, window);
	}

	void SpriteRenderSystem::DrawLayer(auto& view, sf::RenderWindow& window)
	{
		for (entt::entity entity : view)
		{
			DrawableComponent& spriteComponent = view.get<DrawableComponent>(entity);
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			sf::RenderStates states{};
			states.texture = &spriteComponent.RelatedTexture->Texture;
			std::size_t amountOfTiles = spriteComponent.VertexArray.getVertexCount() / 4;

			//todo: solve copy problem
			std::vector<sf::Vertex> verticesToDraw{};
			verticesToDraw.reserve(amountOfTiles);

			for (size_t i = 0; i < amountOfTiles; ++i)
			{
				if (spriteComponent.CullingBits[i]) continue;

				const int spriteRectIndex = spriteComponent.RectsIndices[i];
				if (spriteRectIndex < 0) continue;

				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 1]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 2]);
				verticesToDraw.push_back(spriteComponent.VertexArray[i * 4 + 3]);

				const TextureRect& spriteRect = spriteComponent.RelatedTexture->RectDatas[spriteRectIndex];
				float top = (float)spriteRect.Rect.top;
				float left = (float)spriteRect.Rect.left;
				float width = (float)spriteRect.Rect.width;
				float height = (float)spriteRect.Rect.height;

				spriteComponent.VertexArray[i * 4].texCoords = sf::Vector2f{ left, top };
				spriteComponent.VertexArray[i * 4 + 1].texCoords = sf::Vector2f{ left + width, top };
				spriteComponent.VertexArray[i * 4 + 2].texCoords = sf::Vector2f{ left + width, top + height };
				spriteComponent.VertexArray[i * 4 + 3].texCoords = sf::Vector2f{ left, top + height };
			}

			if (verticesToDraw.size() > 0)
			{
				window.draw(verticesToDraw.data(), verticesToDraw.size(), sf::Quads, states);
			}
		}
	}
}

