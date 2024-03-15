#pragma once

#include <graphics/mesh3D.hpp>
#include <graphics/shader.hpp>
#include <graphics/texture.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <utils/transform.hpp>
#include <graphics/light.hpp>
#include <entities/particle.hpp>

// std
#include <string>
#include <vector>

class Model {

public:
    Model(const std::string& filePath);

    std::vector<Mesh3D *> getMeshVec() {
        return mMeshVec;
    }

    ~Model() {
        for (auto& mesh : mMeshVec) {
            delete mesh;
        }
    }

    void Render(const std::shared_ptr<Shader>& shader,
                glm::mat4 viewMatrix,
                Transform3D transform,
                glm::vec3 cameraPosition,
                Light light,
                float opacity);

    void Render(const std::shared_ptr<Shader>& shader,
                   glm::mat4 viewMatrix,
                   Transform3D transform,
                   glm::vec3 cameraPosition,
                   Light light,
                   float opacity,
                   int hp,
                   int shouldDeform);

    void RenderStencil(const std::shared_ptr<Shader>& shader,
                            glm::mat4 viewMatrix,
                            Transform3D transform,
                            glm::vec3 cameraPosition,
                            Light light,
                            int hp,
                            int shouldDeform);

    void RenderInstanced(const std::shared_ptr<Shader>& shader,
                         glm::mat4 viewMatrix,
                         Transform3D transform,
                         glm::vec3 cameraPosition,
                         Light light,
                         int count);

    void RenderInstancedParticles(const std::shared_ptr<Shader>& shader,
                                  glm::mat4 viewMatrix,
                                  Transform3D transform,
                                  glm::vec3 cameraPosition,
                                  Light light,
                                  std::vector<Particle>& particles);

    bool LoadFromBuffer(bool allAtOnce) {
        if (allAtOnce) {
            while (LoadBufferedMeshes() || LoadBufferedTexture());

            return true;
        }

        if (LoadBufferedMeshes() || LoadBufferedTexture())
            return true;

        return false;
    }

private:

    bool LoadBufferedMeshes() {
        for (auto& mesh : mMeshVec) {
            if (mesh->GetLoadStatus())
                continue;

            mesh->Load();
            return true;
        }

        return false;
    }

    bool LoadBufferedTexture() {
        for (auto& texture : mDiffuseMapVec) {
            if (texture == nullptr)
                continue;

            if (texture->GetLoadStatus())
                continue;

            texture->LoadFromBuffer();
            return true;
        }

        for (auto& texture : mSpecularMapVec) {
            if (texture == nullptr)
                continue;

            if (texture->GetLoadStatus())
                continue;

            texture->LoadFromBuffer();
            return true;
        }

        for (auto& texture : mHeightMapVec) {
            if (texture == nullptr)
                continue;

            if (texture->GetLoadStatus())
                continue;

            texture->LoadFromBuffer();
            return true;
        }

        return false;
    }
    void LoadModel(const std::string& filePath);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh3D *ProcessMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Mesh3D *> mMeshVec;
    std::string directory;

    std::vector<std::shared_ptr<Texture>> mDiffuseMapVec;
    std::vector<std::shared_ptr<Texture>> mSpecularMapVec;
    std::vector<std::shared_ptr<Texture>> mHeightMapVec;
};
