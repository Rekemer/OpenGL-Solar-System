#pragma once
#include "gtc/quaternion.hpp"
class Entity
{
public:
	Entity();
	void SetScale(glm::vec3 scale);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	glm::mat4 GetWorldMatrix()
	{
		if (_recomputeWorldMat) {
			ComputeWorldTransform();
			_recomputeWorldMat= false;
		}
		return _worldMat;
	}
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void ComputeWorldTransform();
	glm::vec3 GetPosition() { return _position; }
	glm::vec3 GetRotation() { return _rotation; }
	glm::mat4 _worldMat;
private:
protected:
	
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
	
	bool _recomputeWorldMat;
	
};

