#include "Skybox.h"

v::renderer::Skybox::Skybox() {}

void v::renderer::Skybox::Bind(std::string cubeMaps[6]) {
    glGenVertexArrays(1, &sVAO);
    glGenBuffers(1, &sVBO);
    glGenBuffers(1, &sEBO);

    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    

    glGenTextures(1, &cbTexture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cbTexture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(false);

    for(unsigned int i = 0; i < 6; i++) {
        int width, height, nrChannels;
        
        unsigned char * data = stbi_load(v::util::normalized_path(cubeMaps[i].c_str()).c_str(), &width, &height, &nrChannels, 0);
        
        v::util::log((std::string("Loading Skybox Texture: ") + cubeMaps[i]).c_str());

        if(data) {
           glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_SRGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
           );

           stbi_image_free(data);
        } else {
            v::util::log((std::string("Failed to load Skybox Texture: ") + cubeMaps[i]).c_str());
        }
    }
}

void v::renderer::Skybox::Draw(v::renderer::Shader & skyboxProgram, v::engine::EngineSettings & settings, v::renderer::Camera & camera) {
        glDepthFunc(GL_LEQUAL);

        skyboxProgram.Activate();

        glm::mat4 view = glm::mat4(1.0F);
        glm::mat4 projection = glm::mat4(1.0F);

        view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));

        projection = glm::perspective(glm::radians(settings.cameraFOVdegrees), (float)(settings.width) / (float)(settings.height), settings.cameraNearPlane, settings.cameraFarPlane);

        skyboxProgram.UniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
        skyboxProgram.UniformMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(sVAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cbTexture);
        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
}
