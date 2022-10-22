#include <functional>
#include "model.hpp"
#include "stb_image.h"
#include <unordered_map>

using Assimp::Importer;
using std::cout;
using std::endl;
using std::string;
using std::vector;

// std::unordered_map<std::string, std::reference_wrapper<Texture>> loaded_textures;
vector<Texture> textures_loaded;	

Model::Model(char const *path) {
    stbi_set_flip_vertically_on_load(true);
    loadModel(path);
}

void Model::Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(string path) {
    Importer importer;
    aiScene const *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP" << importer.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, aiScene const *scene) {
    // first, process the mesh in the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // next, recursively process the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, aiScene const *scene) const {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    // convert Assimp data structure to own data structure
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.Position = glm::vec3 { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3 { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        }
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2 { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y }; 
            // vertex.Tangent = glm::vec3 { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
            // vertex.Bitangent = glm::vec3 { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
        } else {
            vertex.TexCoords = glm::vec2 { 0.0f, 0.0f };
        }
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    
    return Mesh { vertices, indices, textures };
}

vector<Texture> Model::loadMaterialTexture(aiMaterial *material, aiTextureType textureType, std::string type) const {
    vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(textureType); i++)
    {
        // aiString filePath;
        // material->GetTexture(textureType, i, &filePath);
        // char const *path = filePath.C_Str();

        // if (loaded_textures.contains(path)) {
        //     auto texture = loaded_textures.at(path);
        //     textures.push_back(texture);
        // }
        // else {
        //     Texture texture;
        //     texture.Id = loadTextureFromFile(path, directory);
        //     texture.Type = type;
        //     texture.Path = path;
        //     textures.push_back(texture);
        //     loaded_textures.insert(std::make_pair(texture.Path, std::ref(texture)));
        // }

        aiString str;
        material->GetTexture(textureType, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].Path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.Id = loadTextureFromFile(str.C_Str(), this->directory);
            texture.Type = type;
            texture.Path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }

    return textures;
}

unsigned int Model::loadTextureFromFile(char const *path, string const &directory) const {
    unsigned int textureId = 0;
    int width, height, numOfColorChannel;
    string fullPath = directory + '/' + path;
    unsigned char *imageData = stbi_load(fullPath.c_str(), &width, &height, &numOfColorChannel, 0);

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