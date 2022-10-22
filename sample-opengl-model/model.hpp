#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <iostream>
#include "mesh.hpp"
#include <vector>


class Model {
    private:
        std::vector<Mesh> meshes;
        std::string directory;
        
        void loadModel(std::string path);
        void processNode(aiNode *node, aiScene const *scene);
        Mesh processMesh(aiMesh *mesh, aiScene const *scene) const;
        std::vector<Texture> loadMaterialTexture(aiMaterial *material, aiTextureType textureType, std::string type) const;
        unsigned int loadTextureFromFile(char const *path, std::string const &directory) const;
    public:
        Model(char const *path);
        void Draw(Shader &shader);
};