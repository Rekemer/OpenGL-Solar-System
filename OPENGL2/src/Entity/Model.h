#pragma once
#include <material.h>
#include <scene.h>
#include <string>
#include <vector>
#include "Entity.h"
struct Texture;

class Model
{
public:
    
    Model(std::string& path, class Renderer* renderer)
    {
        this->renderer = renderer;
        loadModel(path);
    }

    static Model GenerateSphere(class Renderer* renderer);
    void Draw(class Shader& shader);
private:
    // model data
    std::vector<class Mesh> meshes;
    class Renderer* renderer;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
   std:: vector<struct Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
   unsigned int LoadTexture(std::string fileName);
};

