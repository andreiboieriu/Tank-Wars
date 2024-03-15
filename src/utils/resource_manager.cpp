#include <utils/resource_manager.hpp>

#include <graphics/sprite.hpp>
#include <utils/constants.hpp>
#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

// std
#include <memory>

void ResourceManager::LoadResources() {
    
}

void ResourceManager::LoadTexture(const std::string& name, const std::string& path) {
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(path, false, false);

    mTextureMap[name] = newTexture;
}

std::shared_ptr<Texture> ResourceManager::LoadModelTexture(const std::string& path, bool srgb) {
    for (auto& tex : mModelTextureVec) {
        if (!tex->getPath().compare(path)) {
            return tex;
        }
    }

    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(path, true, srgb);
    mModelTextureVec.push_back(newTexture);

    return newTexture;
}

void ResourceManager::LoadMesh2D(const std::string& name, const std::shared_ptr<Mesh2D>& mesh2D) {
    mMesh2DMap[name] = mesh2D;
}

void ResourceManager::LoadShader(const std::string& name,
                                 const std::string& vertexPath,
                                 const std::string& fragmentPath) {
    std::shared_ptr<Shader> newShader = std::make_shared<Shader>(vertexPath, fragmentPath);

    mShaderMap[name] = newShader;
}

void ResourceManager::LoadSprite(const std::string& name,
                                 const std::shared_ptr<Mesh2D>& mesh2D,
                                 const std::shared_ptr<Texture>& texture) {
    std::shared_ptr<Sprite> newSprite = std::make_shared<Sprite>(mesh2D, texture);
    if (texture == nullptr) {
        std::cout << "sug pula" << std::endl;
    } else if (mesh2D == nullptr) {
        std::cout << "sug penis" << std::endl;
    }
    mSpriteMap[name] = newSprite;
}

void ResourceManager::LoadModel(const std::string& name, const std::string& path, bool buffer) {
    std::shared_ptr<Model> newModel = std::make_shared<Model>(path);


    if (buffer) {
        mBufferedModels.push_back(std::make_pair(name, newModel));
    } else {
        newModel->LoadFromBuffer(true);
        mModelMap[name] = newModel;
    }
}

bool ResourceManager::LoadBufferedModels() {
    auto it = mBufferedModels.begin();

    while (it != mBufferedModels.end()) {
        auto modelPair = *it;

        if (!modelPair.second->LoadFromBuffer(false)) {
            mModelMap[modelPair.first] = modelPair.second;
            std::cout << "Finished loading " << modelPair.first << std::endl;
            
            it = mBufferedModels.erase(it);
        }

        return true;
    }

    return false;
}

void ResourceManager::LoadFont(const std::string& path) {
    // init free type library
    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "Failed to init FreeType library\n";
        exit(EXIT_FAILURE);
    }

    // load font
    FT_Face face;
    if (FT_New_Face(ft, path.c_str(), 0, &face)) {
        std::cerr << "Failed to load font\n";
        exit(EXIT_FAILURE);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 chars in ASCII
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load characters\n";
            exit(EXIT_FAILURE);
        }

        Texture::TextureOptions options;
        options.imageFormat = GL_RED;
        options.internalFormat = GL_RED;
        options.wrapS = GL_CLAMP_TO_EDGE;
        options.wrapT = GL_CLAMP_TO_EDGE;
        options.filterMin = GL_LINEAR;
        options.filterMax = GL_LINEAR;

        // create character texture
        std::shared_ptr<Texture> charTex =
        std::make_shared<Texture>(face->glyph->bitmap.width,
                                  face->glyph->bitmap.rows,
                                  face->glyph->bitmap.buffer,
                                  options);

        std::string character{};
        character += c;

        mTextureMap["char_" + character] = charTex;

        glm::vec2 size = glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        glm::vec2 bearing = glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);

        std::vector<Mesh2D::Vertex> vertices = {
            Mesh2D::Vertex(glm::vec2(bearing.x, bearing.y), glm::vec2(0.0f, 0.0f)),
            Mesh2D::Vertex(glm::vec2(bearing.x, bearing.y - size.y), glm::vec2(0.0f, 1.0f)),
            Mesh2D::Vertex(glm::vec2(bearing.x + size.x, bearing.y - size.y), glm::vec2(1.0f, 1.0f)),
            Mesh2D::Vertex(glm::vec2(bearing.x + size.x, bearing.y), glm::vec2(1.0f, 0.0f))
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0
        };

        std::shared_ptr<Mesh2D> charMesh2D = std::make_shared<Mesh2D>(vertices, indices);
        mMesh2DMap["char_" + character] = charMesh2D;

        LoadSprite("char_" + character, mMesh2DMap["char_" + character], mTextureMap["char_" + character]);

        Character newCharacter{};
        newCharacter.sprite = gResourceManager->GetSprite("char_" + character);
        newCharacter.advance = (face->glyph->advance.x >> 6);
        newCharacter.size = size;
        newCharacter.bearing = bearing;

        mCharacterMap["char_" + character] = newCharacter;
    }

    // destroy freetype vars
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void ResourceManager::RenderText(std::string text, Transform2D transform, glm::vec3 color, float alpha, float& length) {
    std::shared_ptr<Shader> textShader = GetShader("text_shader");

    length = 0;
    float maxHeightUp = -100.f;
    float maxHeightDown = -100.f;

    // center text
    for (auto& c : text) {
        std::string characterStr = "char_";
        characterStr += c;

        Character character = mCharacterMap[characterStr];

        length += character.advance * transform.scale.x;

        if (character.bearing.y > maxHeightUp) {
            maxHeightUp = character.bearing.y;
        }

        if (character.size.y - character.bearing.y > maxHeightDown) {
            maxHeightDown = character.size.y - character.bearing.y;
        }
    }

    float height = maxHeightDown + maxHeightUp;

    transform.position.x -= length / 2;
    transform.position.y -= height / 2;

    for (auto& c : text) {
        std::string characterStr = "char_";
        characterStr += c;

        Character character = mCharacterMap[characterStr];

        character.sprite->Render(textShader, transform, color, alpha);

        transform.position.x += character.advance * transform.scale.x;
    }
}
