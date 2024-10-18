#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Components.h"
#include "ComponentManager.h"
#include "WorldObject.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

// Base System class
class System {
public:
    virtual ~System() {}
    virtual void Update(float deltaTime, ComponentManager& componentManager) = 0;
};

// Input System for player input
class InputSystem : public System {
public:
    GLFWwindow* window;
    unsigned int playerEntityID;

    InputSystem(GLFWwindow* window, unsigned int playerEntityID)
        : window(window), playerEntityID(playerEntityID) {}

    void Update(float deltaTime, ComponentManager& componentManager) override {
        Velocity* velocity = componentManager.getVelocity(playerEntityID);
        if (velocity) {
            velocity->vx = 0.f;
            velocity->vz = 0.f;

            const float speed = 5.f;

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                velocity->vz -= speed;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                velocity->vz += speed;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                velocity->vx -= speed;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                velocity->vx += speed;
        }
    }
};

// Velocity-Movement System (2D)
class MovementSystem : public System {
public:
    void Update(float deltaTime, ComponentManager& componentManager) override {
        for (size_t i = 0; i < componentManager.positions.size(); ++i) {
            unsigned int entityID = componentManager.positionEntityIDs[i];
            Position* position = componentManager.getPosition(entityID);
            Velocity* velocity = componentManager.getVelocity(entityID);

            if (velocity) {
                position->x += velocity->vx * deltaTime;
                position->z += velocity->vz * deltaTime;
            }
        }
    }
};

// AI Velocity System (2D)
class AISystem : public System {
public:
    unsigned int playerEntityID;

    AISystem(unsigned int playerEntityID)
        : playerEntityID(playerEntityID) {}

    void Update(float deltaTime, ComponentManager& componentManager) override {
        // Get player's position
        Position* playerPos = componentManager.getPosition(playerEntityID);
        if (!playerPos) return;

        // For each AI-active entity, update direction of movement
        for (size_t i = 0; i < componentManager.ais.size(); ++i) {
            unsigned int entityID = componentManager.aiEntityIDs[i];
            Position* enemyPos = componentManager.getPosition(entityID);
            Velocity* enemyVel = componentManager.getVelocity(entityID);
            AI& ai = componentManager.ais[i];

            if (enemyPos && enemyVel && ai.isActive) {
                // Calculate direction
                glm::vec2 direction(playerPos->x - enemyPos->x, playerPos->z - enemyPos->z);
                direction = glm::normalize(direction);

                float speed = 2.f;
                enemyVel->vx = direction.x * speed;
                enemyVel->vz = direction.y * speed;
            }
        }
    }
};

// Combat System (2D)
class CombatSystem : public System {
public:
    unsigned int playerEntityID;
    std::vector<unsigned int>& enemyEntities;
    std::vector<std::shared_ptr<WorldObject>>& enemyObjects;

    CombatSystem(unsigned int playerEntityID,
        std::vector<unsigned int>& enemyEntities,
        std::vector<std::shared_ptr<WorldObject>>& enemyObjects)
        : playerEntityID(playerEntityID),
        enemyEntities(enemyEntities),
        enemyObjects(enemyObjects) {}

