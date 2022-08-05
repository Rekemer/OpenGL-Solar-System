#version 330 

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
   // sampler2D texture_diffuse3;
    sampler2D texture_specular1;
   // sampler2D texture_specular2;
	//sampler2D diffuse;
    vec3 specular;
    float shininess;
};


uniform Material material;
uniform float time;
uniform vec3 cameraPos;

in vec2 diffuseTexCoords;
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 BrightColor;


in vec3 fragPos;



void main()
{
   outColor = texture(material.texture_diffuse1, diffuseTexCoords);
   float brightness = dot(outColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(outColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}

