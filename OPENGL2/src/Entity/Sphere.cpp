#include "Sphere.h"

#include "../TextureDefault.h"
#include "../Rendering/Shader.h"
#include "../Rendering/VertexArray.h"
#include "../Rendering/Debuger.h"
#include "../Rendering/Renderer.h"
#include "Camera.h"
Sphere::Sphere(int precision, Renderer* renderer) : Entity()
{
	Init(precision);
	_renderer = renderer;
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
	
	_textureSpecular = new TextureDefault();
	
	//_textureSpecular->Load("res/Textures/Planets/Earth/earth_specular.jpg");

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
	shader.SetVectorUniform("material.specular", glm::vec3(0.8, 0.8, 0.8));
	shader.SetFloatUniform("material.shininess", 32.0f);
	
	/*auto dir = glm::vec4(-0.0f, 0.0f, 1.0f, 1.0f);
	shader.SetVectorUniform("dirLight.direction", (glm::vec3)dir);
	shader.SetVectorUniform("dirLight.ambient", glm::vec3(0.2, 0.2, 0.2));
	shader.SetVectorUniform("dirLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
	shader.SetVectorUniform("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));*/

	glActiveTexture(GL_TEXTURE0);
	_texture->Bind();
	
	//_textureSpecular->Bind();
	_va->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
}

void Sphere::SetTexture(std::string& path)
{
	_texture = new TextureDefault();
	_texture->Load(path);
}

void Sphere::AddSatellite(Sphere* satellite, float speed, float r)
{
	satellite->radiusSatelite = r;
	satellite->speedSatelite = speed;
	s_satellites.emplace_back(satellite);
}

void Sphere::AddSatellite(Model* satellite, float speed, float r )
{
	m_satellites.emplace_back(satellite);
}

void Sphere::Update(float time)
{
	UpdateSelfRot(time);
	UpdateSatellites(time);
	if (radiusSatelite > 0.3f)
	{
		auto omega = speedSatelite / radiusSatelite;

		std::cout << time << std::endl;
		auto xSpeed = radiusSatelite * sin(currentAngle + omega * time);
		auto zSpeed = radiusSatelite * cos(currentAngle + omega * time);
		currentAngle = currentAngle + omega * time;
		auto x = xSpeed;
		auto z = zSpeed;
		SetPosition(x, GetPosition().y, z);
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
	SetRotation(GetRotation().x + time * selfRotationSpeed, GetRotation().y, GetRotation().z);
}