    void Update(float deltaTime, ComponentManager& componentManager) override {
        // Get player components
        Position* playerPos = componentManager.getPosition(playerEntityID);
        Damage* playerDamage = componentManager.getDamage(playerEntityID);
        Health* playerHealth = componentManager.getHealth(playerEntityID);

        if (!playerPos || !playerDamage || !playerHealth) return;

        // For each enemy
        for (size_t i = 0; i < enemyEntities.size(); /* increment inside */) {
            unsigned int enemyEntityID = enemyEntities[i];

            Position* enemyPos = componentManager.getPosition(enemyEntityID);
            Velocity* enemyVelocity = componentManager.getVelocity(enemyEntityID);
            Damage* enemyDamage = componentManager.getDamage(enemyEntityID);
            Health* enemyHealth = componentManager.getHealth(enemyEntityID);

            if (!enemyPos || !enemyVelocity || !enemyDamage || !enemyHealth) {
                ++i;
                continue;
            }

            // Calculate distance
            float dx = playerPos->x - enemyPos->x;
            float dz = playerPos->z - enemyPos->z;
            float distance = sqrtf(dx * dx + dz * dz);

            float collisionRadius = 1.0f;

            if (distance < collisionRadius) {
                // Apply damage to the player
                playerHealth->currentHealth -= enemyDamage->damageAmount;
                if (playerHealth->currentHealth <= 0) {
                    std::cout << "Game Over!" << std::endl; // Player is dead
                    // Can continue playing regardless
                }

                // Apply damage to enemy
                enemyHealth->currentHealth -= playerDamage->damageAmount;
                if (enemyHealth->currentHealth <= 0) { // Enemy is dead
                    // Remove components
                    componentManager.removeHealth(enemyEntityID);
                    componentManager.removeDamage(enemyEntityID);
                    componentManager.removePosition(enemyEntityID);
                    componentManager.removeVelocity(enemyEntityID);
                    componentManager.removeAI(enemyEntityID);

                    // Remove from enemyEntities and enemyObjects lists
                    enemyEntities.erase(enemyEntities.begin() + i);
                    enemyObjects.erase(enemyObjects.begin() + i);

                    // Enemy removed, continue without incrementing i
                    continue;
                }

                // Collision: Push combatants away
                constexpr float pushMod{ 100.f };
                float overlap = collisionRadius - distance;
                float pushX = (dx / distance * overlap / 2) * pushMod;
                float pushZ = (dz / distance * overlap / 2) * pushMod;

                // Player push
                playerPos->x += pushX;
                playerPos->z += pushZ;

                // Enemy push
                enemyPos->x -= pushX;
                enemyPos->z -= pushZ;
            }
            ++i; // Continue iterating if unaltered vector
        }
    }
};

// Potion Pickup System (2D)
class PickupSystem : public System {
public:
    unsigned int playerEntityID;
    std::vector<unsigned int>& pickupEntities;
    std::vector<std::shared_ptr<WorldObject>>& pickupObjects;

    PickupSystem(unsigned int playerEntityID,
        std::vector<unsigned int>& pickupEntities,
        std::vector<std::shared_ptr<WorldObject>>& pickupObjects)
        : playerEntityID(playerEntityID),
        pickupEntities(pickupEntities),
        pickupObjects(pickupObjects) {}

    void Update(float deltaTime, ComponentManager& componentManager) override {
        Position* playerPos = componentManager.getPosition(playerEntityID);
        Inventory* playerInventory = componentManager.getInventory(playerEntityID);

        if (!playerPos || !playerInventory) return;

        // For each pickup
        for (size_t i = 0; i < componentManager.pickupEntityIDs.size(); /* Increment inside */) {
            unsigned int pickupEntityID = componentManager.pickupEntityIDs[i];
            Position* pickupPos = componentManager.getPosition(pickupEntityID);
            Pickup* pickup = componentManager.getPickup(pickupEntityID);

            if (!pickupPos || !pickup) {
                ++i;
                continue;
            }

            // Calculate distance in 2D
            float dx = playerPos->x - pickupPos->x;
            float dz = playerPos->z - pickupPos->z;
            float distance = sqrtf(dx * dx + dz * dz);

            float collisionRadius = 1.0f;

            if (distance < collisionRadius) {
                // Add item to player's inventory
                playerInventory->items.push_back(pickup->itemName);

                // Remove the pickup entity components
                componentManager.removePickup(pickupEntityID);
                componentManager.removePosition(pickupEntityID);

                // Remove from rendering lists
                auto it = std::find(pickupEntities.begin(), pickupEntities.end(), pickupEntityID);
                if (it != pickupEntities.end()) {
                    size_t index = std::distance(pickupEntities.begin(), it);
                    pickupEntities.erase(it);
                    pickupObjects.erase(pickupObjects.begin() + index);
                }

                // Pickup removed, iterate on same index which contains next element
                continue;
            }
            ++i; // Continue iterating if unaltered vector
        }
    }
};

#endif
