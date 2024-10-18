#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityManager {
public:
    EntityManager() : nextEntityID(0) {}

    unsigned int createEntity() {
        return nextEntityID++;
    }

private:
    unsigned int nextEntityID{ 0 };
};

#endif
