#include <graphics/model.hpp>
#include <iostream>
#include <utils/constants.hpp>
#include <utils/resource_manager.hpp>

#include <glad/glad.h>

Model::Model(const std::string& filePath) {
    LoadModel(filePath);
}

void Model::Render(const std::shared_ptr<Shader>& shader,
                   glm::mat4 viewMatrix,
                   Transform3D transform,
                   glm::vec3 cameraPosition,
                   Light light,
                   float opacity) {
    glStencilMask(0x00);
    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), LOGIC_WIDTH / LOGIC_HEIGHT, 0.1f, 100.f);

    glm::mat4 model = transform.GetModelMatrix();

    shader->SetUniform("model", model);
    shader->SetUniform("projection", projection);
    shader->SetUniform("view", viewMatrix);

    shader->SetUniform("opacity", opacity);

    shader->SetUniform("light.direction", light.direction);
    shader->SetUniform("viewPosition", cameraPosition);

    shader->SetUniform("light.ambient", light.ambientStrength);
    shader->SetUniform("light.diffuse", light.diffuseStrength);
    shader->SetUniform("light.specular", light.specularStrength);

    shader->SetUniform("material.shininess", 32.0f);


    for (unsigned int i = 0; i < mMeshVec.size(); i++) {
        Mesh3D *mesh = mMeshVec[i];

        if (mDiffuseMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 0);
            shader->SetUniform("material.diffuse", 0);
            mDiffuseMapVec[i]->Bind();
        }
        
        if (mSpecularMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 1);
            shader->SetUniform("material.specular", 1);
            mSpecularMapVec[i]->Bind();
        }

        if (mHeightMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 2);
            shader->SetUniform("material.height", 2);
            mHeightMapVec[i]->Bind();
        }

        mesh->BindVAO();

        glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
        
        mesh->UnbindVAO();

        // unbind textures
        if (mDiffuseMapVec[i] != nullptr)
            mDiffuseMapVec[i]->Unbind();

        if (mSpecularMapVec[i] != nullptr)
            mSpecularMapVec[i]->Unbind();

        if (mHeightMapVec[i] != nullptr)
            mHeightMapVec[i]->Unbind();

        glActiveTexture(GL_TEXTURE0);
    }
}

void Model::RenderStencil(const std::shared_ptr<Shader>& shader,
                            glm::mat4 viewMatrix,
                            Transform3D transform,
                            glm::vec3 cameraPosition,
                            Light light,
                            int hp,
                            int shouldDeform) {
    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), LOGIC_WIDTH / LOGIC_HEIGHT, 0.1f, 100.f);

    glm::mat4 model = transform.GetModelMatrix();

    shader->SetUniform("model", model);
    shader->SetUniform("projection", projection);
    shader->SetUniform("view", viewMatrix);

    shader->SetUniform("light.direction", light.direction);
    shader->SetUniform("viewPosition", cameraPosition);

    shader->SetUniform("light.ambient", light.ambientStrength);
    shader->SetUniform("light.diffuse", light.diffuseStrength);
    shader->SetUniform("light.specular", light.specularStrength);

    shader->SetUniform("material.shininess", 32.0f);
    shader->SetUniform("opacity", 1.0f);
    shader->SetUniform("hp", hp);
    shader->SetUniform("shouldDeform", shouldDeform);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    for (unsigned int i = 0; i < mMeshVec.size(); i++) {
        Mesh3D *mesh = mMeshVec[i];

        if (mDiffuseMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 0);
            shader->SetUniform("material.diffuse", 0);
            mDiffuseMapVec[i]->Bind();
        }
        
        if (mSpecularMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 1);
            shader->SetUniform("material.specular", 1);
            mSpecularMapVec[i]->Bind();
        }

        if (mHeightMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 2);
            shader->SetUniform("material.height", 2);
            mHeightMapVec[i]->Bind();
        }

        mesh->BindVAO();

        glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
        
        mesh->UnbindVAO();

        // unbind textures
        if (mDiffuseMapVec[i] != nullptr)
            mDiffuseMapVec[i]->Unbind();

        if (mSpecularMapVec[i] != nullptr)
            mSpecularMapVec[i]->Unbind();

        if (mHeightMapVec[i] != nullptr)
            mHeightMapVec[i]->Unbind();

        glActiveTexture(GL_TEXTURE0);
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    auto stencilShader = gResourceManager->GetShader("stencil_shader");
    stencilShader->Use();

    stencilShader->SetUniform("model", model);
    stencilShader->SetUniform("projection", projection);
    stencilShader->SetUniform("view", viewMatrix);
    stencilShader->SetUniform("hp", hp);
    stencilShader->SetUniform("shouldDeform", shouldDeform);


    for (unsigned int i = 0; i < mMeshVec.size(); i++) {
        Mesh3D *mesh = mMeshVec[i];

        mesh->BindVAO();

        glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
        
        mesh->UnbindVAO();
    }

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

