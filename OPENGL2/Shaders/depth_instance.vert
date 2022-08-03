#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 instanceMatrix;


uniform mat4 projMatrix;
uniform mat4 viewMatrix;
void main()
{
    gl_Position = instanceMatrix * vec4(aPos, 1.0);
}  