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

class Model : public Entity
{
public:
	bool _isPrefab;
	float radiusSatelite;
	float speedSatelite;
	class Sphere* owner;
	float selfRotationSpeed;
    float* currentAngle;
    std::vector<float>radiusOffset;
    Model(std::string& path, class Renderer* renderer, std::vector<Entity*>&transforms,bool isPrefab = false )
    {
        int amount = transforms.size();
        this->renderer = renderer;
        _isPrefab = isPrefab;
        std::vector<glm::mat4> matricies;
        loadModel(path);
        currentAngle = new float[amount];
        for (int i = 0; i< amount; i++)
        {
            radiusOffset.emplace_back(0);
        }
        for (int i =0; i< amount; i++)
        {
            currentAngle[i] = PI * 2 * renderer->GetRandomNumber();
            matricies.emplace_back(transforms[i]->GetWorldMatrix());
        }
        
    
      
        //
        //unsigned int buffer;
        //glGenBuffers(1, &buffer);
        //glBindBuffer(GL_ARRAY_BUFFER, buffer);
        //glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &matricies[0], GL_STATIC_DRAW);

        //// set transformation matrices as an instance vertex attribute (with divisor 1)
        //// note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
        //// normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
        //// -----------------------------------------------------------------------------------------------------------------------------------
        //for (unsigned int i = 0; i < meshes.size(); i++)
        //{
        //    unsigned int VAO = meshes[i]->GetVertexArray()->GetVertexArrayId();
        //    glBindVertexArray(VAO);
        //    // set attribute pointers for matrix (4 times vec4)
        //    glEnableVertexAttribArray(3);
        //    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        //    glEnableVertexAttribArray(4);
        //    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        //    glEnableVertexAttribArray(5);
        //    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        //    glEnableVertexAttribArray(6);
        //    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        //    glVertexAttribDivisor(3, 1);
        //    glVertexAttribDivisor(4, 1);
        //    glVertexAttribDivisor(5, 1);
        //    glVertexAttribDivisor(6, 1);

        //    glBindVertexArray(0);
        //}
		
    }

    void Draw(class Shader& shader);
	void DrawInstance(class Shader& shader, std::vector<class Entity*>& transforms);
	void SetTexture(std::string& path, std::string& type);
	void UpdateSelfRot(float time);
    void UpdateSelfRot(float time, glm::mat4 worldMat);
    void UpdatePosition(float time, glm::mat4 worldMat, glm::vec3 pos);
	void Update(float time, std::vector<class Entity*>& transforms);
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

