#version 330  


layout(location = 0) in vec3 vertPos;


uniform mat4 worldMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
out vec3 TexCoords;
void main()
{
	TexCoords =vertPos;
	gl_Position =projMatrix *viewMatrix* worldMatrix * vec4(vertPos,1);
}


