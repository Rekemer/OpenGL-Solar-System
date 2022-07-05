#include "Camera.h"
#include <glfw3.h>
Camera::Camera(GLFWwindow* window)
{
	_window = window;
}

void Camera::Update()
{
    ProcessInput();
    UpdateViewMatrix();
}
void Camera::ProcessInput()
{
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        SetPosition(_position + cameraSpeed * cameraFront);
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
		SetPosition(_position - cameraSpeed * cameraFront);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        SetPosition(_position - glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        SetPosition(_position + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed);
}

void Camera::UpdateViewMatrix()
{
    _view = glm::lookAt(_position, _position + cameraFront, cameraUp);
}