void Model::RenderInstanced(const std::shared_ptr<Shader>& shader,
                            glm::mat4 viewMatrix,
                            Transform3D transform,
                            glm::vec3 cameraPosition,
                            Light light,
                            int count) {
    glStencilMask(0x00);
    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), LOGIC_WIDTH / LOGIC_HEIGHT, 0.1f, 100.f);

    glm::mat4 model = transform.GetModelMatrix();

    shader->SetUniform("model", model);
    shader->SetUniform("projection", projection);
    shader->SetUniform("view", viewMatrix);

    shader->SetUniform("light.direction", light.direction);
    shader->SetUniform("viewPosition", cameraPosition);

    shader->SetUniform("light.ambient", light.ambientStrength);
    shader->SetUniform("light.diffuse", light.diffuseStrength);
    shader->SetUniform("light.specular", light.specularStrength);

    shader->SetUniform("material.shininess", 32.0f);
    shader->SetUniform("opacity", 1.0f);


    for (unsigned int i = 0; i < mMeshVec.size(); i++) {
        Mesh3D *mesh = mMeshVec[i];

        if (mDiffuseMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 0);
            shader->SetUniform("material.diffuse", 0);
            mDiffuseMapVec[i]->Bind();
        }
        
        if (mSpecularMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 1);
            shader->SetUniform("material.specular", 1);
            mSpecularMapVec[i]->Bind();
        }

        if (mHeightMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 2);
            shader->SetUniform("material.height", 2);
            mHeightMapVec[i]->Bind();
        }

        mesh->BindVAO();

        glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0, count);
        
        mesh->UnbindVAO();

        // unbind textures
        if (mDiffuseMapVec[i] != nullptr)
            mDiffuseMapVec[i]->Unbind();

        if (mSpecularMapVec[i] != nullptr)
            mSpecularMapVec[i]->Unbind();

        if (mHeightMapVec[i] != nullptr)
            mHeightMapVec[i]->Unbind();

        glActiveTexture(GL_TEXTURE0);
    }
}

void Model::RenderInstancedParticles(const std::shared_ptr<Shader>& shader,
                                     glm::mat4 viewMatrix,
                                     Transform3D transform,
                                     glm::vec3 cameraPosition,
                                     Light light,
                                     std::vector<Particle>& particles) {
    glStencilMask(0x00);
    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), LOGIC_WIDTH / LOGIC_HEIGHT, 0.1f, 100.f);

    glm::mat4 model = transform.GetModelMatrix();

    shader->SetUniform("model", model);
    shader->SetUniform("projection", projection);
    shader->SetUniform("view", viewMatrix);

    shader->SetUniform("light.direction", light.direction);
    shader->SetUniform("viewPosition", cameraPosition);

    shader->SetUniform("light.ambient", light.ambientStrength);
    shader->SetUniform("light.diffuse", light.diffuseStrength);
    shader->SetUniform("light.specular", light.specularStrength);

    shader->SetUniform("material.shininess", 32.0f);
    shader->SetUniform("opacity", 1.0f);


    for (unsigned int i = 0; i < particles.size(); i++) {
        shader->SetUniform("particles[" + std::to_string(i) + "].offset", particles[i].offset);
        shader->SetUniform("particles[" + std::to_string(i) + "].opacity", particles[i].opacity);
    }


    for (unsigned int i = 0; i < mMeshVec.size(); i++) {
        Mesh3D *mesh = mMeshVec[i];

        if (mDiffuseMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 0);
            shader->SetUniform("material.diffuse", 0);
            mDiffuseMapVec[i]->Bind();
        }
        
        if (mSpecularMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 1);
            shader->SetUniform("material.specular", 1);
            mSpecularMapVec[i]->Bind();
        }

        if (mHeightMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 2);
            shader->SetUniform("material.height", 2);
            mHeightMapVec[i]->Bind();
        }

        mesh->BindVAO();

        glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0, particles.size());
        
        mesh->UnbindVAO();

        // unbind textures
        if (mDiffuseMapVec[i] != nullptr)
            mDiffuseMapVec[i]->Unbind();

        if (mSpecularMapVec[i] != nullptr)
            mSpecularMapVec[i]->Unbind();

        if (mHeightMapVec[i] != nullptr)
            mHeightMapVec[i]->Unbind();

        glActiveTexture(GL_TEXTURE0);
    }
}

