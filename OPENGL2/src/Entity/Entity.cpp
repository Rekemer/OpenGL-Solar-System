#include "Entity.h"

#include "../Rendering/Renderer.h"

void Entity::SetPosition(glm::vec3 pos)
{
	_position = pos;
	_recomputeWorldMat = true;
}

void Entity::SetRotation(glm::vec3 rot)
{
	_rotation = rot;
	_recomputeWorldMat = true;
}
void Entity::SetScale(glm::vec3 scale)
{
	_scale = scale;
	_recomputeWorldMat = true;
}

Entity::Entity() :
_position(glm::vec3(0.0f, 0.0f, 0.0f)),
_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
_recomputeWorldMat(true)
{

}


void Entity::ComputeWorldTransform()
{
	if (!_recomputeWorldMat) return;
	// create scale matrix
	glm::mat4 identityMat = glm::mat4(1.0f);


	_worldMat = glm::translate(identityMat, _position);
	_worldMat *= glm::rotate(identityMat, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	_worldMat *= glm::rotate(identityMat, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	_worldMat *= glm::rotate(identityMat, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	_worldMat *= glm::scale(identityMat, _scale);
	_recomputeWorldMat = false;


}