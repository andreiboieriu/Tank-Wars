#pragma once

// std
#include <string>
#include <glad/glad.h>

class Texture {

public:
    struct TextureOptions {
        unsigned int internalFormat;
        unsigned int imageFormat;
        unsigned int wrapS;
        unsigned int wrapT;
        unsigned int filterMin;
        unsigned int filterMax;
    };

    struct StbiTextureBuffer {
        int width;
        int height;
        int nrChannels;
        unsigned char *data;
    };

    Texture(const std::string& filePath, bool buffer, bool srgb);
    Texture(unsigned int width, unsigned int height, const unsigned char* data, TextureOptions options);

    void LoadFromBuffer();

    void Bind() const;
    void Unbind() const;

    Texture() {}
    ~Texture() {}

    std::string getPath() {
        return filePath;
    }

    bool GetLoadStatus() {
        return mDoneLoading;
    }
    
private:
    void Generate(unsigned int width, unsigned int height, const unsigned char* data);
    void Generate(unsigned int width, unsigned int height, const unsigned char* data, TextureOptions options);


    std::string filePath;

    unsigned int mId = 0;
    unsigned int mWidth = 0;
    unsigned int mHeight = 0;

    bool mDoneLoading = false;
    bool mSrgb;

    TextureOptions mTextureOptions;
    StbiTextureBuffer mBuffer;
};