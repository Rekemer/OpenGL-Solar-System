#pragma once
#include "gtc/quaternion.hpp"
class Entity
{
public:
	Entity();
	void SetScale(glm::vec3 scale);
	glm::mat4 GetWorldMatrix() const { return _worldMat; }
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void ComputeWorldTransform();
	glm::vec3 GetPosition() { return _position; }
private:
protected:
	
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
	glm::mat4 _worldMat;
	bool _recomputeWorldMat;
	
};

