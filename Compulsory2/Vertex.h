#ifndef VERTEX_H
#define VERTEX_H

#include <glm/vec3.hpp>

class Vertex3D {
public:
    glm::vec3 position;
    glm::vec3 color;

    // Default constructor
    Vertex3D() : position(0.0f), color(0.0f) {}

    // Constructor for position and color
    Vertex3D(const glm::vec3& pos, const glm::vec3& col) : position(pos), color(col) {}
};
#endif
