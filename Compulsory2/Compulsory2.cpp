#include <iostream>

#include "Renderer.h"
#include "PrimitiveGenerator.h"
#include "Camera.h"
#include "WorldObject.h"
#include "Components.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Systems.h"
#include "UIManager.h"
#include "Level.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// GLFW window size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Compulsory 2 | 2SPIM131", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // Initialize renderer
    Renderer renderer;
    renderer.setAspect(SCR_WIDTH, SCR_HEIGHT);

    // Initialize ECS
    EntityManager entityManager;
    ComponentManager componentManager;

    // Create player entity
    unsigned int playerEntity = entityManager.createEntity();
    componentManager.addComponent(playerEntity, Position(0.f, 0.f));
    componentManager.addComponent(playerEntity, Velocity(0.f, 0.f));
    componentManager.addComponent(playerEntity, Health(100, 100));
    componentManager.addComponent(playerEntity, Damage(25));
    componentManager.addComponent(playerEntity, Inventory());

    // Add potion to player inventory
    Inventory* playerInventory = componentManager.getInventory(playerEntity);
    if (playerInventory) {
        playerInventory->items.push_back("Potion");
    }

    // Create player mesh
    Mesh3D playerMesh = PrimitiveGenerator::createBox(1.0f, 2.0f, 1.0f, glm::vec3(0.f, 0.f, 1.f));
    std::shared_ptr<WorldObject> playerObject = std::make_shared<WorldObject>(playerMesh);

    // Initialize Level
    Level level(entityManager, componentManager);

    // Generate level with specified number of enemies and pickups
    int numEnemies = 1;
    int numPickups = 2;
    unsigned int seed = 12345; // First level is always the same
    level.generateLevel(numEnemies, numPickups, seed);

    // Initialize systems
    InputSystem inputSystem(window /*Client Input*/, playerEntity /*Affected player*/);
    AISystem aiSystem(playerEntity         /*Target player for enemy AI*/);
    CombatSystem combatSystem(playerEntity /*Target player for enemy AI*/, level.getEnemyEntities(), level.getEnemyObjects());
    MovementSystem movementSystem;
    PickupSystem pickupSystem(playerEntity /*Player able to pick up*/, level.getPickupEntities(), level.getPickupObjects());

    // Initialize Camera
    Camera camera;

    // Initialize UI Manager
    UIManager uiManager(window, componentManager, playerEntity);

    // Initialize frame time tracking
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        // Update deltaTime
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process window input
        glfwPollEvents();

        // Start ImGui frame
        uiManager.beginFrame();

        // Update Systems
        pickupSystem.Update(deltaTime, componentManager);
        inputSystem.Update(deltaTime, componentManager);
        aiSystem.Update(deltaTime, componentManager);
        combatSystem.Update(deltaTime, componentManager);
        movementSystem.Update(deltaTime, componentManager);

        // Check if all enemies are defeated
        if (level.getEnemyEntities().empty()) {
            std::cout << "You have cleared the level!" << std::endl;

            // Increase difficulty: More enemies
            numEnemies += 2;
            numPickups += 1;
            // generateLevel can be reworked to adjust enemy damage here

            // Generate a new level
            level.generateLevel(numEnemies, numPickups /*, Random Seed */);
        }

        // Update camera to follow player
        Position* playerPos = componentManager.getPosition(playerEntity);
        if (playerPos) {
            glm::vec3 camOffset(0.f, 10.f, 10.f);
            camera.position = glm::vec3(playerPos->x, 0.f /* Fixed Y */, playerPos->z) + camOffset;
            camera.front = glm::normalize(glm::vec3(playerPos->x, 0.f /* Fixed Y */, playerPos->z) - camera.position);
            camera.up = glm::vec3(0.f, 1.f, 0.f);
        }

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update WorldObject positions from ECS positions
        if (playerPos) {
            playerObject->position = glm::vec3(playerPos->x, 0.f /* Fixed Y */, playerPos->z);
        }

        // Update enemy positions and render
        const auto& enemyEntities = level.getEnemyEntities();
        const auto& enemyObjects = level.getEnemyObjects();

        for (size_t i = 0; i < enemyEntities.size(); ++i) {
            unsigned int enemyEntityID = enemyEntities[i];
            Position* enemyPos = componentManager.getPosition(enemyEntityID);
            if (enemyPos) {
                enemyObjects[i]->position = glm::vec3(enemyPos->x, 0.f /* Fixed Y */, enemyPos->z);
            }
        }

        for (const auto& enemyObj : enemyObjects) {
            renderer.render(enemyObj, camera);
        }

        // Update pickup positions and render
        const auto& pickupEntities = level.getPickupEntities();
        const auto& pickupObjects = level.getPickupObjects();

        for (size_t i = 0; i < pickupEntities.size(); ++i) {
            unsigned int pickupEntityID = pickupEntities[i];
            Position* pickupPos = componentManager.getPosition(pickupEntityID);
            if (pickupPos) {
                pickupObjects[i]->position = glm::vec3(pickupPos->x, 0.f /* Fixed Y */, pickupPos->z);
            }
        }

        for (const auto& pickupObj : pickupObjects) {
            renderer.render(pickupObj, camera);
        }

        // Render player
        renderer.render(playerObject, camera);

        // Render UI
        uiManager.render();

        // End ImGui frame
        uiManager.endFrame();

        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}