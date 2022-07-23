#pragma once
#include <material.h>
#include <scene.h>
#include <string>
#include <vector>
#include "Entity.h"
#include "Mesh.h"
struct Texture;

class Model : public Entity
{
public:
	bool _isPrefab;

    Model(std::string& path, class Renderer* renderer, bool isPrefab = false)
    {
        this->renderer = renderer;
        _isPrefab = isPrefab;
        loadModel(path);
    }

    void Draw(class Shader& shader);
    void SetTexture(std::string& path, std::string& type);
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

