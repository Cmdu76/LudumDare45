#include "EntityPrefab.hpp"

#include <Enlivengine/Core/Components.hpp>

#include "Components.hpp"

bool EntityPrefab::createEntity(entt::registry& world, en::F32 x, en::F32 y)
{
	auto entity = world.create();
	auto& position = world.assign<en::PositionComponent>(entity);
	position.setPosition(x, y);
	auto& renderable = world.assign<en::RenderableComponent>(entity);
	renderable.z = 0;
	auto& sprite = world.assign<en::SpriteComponent>(entity);
	//sprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>(GameSingleton::mEverythingTexture).Get());
	sprite.setTextureRect(en::toSF(en::Recti(0, 0, 120, 120)));
	sprite.setOrigin(60, 60);
	auto& text = world.assign<en::TextComponent>(entity);
	//text.setFont(en::ResourceManager::GetInstance().Get<en::Font>(GameSingleton::mFont).Get());
	text.setPosition(10, 10);
	text.setString("Asteroid");
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1.f);

	return true;
}

bool EntityPrefab::createPlayer(entt::registry& world, entt::entity& playerEntity)
{
	playerEntity = world.create();
#ifdef ENLIVE_DEBUG
	auto& nameComponent = world.assign<en::NameComponent>(playerEntity);
	nameComponent.insert(0, "Player");
#endif
	world.assign<en::PositionComponent>(playerEntity);
	world.assign<en::RenderableComponent>(playerEntity);
	world.assign<PlayerComponent>(playerEntity);
	auto& humanComponent = world.assign<HumanComponent>(playerEntity);
	humanComponent.body.setTexture(GameSingleton::mEverythingTexture.Get());
	humanComponent.body.setOrigin(8, 16);
	humanComponent.body.setTextureRect(sf::IntRect(0, 0, 16, 16));
	world.assign<VelocityComponent>(playerEntity);
	return true;
}

bool EntityPrefab::createNothing(entt::registry& world, entt::entity& nothingEntity)
{
	nothingEntity = world.create();
#ifdef ENLIVE_DEBUG
	auto& nameComponent = world.assign<en::NameComponent>(nothingEntity);
	nameComponent.insert(0, "Nothing");
#endif
	world.assign<en::PositionComponent>(nothingEntity);
	world.assign<en::RenderableComponent>(nothingEntity);
	world.assign<NothingComponent>(nothingEntity);
	auto& humanComponent = world.assign<HumanComponent>(nothingEntity);
	humanComponent.body.setTexture(GameSingleton::mNothingTexture.Get());
	humanComponent.body.setOrigin(8, 16);
	humanComponent.body.setTextureRect(sf::IntRect(0, 0, 16, 16));
	humanComponent.direction = HumanComponent::Direction::BasGauche;
	humanComponent.currentAnimation = HumanComponent::Animation::Idle;
	humanComponent.animLoop = true;
	humanComponent.frameTime = en::Time::Zero;
	humanComponent.currentFrame = 0;
	world.assign<VelocityComponent>(nothingEntity);
	return true;
}


bool EntityPrefab::createProps(entt::registry& world, en::F32 x, en::F32 y, en::U32 gid, en::tmx::TilesetPtr tileset)
{
	auto entity = world.create();
#ifdef ENLIVE_DEBUG
	static int propsCounter = 0;
	auto& nameComponent = world.assign<en::NameComponent>(entity);
	nameComponent.insert(0, "Props-" + std::to_string(propsCounter++));
#endif
	auto& positionComponent = world.assign<en::PositionComponent>(entity);
	positionComponent.setPosition(x + 8, y + 8);
	world.assign<en::RenderableComponent>(entity);
	auto& propsComponent = world.assign<PropsComponent>(entity);
	propsComponent.sprite.setOrigin(8, 8);
	propsComponent.coords = GameSingleton::mMap.worldToCoords(en::Vector2f(x + 8.0f, y + 8.0f));
	propsComponent.destructFrames = 3;
	propsComponent.gid = gid;
	if (tileset.IsValid())
	{
		if (tileset.Get().GetTexture().IsValid())
		{
			propsComponent.sprite.setTexture(tileset.Get().GetTexture().Get());
		}
		const sf::Vector2f vertexPos = toSF(tileset.Get().ToPos(gid));
		propsComponent.sprite.setTextureRect(sf::IntRect((int)vertexPos.x, (int)vertexPos.y, 16, 16));
	}

	if (gid == 113)
	{
		auto& doorComponent = world.assign<DoorComponent>(entity);
		doorComponent.enabled = GameSingleton::mFirstThrowNothingDone;
		doorComponent.cooldownSpawnTimer = en::seconds(en::Random::get<en::F32>(8, 20));
	}

	return true;
}

bool EntityPrefab::createAI(entt::registry& world, en::F32 x, en::F32 y)
{
	if (GameSingleton::world.view<AIComponent>().size() > 50)
	{
		return true;
	}

	static int aiCounter = 0;
	auto entity = world.create();
#ifdef ENLIVE_DEBUG
	auto& nameComponent = world.assign<en::NameComponent>(entity);
	nameComponent.insert(0, "AI-" + std::to_string(aiCounter++));
#endif
	auto& positionComponent = world.assign<en::PositionComponent>(entity);
	positionComponent.setPosition(x, y);
	world.assign<en::RenderableComponent>(entity);
	auto& aiComponent = world.assign<AIComponent>(entity);
	aiComponent.randomEngine.setSeed(aiCounter);
	auto& humanComponent = world.assign<HumanComponent>(entity);
	humanComponent.body.setTexture(GameSingleton::mAITexture.Get());
	humanComponent.body.setOrigin(8, 16);
	humanComponent.body.setTextureRect(sf::IntRect(0, 0, 16, 16));
	humanComponent.direction = HumanComponent::Direction::BasGauche;
	humanComponent.currentAnimation = HumanComponent::Animation::Idle;
	humanComponent.animLoop = true;
	humanComponent.frameTime = en::Time::Zero;
	humanComponent.currentFrame = 0;
	world.assign<VelocityComponent>(entity);
	return true;
}

bool EntityPrefab::createPiece(entt::registry& world, en::F32 x, en::F32 y)
{
	auto entity = world.create();
#ifdef ENLIVE_DEBUG
	static int pieceCounter = 0;
	auto& nameComponent = world.assign<en::NameComponent>(entity);
	nameComponent.insert(0, "Piece-" + std::to_string(pieceCounter++));
#endif
	auto& positionComponent = world.assign<en::PositionComponent>(entity);
	positionComponent.setPosition(x, y);
	world.assign<en::RenderableComponent>(entity);
	auto& pieceComponent = world.assign<PieceComponent>(entity);
	pieceComponent.sprite.setScale(sf::Vector2f(0.3f, 0.3f));
	pieceComponent.sprite.setOrigin(sf::Vector2f(2, 2));
	pieceComponent.sprite.setTexture(en::ResourceManager::GetInstance().Get<en::Texture>("coin").Get());
	pieceComponent.sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	pieceComponent.value = 1;
	return true;
}
