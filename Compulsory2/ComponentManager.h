#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "Components.h"
#include <vector>
#include <unordered_map>

// Component allocation and management
class ComponentManager {
public:
    // Store entity IDs alongside components for quick access
    std::vector<unsigned int> positionEntityIDs;
    std::vector<Position> positions;

    std::vector<unsigned int> velocityEntityIDs;
    std::vector<Velocity> velocities;

    std::vector<unsigned int> healthEntityIDs;
    std::vector<Health> healths;

    std::vector<unsigned int> damageEntityIDs;
    std::vector<Damage> damages;

    std::vector<unsigned int> inventoryEntityIDs;
    std::vector<Inventory> inventories;

    std::vector<unsigned int> aiEntityIDs;
    std::vector<AI> ais;

    std::vector<unsigned int> pickupEntityIDs;
    std::vector<Pickup> pickups;

    // Maps for quick component lookup by entity ID
    std::unordered_map<unsigned int, size_t> positionIndices;
    std::unordered_map<unsigned int, size_t> velocityIndices;
    std::unordered_map<unsigned int, size_t> healthIndices;
    std::unordered_map<unsigned int, size_t> damageIndices;
    std::unordered_map<unsigned int, size_t> inventoryIndices;
    std::unordered_map<unsigned int, size_t> aiIndices;
    std::unordered_map<unsigned int, size_t> pickupIndices;

    // Add Component Methods
    void addComponent(unsigned int entityID, const Position& position) {
        positionEntityIDs.push_back(entityID);
        positions.push_back(position);
        positionIndices[entityID] = positions.size() - 1;
    }

    void addComponent(unsigned int entityID, const Velocity& velocity) {
        velocityEntityIDs.push_back(entityID);
        velocities.push_back(velocity);
        velocityIndices[entityID] = velocities.size() - 1;
    }

    void addComponent(unsigned int entityID, const Health& health) {
        healthEntityIDs.push_back(entityID);
        healths.push_back(health);
        healthIndices[entityID] = healths.size() - 1;
    }

    void addComponent(unsigned int entityID, const Damage& damage) {
        damageEntityIDs.push_back(entityID);
        damages.push_back(damage);
        damageIndices[entityID] = damages.size() - 1;
    }

    void addComponent(unsigned int entityID, const Inventory& inventory) {
        inventoryEntityIDs.push_back(entityID);
        inventories.push_back(inventory);
        inventoryIndices[entityID] = inventories.size() - 1;
    }

    void addComponent(unsigned int entityID, const AI& ai) {
        aiEntityIDs.push_back(entityID);
        ais.push_back(ai);
        aiIndices[entityID] = ais.size() - 1;
    }

    void addComponent(unsigned int entityID, const Pickup& pickup) {
        pickupEntityIDs.push_back(entityID);
        pickups.push_back(pickup);
        pickupIndices[entityID] = pickups.size() - 1;
    }

    // Get Component Methods
    Position* getPosition(unsigned int entityID) {
        auto it = positionIndices.find(entityID);
        if (it != positionIndices.end()) {
            return &positions[it->second];
        }
        return nullptr;
    }

    Velocity* getVelocity(unsigned int entityID) {
        auto it = velocityIndices.find(entityID);
        if (it != velocityIndices.end()) {
            return &velocities[it->second];
        }
        return nullptr;
    }

    Health* getHealth(unsigned int entityID) {
        auto it = healthIndices.find(entityID);
        if (it != healthIndices.end()) {
            return &healths[it->second];
        }
        return nullptr;
    }

    Damage* getDamage(unsigned int entityID) {
        auto it = damageIndices.find(entityID);
        if (it != damageIndices.end()) {
            return &damages[it->second];
        }
        return nullptr;
    }

    Inventory* getInventory(unsigned int entityID) {
        auto it = inventoryIndices.find(entityID);
        if (it != inventoryIndices.end()) {
            return &inventories[it->second];
        }
        return nullptr;
    }

    AI* getAI(unsigned int entityID) {
        auto it = aiIndices.find(entityID);
        if (it != aiIndices.end()) {
            return &ais[it->second];
        }
        return nullptr;
    }

    Pickup* getPickup(unsigned int entityID) {
        auto it = pickupIndices.find(entityID);
        if (it != pickupIndices.end()) {
            return &pickups[it->second];
        }
        return nullptr;
    }

    // Remove Component Methods
    void removePosition(unsigned int entityID) {
        auto it = positionIndices.find(entityID);
        if (it != positionIndices.end()) {
            size_t index = it->second;
            positions.erase(positions.begin() + index);
            positionEntityIDs.erase(positionEntityIDs.begin() + index);
            positionIndices.erase(it);

            // Update indices
            for (size_t i = index; i < positions.size(); ++i) {
                positionIndices[positionEntityIDs[i]] = i;
            }
        }
    }

    void removeVelocity(unsigned int entityID) {
        auto it = velocityIndices.find(entityID);
        if (it != velocityIndices.end()) {
            size_t index = it->second;
            velocities.erase(velocities.begin() + index);
            velocityEntityIDs.erase(velocityEntityIDs.begin() + index);
            velocityIndices.erase(it);

            // Update indices
            for (size_t i = index; i < velocities.size(); ++i) {
                velocityIndices[velocityEntityIDs[i]] = i;
            }
        }
    }

    void removeHealth(unsigned int entityID) {
        auto it = healthIndices.find(entityID);
        if (it != healthIndices.end()) {
            size_t index = it->second;
            healths.erase(healths.begin() + index);
            healthEntityIDs.erase(healthEntityIDs.begin() + index);
            healthIndices.erase(it);

            // Update indices
            for (size_t i = index; i < healths.size(); ++i) {
                healthIndices[healthEntityIDs[i]] = i;
            }
        }
    }

    void removeDamage(unsigned int entityID) {
        auto it = damageIndices.find(entityID);
        if (it != damageIndices.end()) {
            size_t index = it->second;
            damages.erase(damages.begin() + index);
            damageEntityIDs.erase(damageEntityIDs.begin() + index);
            damageIndices.erase(it);

            // Update indices
            for (size_t i = index; i < damages.size(); ++i) {
                damageIndices[damageEntityIDs[i]] = i;
            }
        }
    }

    void removeAI(unsigned int entityID) {
        auto it = aiIndices.find(entityID);
        if (it != aiIndices.end()) {
            size_t index = it->second;
            ais.erase(ais.begin() + index);
            aiEntityIDs.erase(aiEntityIDs.begin() + index);
            aiIndices.erase(it);

            // Update indices
            for (size_t i = index; i < ais.size(); ++i) {
                aiIndices[aiEntityIDs[i]] = i;
            }
        }
    }

    void removePickup(unsigned int entityID) {
        auto it = pickupIndices.find(entityID);
        if (it != pickupIndices.end()) {
            size_t index = it->second;
            pickups.erase(pickups.begin() + index);
            pickupEntityIDs.erase(pickupEntityIDs.begin() + index);
            pickupIndices.erase(it);

            // Update indices
            for (size_t i = index; i < pickups.size(); ++i) {
                pickupIndices[pickupEntityIDs[i]] = i;
            }
        }
    }
};
#endif
