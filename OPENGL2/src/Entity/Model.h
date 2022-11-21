#pragma once
#include <material.h>
#include <scene.h>
#include <string>
#include <vector>
#include "Entity.h"
#include "Mesh.h"
#include "glew.h"

#include "../Rendering/VertexArray.h"
struct Texture;
class Renderer;

#define PI 3.14159265358979323846

float lerp(float a, float b, float t);

class Model : public Entity
{
public:
	bool _isPrefab;
    unsigned int posBuffer;
	float radiusSatelite;
	float speedSatelite;
	class Sphere* owner;
	float selfRotationSpeed;
    float* currentAngle;
    std::vector<float>radiusOffsetXZ;
    std::vector<Entity> transforms;

    std::vector<float> radiusOffsetY;

    Model(std::string& path, class Renderer* renderer, int amount, bool isPrefab = false);

    void Draw(class Shader& shader, glm::mat4x4& viewMat, glm::mat4x4& projMat);
	void DrawInstance(class Shader& shader);
	void SetTexture(std::string& path, std::string& type);
	void UpdateSelfRot(float time);
    void UpdateSelfRot(float time, glm::mat4 worldMat);
    void UpdatePosition(float time, glm::mat4 worldMat, glm::vec3 pos);
	void Update(float time);
private:
    // model data
    std::vector<Mesh> meshes;
    Renderer* renderer;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
   std:: vector<struct Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
	
	unsigned int LoadTexture(std::string fileName);
};

