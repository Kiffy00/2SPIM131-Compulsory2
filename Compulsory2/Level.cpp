#include "Level.h"

Level::Level(EntityManager& entityManager, ComponentManager& componentManager)
    : entityManager(entityManager), componentManager(componentManager) {
    // Initialize meshes
    enemyMesh = PrimitiveGenerator::createBox(1.0f, 2.0f, 1.0f, glm::vec3(1.f, 0.f, 0.f));
    pickupMesh = PrimitiveGenerator::createBox(0.5f, 0.5f, 0.5f, glm::vec3(0.f, 1.f, 0.f));
}

void Level::generateLevel(int numEnemies, int numPickups, unsigned int seed) {
    std::mt19937 rng;
    if (seed == 0) {
        rng.seed(static_cast<unsigned int>(std::random_device{}()));
    }
    else {
        rng.seed(seed);
    }
    std::uniform_real_distribution<float> dist(-20.0f, 20.0f);

    // Generate enemies
    for (int i = 0; i < numEnemies; ++i) {
        float x = dist(rng);
        float z = dist(rng);

        unsigned int enemyEntity = entityManager.createEntity();
        componentManager.addComponent(enemyEntity, Position(x, z));
        componentManager.addComponent(enemyEntity, Velocity(0.f, 0.f));
        componentManager.addComponent(enemyEntity, Health(50));
        componentManager.addComponent(enemyEntity, Damage(15));
        componentManager.addComponent(enemyEntity, AI(true));

        std::shared_ptr<WorldObject> enemyObject = std::make_shared<WorldObject>(enemyMesh);
        enemyEntities.push_back(enemyEntity);
        enemyObjects.push_back(enemyObject);
    }

    // Generate pickups
    for (int i = 0; i < numPickups; ++i) {
        float x = dist(rng);
        float z = dist(rng);

        unsigned int pickupEntity = entityManager.createEntity();
        componentManager.addComponent(pickupEntity, Position(x, z));
        componentManager.addComponent(pickupEntity, Pickup("Potion"));

        std::shared_ptr<WorldObject> pickupObject = std::make_shared<WorldObject>(pickupMesh);
        pickupEntities.push_back(pickupEntity);
        pickupObjects.push_back(pickupObject);
    }
}

std::vector<unsigned int>& Level::getEnemyEntities() {
    return enemyEntities;
}

std::vector<std::shared_ptr<WorldObject>>& Level::getEnemyObjects() {
    return enemyObjects;
}

std::vector<unsigned int>& Level::getPickupEntities() {
    return pickupEntities;
}

std::vector<std::shared_ptr<WorldObject>>& Level::getPickupObjects() {
    return pickupObjects;
}
