#include "Camera.h"
#include <glfw3.h>
#include <iostream>




Camera::Camera(GLFWwindow* window)
{
    _window = window;
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    _lastX = width / 2.f;
    _lastY = height / 2.f;
}
void Camera::Update()
{
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    UpdateCursor();
    ProcessKeyboardInput(deltaTime);
    ProcessMouseInput();
    UpdateViewMatrix();
}

void Camera::UpdateCursor()
{
    GLdouble currentXPos, currentYPos;
    glfwGetCursorPos(_window, &currentXPos, &currentYPos);
    int height,width;
    glfwGetWindowSize(_window, &width, &height);
    if (currentXPos <width && currentXPos >0 &&
        currentYPos < height && currentYPos >0)
    {
        if (!isCursorHoveringWindow)
        {
          // firstMouse = true;
        }
        isCursorHoveringWindow = true;
    }
    else
    {
        isCursorHoveringWindow = false;
    }
}

void Camera::ProcessMouseInput()
{


   // if (!isCursorHoveringWindow) return;
    if (firstMouse )
    {
        GLdouble currentXPos, currentYPos;
        glfwGetCursorPos(_window, &currentXPos, &currentYPos);
        _lastX = currentXPos;
        _lastY = currentYPos;
        firstMouse = false;
    }
    GLdouble currentXPos, currentYPos;
    glfwGetCursorPos(_window, &currentXPos, &currentYPos);
   // std::cout << currentXPos << " " << currentYPos << "\n";
    float xoffset = currentXPos - _lastX;
    float yoffset = currentYPos - _lastY;
    yoffset *= -1;
    _lastX = currentXPos;
    _lastY = currentYPos;
    glm::vec3 direction;
    float sens = 0.4f;
    yaw += xoffset * sens;
    pitch += yoffset * sens;
    pitch = glm::clamp(pitch, -89.f, 89.f);
    float sq = cos(glm::radians(pitch));
    direction.x = cos(glm::radians(yaw)) * sq;
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * sq;
    cameraFront = glm::normalize(direction);

}
void Camera::ProcessKeyboardInput(float deltaTime)
{
    const float cameraSpeed = 5.f; // adjust accordingly
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        auto pos = _position + cameraSpeed * cameraFront * deltaTime;
        SetPosition(pos  );
    }
        
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
	{
        auto pos = _position - cameraSpeed * cameraFront * deltaTime;
        SetPosition(pos  );
	}
		
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        auto pos = _position - glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
        SetPosition(pos );
    }
       
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        auto pos = _position + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
        SetPosition(pos );
    }
      

}

void Camera::UpdateViewMatrix()
{
    _view = glm::lookAt(_position, _position + cameraFront, cameraUp);
}


