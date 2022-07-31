#include "Sphere.h"
#include "../TextureDefault.h"
#include "../Rendering/Shader.h"
#include "../Rendering/VertexArray.h"
#include "../Rendering/Debuger.h"
#include "../Rendering/Renderer.h"
#include "Camera.h"
#include "Model.h"
#define PI 3.14159265358979323846
Sphere::Sphere(int precision, Renderer* renderer, bool isLight):Entity()
{
	Init(precision);
	_renderer = renderer;
	currentAngle = PI * 2 * renderer->GetRandomNumber();
}

void Sphere::Init(int prec)
{
	numVertices = (prec+1) * (prec +1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++)
	{
		vertices.push_back(Vertex());

	}
	for (int i = 0; i < numIndices; i++)
	{
		indices.push_back(0);
	} 


	//generate verticies
	for (int i = 0; i <= prec; i++) {
		for (int j = 0; j <= prec; j++) {
			float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
			float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
			float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
			vertices[i * (prec + 1) + j].Position = glm::vec3(x, y, z);
			if ((abs(x) <0.01 && y==1 && abs(z) < 0.01) || (abs(x) < 0.01 && y ==-1 && abs(z) < 0.01))
			{
				vertices[i * (prec + 1) + j].Tangent = glm::vec3(0.0f, 0.0f, -1.0f);
			}
			else
			{
				vertices[i * (prec + 1) + j].Tangent = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(x, y, z));
			}
			vertices[i * (prec + 1) + j].TexCoords= glm::vec2(((float)j / prec), ((float)i / prec));
			vertices[i * (prec + 1) + j].Normal = glm::vec3(x, y, z);
		}
	}

	// generate indexes
	for (int i = 0; i < prec; i++) {
		for (int j = 0; j < prec; j++) {
			indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
			indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
			indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
			indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
		}

	}
	_va = new VertexArray(vertices, indices);

}

Sphere::~Sphere()
{
}

void Sphere::Draw(Shader& shader)
{
	ComputeWorldTransform();
	shader.SetMatrixUniform("worldMatrix", _worldMat);
	shader.SetMatrixUniform("projMatrix", _renderer->GetPerspectiveMatrix());
	// camera/view transformation
	shader.SetMatrixUniform("viewMatrix", _renderer->GetCamera()->GetViewMatrix());
	shader.SetVectorUniform("material.specular", glm::vec3(0.1, 0.1, 0.1));
	shader.SetFloatUniform("material.shininess", 0.0f);


	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int cubeNr = 1;


	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i]->GetType();
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
			glBindTexture(GL_TEXTURE_2D, textures[i]->GetId());
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
			glBindTexture(GL_TEXTURE_2D, textures[i]->GetId());
		}
			
		else if (name == "texture_cube")
		{
			number = std::to_string(cubeNr++);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i]->GetId());
		}
		
		shader.setInt(("material." + name + number).c_str(), i);
	}



	
	//_textureSpecular->Bind();
	_va->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindTexture(GL_TEXTURE0, 0);
}

void Sphere::SetTexture(std::string& path, std::string type, bool isCube)
{
	
	textures.emplace_back(new TextureDefault(type, isCube));
	if (isCube)
	{
		textures.back()->LoadCubeMap(path.c_str());
	}
	else
	{
		textures.back()->Load(path);
	}
	
}

void Sphere::AddSatellite(Sphere* satellite, float speed, float r)
{
	satellite->owner = this;
	satellite->radiusSatelite = r;
	satellite->speedSatelite = speed;
	s_satellites.emplace_back(satellite);
}

float lerp(float a, float b, float t)
{
	float c = (1 - t) * a + t * b;
	return c;
}

void Sphere::AddSatellite(Model* satellite, float speed, float r )
{
	m_satellites.emplace_back(satellite);
	satellite->owner = this;
	satellite->radiusSatelite = r;
	satellite->speedSatelite = speed;
	
	for (int i =0; i<satellite->radiusOffsetXZ.size(); i++)
	{
		float rand = _renderer->GetRandomNumber();
		satellite->radiusOffsetXZ[i] = lerp(-2, 2, rand);
	}
	
}
void PrintVec(glm::vec3 pos);

void Sphere::Update(float time)
{
	UpdateSelfRot(time);
	UpdateSatellites(time);
	if (radiusSatelite > 0.3f)
	{
		auto omega = speedSatelite / radiusSatelite;
		auto xSpeed = radiusSatelite * sin(currentAngle + omega * time);
		auto zSpeed = radiusSatelite * cos(currentAngle + omega * time);
		currentAngle = currentAngle + omega * time;
		auto x = xSpeed;
		auto z = zSpeed;
		auto ownerPos = owner->GetPosition();
		SetPosition(x+ ownerPos.x, GetPosition().y, z+ ownerPos.z);
	}
	
	
}
void Sphere::UpdateSatellites(float time)
{
	for (auto satellite : s_satellites)
	{
		satellite->Update(time);
	}
	
}
void Sphere::UpdateSelfRot(float time)
{
	SetRotation(GetRotation().x , GetRotation().y+ time * selfRotationSpeed, GetRotation().z);
}
