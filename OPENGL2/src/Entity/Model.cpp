#include "Model.h"
#include "Mesh.h"
#include <Importer.hpp>
#include <iostream>
#include <scene.h>
#include <postprocess.h>
#include <SOIL.h>
#include <glew.h>

#include "Camera.h"
#include "../Rendering/Debuger.h"
#include "../Rendering/Renderer.h"
#include "Sphere.h"
#include "../Rendering/Shader.h"

void Model::Draw(Shader& shader)
{
    

     for (unsigned int i = 0; i < meshes.size(); i++)
     {
         ComputeWorldTransform();
         meshes[i]->ComputeWorldTransform();
         meshes[i]->Draw(shader, *this);
     }
}

void UpdateMatrix(glm::mat4* matrix, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{

}

void Model::DrawInstance(Shader& shader,std::vector<Entity*>& transforms)
{
    shader.Bind();
   
    shader.SetMatrixUniform("projMatrix", renderer->GetPerspectiveMatrix());
    // camera/view transformation
    shader.SetMatrixUniform("viewMatrix", renderer->GetCamera()->GetViewMatrix());
    for (unsigned int i = 0; i < transforms.size(); i++)
    {
        for (unsigned int j = 0; j < meshes.size(); j++)
        {
            shader.SetMatrixUniform("worldMatrix", transforms[i]->GetWorldMatrix());
            meshes[j]->Draw(shader, *this);
        }
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto meshToAdd = processMesh(mesh, scene);
        meshes.push_back(meshToAdd);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;


        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
	if (mesh->mMaterialIndex >= 0 && !_isPrefab)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return new  Mesh(vertices, indices, textures, renderer);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = LoadTexture(directory + '/' + str.C_Str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

void Model::SetTexture(std::string& path, std::string& type)
{
    Texture texture;
    texture.id = LoadTexture(path);
    texture.type = type;
    texture.path = path;
   // textures.push_back(texture);
    textures_loaded.push_back(texture); // add to loaded textures
}


void Model::UpdateSelfRot(float time)
{
    SetRotation(GetRotation().x + time * selfRotationSpeed, GetRotation().y, GetRotation().z);
}
void Model::UpdateSelfRot(float time,glm::mat4 worldMat )
{
    SetRotation(GetRotation().x + time * selfRotationSpeed, GetRotation().y, GetRotation().z);
}

void Model::UpdatePosition(float time, glm::mat4 worldMat, glm::vec3 pos)
{
}

void Model::Update(float time, std::vector< Entity*>& transforms)
{

    for (int i = 0; i < transforms.size(); i++)
    {


        // calculate pos
        // !
        // calculate offset
        // !
        //UpdateSatellites(time);
        if (radiusSatelite > 0.1f)
        {
            auto omega = speedSatelite / radiusSatelite;
            auto xSpeed = (radiusSatelite+radiusOffset[i]) * sin(currentAngle[i] + omega * time);
            auto zSpeed = (radiusSatelite+radiusOffset[i]) * cos(currentAngle[i] + omega * time);
            currentAngle[i] = currentAngle[i] + omega * time;
            auto x = xSpeed;
            auto z = zSpeed;
            auto ownerPos = owner->GetPosition();
            auto  position = glm::vec3(x + ownerPos.x, transforms[i]->GetPosition().y, z + ownerPos.z);
            transforms[i]->SetPosition(position);
        }

        // calculate rot

        //SetRotation(GetRotation().x + time * selfRotationSpeed, GetRotation().y, GetRotation().z);
        glm::vec3 rotation =glm::vec3(0,0,0);
        transforms[i]->SetRotation(rotation);
        // calculate scale
        glm::vec3 scale = glm::vec3(0.2, 0.2, 0.2);
        transforms[i]->SetScale(scale);
        // update world matrix
        transforms[i]->ComputeWorldTransform();

    }
    
}

unsigned int Model::LoadTexture(std::string fileName)
{
    int channels = 0;

    int _width, _height;
    unsigned int _textureID;
    unsigned char* image = SOIL_load_image(
        fileName.c_str(),
        &_width,
        &_height,
        &channels,
        SOIL_LOAD_AUTO
    );
    if (image == nullptr)
    {
        std::cout << ("[SOIL] Failed to load image %s", fileName.c_str(), SOIL_last_result()) << "\n";
        return -1;
    }
    int format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }
    GLCall(glGenTextures(1, &_textureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));

    GLCall(glTexImage2D(
        GL_TEXTURE_2D, // Texture target
        0, // Level of detail (for now, assume 0)
        format, // Color format OpenGL should use
        _width, // Width of texture
        _height, // Height of texture
        0, // Border – "this value must be 0"
        format, // Color format of input data
        GL_UNSIGNED_BYTE,// Bit depth of input data
        // Unsigned byte specifies 8-bit channels
        image // Pointer to image data
    ));

    SOIL_free_image_data(image);

    // bilinear  filtering
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    return _textureID;
}
