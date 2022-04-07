#include "Texture.h"

v::renderer::Texture::Texture(const char * image, const char * texType, GLuint slot) {
    type = texType;
    
    int widthImg, heightImg, numColChannel;

    stbi_set_flip_vertically_on_load(true);

    unsigned char * bytes = stbi_load(image, &widthImg, &heightImg, &numColChannel, 0);

    v::util::log((std::string("Loading Texture: ") + image).c_str());

    // Rework error checking
    if(!bytes) {
        v::util::log((std::string("Failed to load Texture: ") + image).c_str());
        return;
    }

    glGenTextures(1, &ID);

    glActiveTexture(GL_TEXTURE0 + slot);

    unit = slot;

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    if(numColChannel == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    } else if(numColChannel == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    } else if(numColChannel == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
    } else {
        v::util::log((std::string("Wrong Color Channel Number at Texture: ") + image).c_str());
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void v::renderer::Texture::texUnit(Shader & shader, const char * uniform, GLuint unit) {
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);

    shader.Activate();

    glUniform1i(texUni, unit);
}

void v::renderer::Texture::Bind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void v::renderer::Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void v::renderer::Texture::Delete() {
    glDeleteTextures(1, &ID);
}
