#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

v::renderer::Texture::Texture(const char * image, const char * texType, GLuint slot) {
    type = texType;
    
    int widthImg, heightImg, numColChannel;

    stbi_set_flip_vertically_on_load(true);

    unsigned char * bytes = stbi_load(image, &widthImg, &heightImg, &numColChannel, 0);

    v::util::log("Trying to load texture...\n");

    // Rework error checking
    if(!bytes) {
        std::string a = "Faulty Path at texture constr: ";
        v::util::log((a + std::string(image) + '\n').c_str());
        throw std::invalid_argument("Wrong File path");
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    } else if(numColChannel == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    } else if(numColChannel == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
    } else {
        v::util::log("Wrong Color Channel Number at texture constr\n");
        throw std::invalid_argument("Wrong Color Channel Number.");
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