#include "Renderer.h"

void Renderer::initializeShaders() {
    std::string vertexShaderSource = ShaderHelper::readShader(VertexSource);
    std::string fragmentShaderSource = ShaderHelper::readShader(FragmentSource);

    // Debug: Print the shader source
    std::cout << "Vertex Shader Source:\n" << vertexShaderSource << std::endl;
    std::cout << "Fragment Shader Source:\n" << fragmentShaderSource << std::endl;

    shaderProgram = ShaderHelper::createProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

    if (shaderProgram == 0) {
        std::cerr << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n";
        return;  // Exit the function if the shader program failed to create
    }

    // Get uniform locations
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projLoc = glGetUniformLocation(shaderProgram, "projection");
    modelLoc = glGetUniformLocation(shaderProgram, "model");

    // Check if uniform locations are valid
    if (viewLoc == -1) {
        std::cerr << "Error: 'view' uniform location not found in shader program.\n";
    }
    if (projLoc == -1) {
        std::cerr << "Error: 'projection' uniform location not found in shader program.\n";
    }
    if (modelLoc == -1) {
        std::cerr << "Error: 'model' uniform location not found in shader program.\n";
    }
}


void Renderer::render(const std::shared_ptr<WorldObject>& worldObject, const Camera& camera) {
    glUseProgram(shaderProgram);
    updateUniforms(camera);
    /*
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.position));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
    */
    // Calculate and set the model matrix for the current object
    glm::mat4 modelMatrix = worldObject->getModelMatrix();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind the VAO and draw the object
    glBindVertexArray(worldObject->getGLObject(VAOtype));
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(worldObject->model.getIndices().size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::updateUniforms(const Camera& camera) {
    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
    glm::mat4 projection = glm::perspective(glm::radians(camera.FoV), SCR_WIDTH / SCR_HEIGHT, camera.nearClip, camera.farClip);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Renderer::cleanup() {
    glDeleteProgram(shaderProgram);
}
