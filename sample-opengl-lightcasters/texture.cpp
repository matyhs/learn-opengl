#include <glad/glad.h>

#include <iostream>
using std::cout;
using std::endl;

#include "texture.hpp"

#include "stb_image.h"

Texture::Texture(char const *path, unsigned int unit) : textureUnit(unit) {
    textureId = LoadTexture(path);
}

int Texture::LoadTexture(char const *path) const {
    GLuint textureId = 0;
    int width, height, numOfColorChannel;
    unsigned char *imageData = stbi_load(path, &width, &height, &numOfColorChannel, 0);

    if (imageData) {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = GL_RGBA;
        if (numOfColorChannel == 1) {
            format = GL_RED;
        } else if (numOfColorChannel == 3) {
            format = GL_RGB;
        } else if (numOfColorChannel == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load image" << endl;
    }

    stbi_image_free(imageData);
    return textureId;
}