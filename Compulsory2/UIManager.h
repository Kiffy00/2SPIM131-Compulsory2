#pragma once
#include <string>
#include <vector>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include <GLFW/glfw3.h>
#include "ImGui/imgui_impl_opengl3.h"
#include "ComponentManager.h"

class UIManager {
public:
    UIManager(GLFWwindow* window, ComponentManager& componentManager, unsigned int playerEntity);
    ~UIManager();

    void beginFrame();
    void endFrame();
    void render();

private:
    GLFWwindow* window;
    ComponentManager& componentManager;
    unsigned int playerEntity;

    void buildUI();
};
