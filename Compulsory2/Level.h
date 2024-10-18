// Level.h
#pragma once

#include <vector>
#include <memory>
#include <random>
#include "EntityManager.h"
#include "ComponentManager.h"
#include "WorldObject.h"
#include "PrimitiveGenerator.h"

class Level {
public:
    Level(EntityManager& entityManager, ComponentManager& componentManager);

    // Generate level with enemies and pickups, randomized if seed == 0
    void generateLevel(int numEnemies, int numPickups, unsigned int seed = 0);

    // Get entities and WorldObjects
    std::vector<unsigned int>& getEnemyEntities();
    std::vector<std::shared_ptr<WorldObject>>& getEnemyObjects();

    std::vector<unsigned int>& getPickupEntities();
    std::vector<std::shared_ptr<WorldObject>>& getPickupObjects();

private:
    EntityManager& entityManager;
    ComponentManager& componentManager;

    // Entities and WorldObjects
    std::vector<unsigned int> enemyEntities;
    std::vector<std::shared_ptr<WorldObject>> enemyObjects;

    std::vector<unsigned int> pickupEntities;
    std::vector<std::shared_ptr<WorldObject>> pickupObjects;

    // Meshes for enemies and pickups
    Mesh3D enemyMesh;
    Mesh3D pickupMesh;
};
