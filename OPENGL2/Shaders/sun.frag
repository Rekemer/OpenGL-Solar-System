#version 330 
 float PI =3.14159265359;

float random (vec2 st);
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

struct Light {
    vec3 position;
	vec3 direction;
	float cutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 


struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  


struct SpotLight{
    vec3 position;
    float cutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;

};
uniform SpotLight spotLight;
vec3 CalcSpotLight( SpotLight spotLight,vec3  normal, vec3 fragPos, vec3 viewDir); 

uniform Light light;
uniform Material material;
uniform float time;
uniform vec3 cameraPos;

in vec2 diffuseTexCoords;
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 BrightColor;

in vec3 normal;
in vec3 lightPosition;
in vec3 fragPos;

uniform vec3 colorSun;
uniform vec3 cellColor;





vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}


vec4 voronoi(vec2 texCoord)
{
   vec4 result;
   vec2 i_st = floor(texCoord);
   vec2 f_st = fract(texCoord);
   float m_dist = 1.;
  
    for (int y= -1; y <= 1; y++) 
    {
       for (int x= -1; x <= 1; x++) 
       {
        // Neighbor place in the grid
        vec2 neighbor = vec2(float(x),float(y));
        vec2 neighborRel = i_st + neighbor;
//        if (neighborRel.x > i_st.x){
//        neighborRel.x = neighborRel.x-i_st.x;
//        }
//        if (neighborRel.y > i_st.y){
//        neighborRel.y = neighborRel.y-i_st.y;
//        }
        vec2 point =  random2(neighborRel);
       // point = clamp(point,0,1);
        point = 0.5 + 0.5*sin(time + 6.2831*point);
         vec2 diff = neighbor + point - f_st;

        // Distance to the point
        float dist = length(diff);

        // Keep the closer distance
        m_dist = min(m_dist, dist);
    
       }

    }

   
   result = vec4(1-m_dist,1-m_dist,1-m_dist,1) *vec4(cellColor,0) + vec4(colorSun,0);

   //result = vec4(i_st,0,1);
   return result;
}

float atan2(in float y, in float x)
{
    bool s = (abs(x) > abs(y));
    return mix(PI/2.0 - atan(x,y), atan(y,x), s);
}
void main()
{
	
    // properties
   vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - fragPos);
//
//    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

   float offsetX =sin(time/10); 

   vec2 texCoord = vec2(diffuseTexCoords.x,diffuseTexCoords.y) *30 ;
   
   
  vec4 cellColor = vec4(224/256.0f,180/256.0f,20/256.0f,1);
  vec4 cellColor2 = vec4(235/256.0f,121/256.0f,21/256.0f,1);

 
 vec2 tex;
 tex.x = (PI + atan2(fragPos.x, fragPos.z)) / (2 * PI);
 vec4 color1 = texture(material.texture_diffuse2,diffuseTexCoords);
 color1 = (color1*2 - 1) * sin(time)/100;

 vec4 color2 = texture(material.texture_diffuse1,diffuseTexCoords+color1.xy) * vec4(colorSun,1);
 outColor =  color2;
 outColor *= (voronoi(texCoord));

 float brightness = dot(outColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(outColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

// float threshold = 0.8;
// if (outColor.length <threshold){
//
// outColor += cellColor2;
// }

}
float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, diffuseTexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, diffuseTexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, diffuseTexCoords));
    return (ambient + diffuse + specular);
}

