#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float FoV{ 75.0f };
    float nearClip{ 0.1f };
    float farClip{ 100.0f };

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
        : position(position), front(front), up(up) {}
};

#endif
