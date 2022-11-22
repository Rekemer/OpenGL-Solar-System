#pragma once


#include "Entity.h"
struct GLFWwindow;
class Camera : public Entity
{
public:
	void OnMouseEnter( );
	Camera()=default;
	void Init(GLFWwindow* _window);
	void ProcessMouseInput();
	void Update();
	void UpdateCursor();
	void ProcessKeyboardInput(float deltaTime);
	glm::mat4 GetViewMatrix() { return _view; }
	void UpdateViewMatrix();
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
private:

protected:
	GLFWwindow* _window;
	
	float lastFrame = 0.f;
	bool firstMouse = true;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 _view;
	float yaw = 90.0f; // because -z is forward 
	float pitch = 0.f; // up and down axis
	float _lastX;
	float _lastY;
	bool isCursorHoveringWindow;
};

