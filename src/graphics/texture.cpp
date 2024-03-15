#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <glad/glad.h>

// std
#include <string>
#include <iostream>

Texture::Texture(unsigned int width, unsigned int height, const unsigned char* data, TextureOptions options) {
    glGenTextures(1, &mId);

    Generate(width, height, data, options);
}

Texture::Texture(const std::string& filePath, bool buffer, bool srgb) {
    this->filePath = filePath;
    mSrgb = srgb;

    mBuffer.data = stbi_load(filePath.c_str(), &mBuffer.width, &mBuffer.height, &mBuffer.nrChannels, 0);

    if (buffer)
        return;

    LoadFromBuffer();
}

void Texture::LoadFromBuffer() {
    Generate(mBuffer.width, mBuffer.height, mBuffer.data);
    stbi_image_free(mBuffer.data);

    mDoneLoading = true;
}

void Texture::Generate(unsigned int width, unsigned int height, const unsigned char* data) {
    if (mBuffer.nrChannels == 1) {
        mTextureOptions.imageFormat = GL_RED;
        mTextureOptions.internalFormat = GL_RED;
    } else if (mBuffer.nrChannels == 3) {
        mTextureOptions.imageFormat = GL_RGB;

        if (mSrgb)
            mTextureOptions.internalFormat = GL_SRGB;
        else
            mTextureOptions.internalFormat = GL_RGB;
    } else if (mBuffer.nrChannels == 4) {

        if (mSrgb) {
            mTextureOptions.imageFormat = GL_SRGB_ALPHA;
            mTextureOptions.internalFormat = GL_SRGB_ALPHA;
        } else {
            mTextureOptions.imageFormat = GL_RGBA;
            mTextureOptions.internalFormat = GL_RGBA;
        }
        
    }

    // default options
    mTextureOptions.wrapS = GL_REPEAT;
    mTextureOptions.wrapT = GL_REPEAT;
    mTextureOptions.filterMin = GL_LINEAR_MIPMAP_LINEAR;
    mTextureOptions.filterMax = GL_LINEAR;

    mWidth = width;
    mHeight = height;

    glGenTextures(1, &mId);

    // create and bind texture
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, mTextureOptions.internalFormat, mWidth, mHeight, 0, mTextureOptions.imageFormat, GL_UNSIGNED_BYTE, data);

    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mTextureOptions.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mTextureOptions.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mTextureOptions.filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mTextureOptions.filterMax);

    glGenerateMipmap(GL_TEXTURE_2D);

    // unbind texture
    Unbind();
    mDoneLoading = true;
}

void Texture::Generate(unsigned int width, unsigned int height, const unsigned char* data, TextureOptions options) {
    mTextureOptions = options;
    mWidth = width;
    mHeight = height;

    glGenTextures(1, &mId);

    // create and bind texture
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, mTextureOptions.internalFormat, mWidth, mHeight, 0, mTextureOptions.imageFormat, GL_UNSIGNED_BYTE, data);

    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mTextureOptions.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mTextureOptions.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mTextureOptions.filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mTextureOptions.filterMax);

    glGenerateMipmap(GL_TEXTURE_2D);
    // unbind texture
    Unbind();
    mDoneLoading = true;
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, mId);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
