#pragma once

#include "graphics/texture.hpp"
#include "graphics/mesh2D.hpp"
#include "graphics/shader.hpp"
#include "graphics/sprite.hpp"
#include <graphics/model.hpp>

// std
#include <map>
#include <string>
#include <memory>

class ResourceManager {

public:
    struct Character {
        std::shared_ptr<Sprite> sprite;
        float advance;
        glm::vec2 size;
        glm::vec2 bearing;
    };

    void LoadResources();
    void LoadTexture(const std::string& name, const std::string& path);
    std::shared_ptr<Texture> LoadModelTexture(const std::string& path, bool srgb);
    void LoadMesh2D(const std::string& name, const std::shared_ptr<Mesh2D>& mesh2D);
    void LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    void LoadSprite(const std::string& name,
                    const std::shared_ptr<Mesh2D>& mesh2D,
                    const std::shared_ptr<Texture>& texture);

    void LoadFont(const std::string& path);
    void LoadModel(const std::string& name, const std::string& path, bool buffer);

    bool LoadBufferedModels();

    void RenderText(std::string text, Transform2D transform, glm::vec3 color, float alpha, float& length);

    std::shared_ptr<Mesh2D> GetMesh2D(const std::string& name) {
        return mMesh2DMap[name];
    }

    std::shared_ptr<Texture> GetTexture(const std::string& name) {
        return mTextureMap[name];
    }

    std::shared_ptr<Shader> GetShader(const std::string& name) {
        return mShaderMap[name];
    }

    std::shared_ptr<Sprite> GetSprite(const std::string& name) {
        return mSpriteMap[name];
    }

    std::shared_ptr<Model> GetModel(const std::string& name) {
        return mModelMap[name];
    }

private:
    std::map<std::string, std::shared_ptr<Texture>> mTextureMap;
    std::map<std::string, std::shared_ptr<Mesh2D>> mMesh2DMap;
    std::map<std::string, std::shared_ptr<Shader>> mShaderMap;
    std::map<std::string, std::shared_ptr<Sprite>> mSpriteMap;
    std::map<std::string, std::shared_ptr<Model>> mModelMap;

    std::vector<std::pair<std::string, std::shared_ptr<Model>>> mBufferedModels;

    std::vector<std::shared_ptr<Texture>> mModelTextureVec;
    std::vector<std::string> mTexturePathBuffer;

    std::map<std::string, Character> mCharacterMap;
};

extern std::unique_ptr<ResourceManager> gResourceManager;
