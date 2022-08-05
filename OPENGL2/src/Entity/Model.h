#pragma once
#include <material.h>
#include <scene.h>
#include <string>
#include <vector>
#include "Entity.h"
#include "Mesh.h"
#include "glew.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/VertexArray.h"
struct Texture;
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
    std::vector<Entity*> transforms;

    std::vector<float> radiusOffsetY;

    Model(std::string& path, class Renderer* renderer, int amount,bool isPrefab = false )
    {
        this->renderer = renderer;
        _isPrefab = isPrefab;
        loadModel(path);
        currentAngle = new float[amount];
        for (int i = 0; i< amount; i++)
        {
            radiusOffsetXZ.emplace_back(0);
            radiusOffsetY.emplace_back(lerp(-2,2, renderer->GetRandomNumber()));
        }
        for (int i =0; i< amount; i++)
        {
            currentAngle[i] = PI * 2 * renderer->GetRandomNumber();
            Entity* en = new Entity;
            en->SetRotation(360 * renderer->GetRandomNumber(), 360 * renderer->GetRandomNumber(), 360 * renderer->GetRandomNumber());
            en->ComputeWorldTransform();
            transforms.emplace_back(en);
        }

        
        glGenBuffers(1, &posBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            unsigned int VAO = meshes[i]->GetVertexArray()->GetVertexArrayId();
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }

		
    }

    void Draw(class Shader& shader);
	void DrawInstance(class Shader& shader);
	void SetTexture(std::string& path, std::string& type);
	void UpdateSelfRot(float time);
    void UpdateSelfRot(float time, glm::mat4 worldMat);
    void UpdatePosition(float time, glm::mat4 worldMat, glm::vec3 pos);
	void Update(float time);
private:
    // model data
    std::vector<class Mesh*> meshes;
    class Renderer* renderer;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
   std:: vector<struct Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
	
	unsigned int LoadTexture(std::string fileName);
};

