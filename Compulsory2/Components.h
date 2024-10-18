#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <vector>
#include <string>

// Base Arbitrary Component class
class Component {
public:
    virtual ~Component() {}
};

// Position Component (2D)
class Position : public Component {
public:
    float x, z;
    Position(float x = 0.f, float z = 0.f)
        : x(x), z(z) {}
};

// Velocity Component (2D)
class Velocity : public Component {
public:
    float vx, vz;
    Velocity(float vx = 0.f, float vz = 0.f)
        : vx(vx), vz(vz) {}
};

// Health Component
class Health : public Component {
public:
    int currentHealth;
    int maxHealth;
    Health(int health = 100, int maxHealth = 100)
        : currentHealth(health), maxHealth(maxHealth) {}
};

// Damage Component
class Damage : public Component {
public:
    int damageAmount;
    Damage(int damage = 10)
        : damageAmount(damage) {}
};

// Inventory Component
class Inventory : public Component {
public:
    std::vector<std::string> items;
};

// AI Component
class AI : public Component {
public:
    bool isActive;
    AI(bool active = true)
        : isActive(active) {}
};

// Pickup Component
class Pickup : public Component {
public:
    std::string itemName;

    Pickup(const std::string& itemName)
        : itemName(itemName) {}
};


#endif
