#include "UIManager.h"

UIManager::UIManager(GLFWwindow* window, ComponentManager& componentManager, unsigned int playerEntity)
    : window(window), componentManager(componentManager), playerEntity(playerEntity) {
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;  // Disable setting saving

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

UIManager::~UIManager() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::beginFrame() {
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::endFrame() {
    // Render ImGui UI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::render() {
    buildUI();
}

void UIManager::buildUI() {
    static bool firstFrame = true; // Set window size once
    if (firstFrame) {
        float windowWidth = 300;
        float windowHeight = 150;

        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        firstFrame = false;
    }

    // Begin main window
    ImGui::Begin("Player Status");

    // Get player Health and Inventory components
    Health* health = componentManager.getHealth(playerEntity);
    Inventory* inventory = componentManager.getInventory(playerEntity);

    // Display player health
    if (health != nullptr) 
        ImGui::Text("Health: %d / %d", health->currentHealth, health->maxHealth);
    else 
        ImGui::Text("Health: N/A");

    // Display player inventory
    if (inventory) {
        ImGui::Separator();
        ImGui::Text("Inventory:");

        if (inventory->items.empty()) {
            ImGui::Text("  (empty)");
        }
        else {
            for (size_t i = 0; i < inventory->items.size(); ++i) {
                const std::string& item = inventory->items[i];
                if (ImGui::Button(item.c_str())) {
                    // Handle item use
                    if (item == "Potion") {
                        if (health) {
                            health->currentHealth += 35; // Limit heal to full 
                            if (health->currentHealth > health->maxHealth) 
                                health->currentHealth = health->maxHealth; 
                        }
                        // Remove potion from inventory and adjust index
                        inventory->items.erase(inventory->items.begin() + i);
                        --i;
                    }
                }
            }
        }
    }
    else {
        ImGui::Text("Inventory: N/A");
    }
    ImGui::End();
}