void Model::Render(const std::shared_ptr<Shader>& shader,
                   glm::mat4 viewMatrix,
                   Transform3D transform,
                   glm::vec3 cameraPosition,
                   Light light,
                   float opacity,
                   int hp,
                   int shouldDeform) {
    glStencilMask(0x00);
    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), LOGIC_WIDTH / LOGIC_HEIGHT, 0.1f, 100.f);

    glm::mat4 model = transform.GetModelMatrix();

    shader->SetUniform("model", model);
    shader->SetUniform("projection", projection);
    shader->SetUniform("view", viewMatrix);
    shader->SetUniform("hp", hp);

    shader->SetUniform("opacity", opacity);

    shader->SetUniform("light.direction", light.direction);
    shader->SetUniform("viewPosition", cameraPosition);

    shader->SetUniform("light.ambient", light.ambientStrength);
    shader->SetUniform("light.diffuse", light.diffuseStrength);
    shader->SetUniform("light.specular", light.specularStrength);
    shader->SetUniform("shouldDeform", shouldDeform);

    shader->SetUniform("material.shininess", 32.0f);

    for (unsigned int i = 0; i < mMeshVec.size(); i++) {
        Mesh3D *mesh = mMeshVec[i];

        if (mDiffuseMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 0);
            shader->SetUniform("material.diffuse", 0);
            mDiffuseMapVec[i]->Bind();
        }
        
        if (mSpecularMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 1);
            shader->SetUniform("material.specular", 1);
            mSpecularMapVec[i]->Bind();
        }

        if (mHeightMapVec[i] != nullptr) {
            glActiveTexture(GL_TEXTURE0 + 2);
            shader->SetUniform("material.height", 2);
            mHeightMapVec[i]->Bind();
        }

        mesh->BindVAO();

        glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0);
        
        mesh->UnbindVAO();

        // unbind textures
        if (mDiffuseMapVec[i] != nullptr)
            mDiffuseMapVec[i]->Unbind();

        if (mSpecularMapVec[i] != nullptr)
            mSpecularMapVec[i]->Unbind();

        if (mHeightMapVec[i] != nullptr)
            mHeightMapVec[i]->Unbind();

        glActiveTexture(GL_TEXTURE0);
    }
}

void Model::LoadModel(const std::string& filePath) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(filePath,
                                            aiProcess_Triangulate |
                                            aiProcess_FlipUVs |
                                            aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp failed to load model: " << import.GetErrorString() << "\n";
        exit(EXIT_FAILURE);
    }

    directory = filePath.substr(0, filePath.find_last_of('/'));
    directory += '/';

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshVec.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh3D *Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Mesh3D::Vertex> vertices;
    std::vector<unsigned int> indices;

    // get vertices from assimp mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position = glm::vec3(mesh->mVertices[i].x,
                                       mesh->mVertices[i].y,
                                       mesh->mVertices[i].z);

        glm::vec3 normal = glm::vec3(mesh->mNormals[i].x,
                                     mesh->mNormals[i].y,
                                     mesh->mNormals[i].z);

        glm::vec2 texCoords = glm::vec2(0.0f, 0.0f);

        // check if mesh contains tex coords
        if (mesh->mTextureCoords[0]) {
            texCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                  mesh->mTextureCoords[0][i].y);
        }

        glm::vec3 tangent = glm::vec3(mesh->mTangents[i].x,
                                      mesh->mTangents[i].y,
                                      mesh->mTangents[i].z);

        vertices.push_back(Mesh3D::Vertex(position, normal, texCoords, tangent));
    }

    // get indices from assimp mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<std::shared_ptr<Texture>> diffuseMaps;

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

            mDiffuseMapVec.push_back(gResourceManager->LoadModelTexture(directory + std::string(path.C_Str()), true));
        } else {
            mDiffuseMapVec.push_back(nullptr);
        }

        if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            aiString path;
            material->GetTexture(aiTextureType_SPECULAR, 0, &path);

            mSpecularMapVec.push_back(gResourceManager->LoadModelTexture(directory + std::string(path.C_Str()), false));
        } else if (material->GetTextureCount(aiTextureType_UNKNOWN) > 0) {
            aiString path;
            material->GetTexture(aiTextureType_UNKNOWN, 0, &path);

            mSpecularMapVec.push_back(gResourceManager->LoadModelTexture(directory + std::string(path.C_Str()), false));
        } else {
            mSpecularMapVec.push_back(nullptr);
        }

        if (material->GetTextureCount(aiTextureType_NORMALS) > 0) {
            aiString path;
            material->GetTexture(aiTextureType_NORMALS, 0, &path);

            std::cout << std::string(path.C_Str()) << std::endl;

            mHeightMapVec.push_back(gResourceManager->LoadModelTexture(directory + std::string(path.C_Str()), false));
        } else {
            mHeightMapVec.push_back(nullptr);
        }
    }

    return new Mesh3D(vertices, indices, true);
}