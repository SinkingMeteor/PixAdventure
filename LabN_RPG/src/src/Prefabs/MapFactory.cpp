#include "Prefabs/MapFactory.h"
#include "Database.h"
namespace vg 
{
	 std::optional<entt::entity> MapFactory::CreateEntity(entt::registry& registry, const MapLoadingData& data)
	{
		std::string mapFullPath = (data.LoadingPath + data.MapFileName);
		std::ifstream input{ static_cast<const char*>(mapFullPath.c_str()) };
		nlohmann::json rootNode;
		input >> rootNode;
		input.close();

		if (rootNode.is_null()) return std::optional<entt::entity>{};

		nlohmann::json& tilesetNode = rootNode["tilesets"];
		for (nlohmann::json& tileSet : tilesetNode) 
		{
			entt::id_type tilesetId = entt::hashed_string{ tileSet["name"].get<std::string>().c_str() }.value();
			std::string tilesetFileName = tileSet["image"];
			m_textureProvider->load(tilesetId, (data.LoadingPath + tilesetFileName).c_str());
		}

		entt::resource<SlicedTexture> texture = (*m_textureProvider)[Database::Textures::DESERT_GROUND_TILESET];

		nlohmann::json& layersNode = rootNode["layers"];

		if(layersNode.is_null() || !layersNode.is_array()) return std::optional<entt::entity>{};

		std::vector<unsigned int> indices{};
		std::unordered_map<entt::id_type, sf::Vector2f> spawnPoints{};

		for (auto& layer : layersNode)
		{
			nlohmann::json tilesData = layer["data"];
			if (!tilesData.is_null())
			{
				indices = tilesData.get<std::vector<unsigned int>>();
			}
			nlohmann::json objectsData = layer["objects"];
			if (!objectsData.is_null())
			{
				for (auto& object : objectsData)
				{
					if (object["class"] == "SpawnPoint")
					{
						float x = object["x"].get<float>();
						float y = object["y"].get<float>();
						entt::id_type objectName = entt::hashed_string{ object["name"].get<std::string>().c_str() }.value();
						spawnPoints.emplace(std::make_pair(objectName, sf::Vector2f{ x, y }));
					}
				}
			}
		}
		unsigned int mapHeight = rootNode["height"].get<unsigned int>();
		unsigned int mapWidth = rootNode["width"].get<unsigned int>();
		unsigned int tileHeight = rootNode["tileheight"].get<unsigned int>();
		unsigned int tileWidth = rootNode["tilewidth"].get<unsigned int>();
		unsigned int vertexCount = mapHeight * mapWidth * 4;
		entt::entity mapEntity = registry.create();
		sf::VertexArray vertices{ sf::Quads, vertexCount };
		for (size_t x = 0; x < mapWidth; ++x)
		{
			for (size_t y = 0; y < mapHeight; ++y)
			{
				unsigned int tileNumber = indices[x + y * mapWidth] - 1;

				unsigned int tu = tileNumber % (texture->Texture.getSize().x / tileWidth);
				unsigned int tv = tileNumber / (texture->Texture.getSize().x / tileHeight);

				sf::Vertex* quad = &vertices[(x + y * mapWidth) * 4];

				quad[0].position = sf::Vector2f(x * tileWidth, y * tileHeight);
				quad[1].position = sf::Vector2f((x + 1) * tileWidth, y * tileHeight);
				quad[2].position = sf::Vector2f((x + 1) * tileWidth, (y + 1) * tileHeight);
				quad[3].position = sf::Vector2f(x * tileWidth, (y + 1) * tileHeight);

				quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
				quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);
			}
		}


		MapComponent& mapComponent = registry.emplace<MapComponent>(mapEntity);
		mapComponent.SpawnPoints = std::move(spawnPoints);
		mapComponent.MapIndices = std::move(indices);
		mapComponent.VertexArray = std::move(vertices);
		mapComponent.RelatedTexture = &*texture;
		mapComponent.MapSize = sf::Vector2<unsigned int>{ mapWidth, mapHeight };
		mapComponent.TileSize = sf::Vector2<unsigned int>{ tileWidth, tileHeight };
		return mapEntity;
	}
}