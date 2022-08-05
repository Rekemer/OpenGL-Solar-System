#version 330 

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    samplerCube texture_cube1;
   // sampler2D texture_diffuse3;
    //sampler2D texture_specular1;
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
#define NR_POINT_LIGHTS 3  
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

uniform Material material;
uniform float time;
uniform vec3 cameraPos;
uniform samplerCube depthMap;
in vec2 diffuseTexCoords;
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 BrightColor;
;

in vec3 normal;
in vec3 lightPosition;
in vec3 fragPos;
in vec3 tangent;
in vec3 TexCoords;
in mat3 TBN;
vec3 calc_new_normal()
{
    vec3 norm = normalize(normal);
    vec3 tangent = normalize(tangent);
    tangent = normalize(tangent - dot(tangent,normal) * normal);
    vec3 bitangent = cross(norm,tangent);
    mat3 tbn = mat3(tangent,bitangent,norm);
    vec3 retrievedNormal = texture(material.texture_diffuse2,diffuseTexCoords).xyz;
    retrievedNormal = retrievedNormal*2 -1;
 
    vec3 newNormal = TBN*retrievedNormal;
    return newNormal;
}

void main()
{
	
    // properties
   vec4 normalMap= texture(material.texture_diffuse2,diffuseTexCoords);
    vec3 norm = calc_new_normal();
    vec3 viewDir = normalize(cameraPos - fragPos);
    
    vec3 result;
//
//    // phase 1: Directional lighting
    result = CalcDirLight(dirLight, norm, viewDir);
//    // phase 2: Point lights
   for(int i = 0; i < 1; i++)
     result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);    
    // result = CalcPointLight(pointLights[0], norm, fragPos, viewDir);    
//    // phase 3: Spot light
//    result += CalcSpotLight(spotLight, norm, fragPos, viewDir);
    //outColor = vec4(result,1.0f);
   //outColor = texture(material.texture_diffuse1,diffuseTexCoords);
  // float offsetX =time; 
  // vec2 texCoord = vec2(diffuseTexCoords.x+offsetX,diffuseTexCoords.y);
  
   outColor = vec4(result ,1.0);
  float brightness = dot(outColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(outColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

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
  //  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, diffuseTexCoords));
    return vec3(ambient);
}


float ShadowCalculation(vec3 fragPos,vec3 lightPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= 25;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
} 
vec4 ShadowDebug(vec3 fragPos,vec3 lightPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= 25;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    return vec4(vec3(closestDepth / 25), 1.0);
  
}  


vec3 textureDebug(vec3 fragPos,vec3 lightPos){
    vec3 fragToLight = fragPos - lightPos;
    vec3 result = texture(depthMap, fragToLight).rgb;
    return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, diffuseTexCoords));
    vec3 diffuse  = light.diffuse  * diff *vec3(texture(material.texture_diffuse1, diffuseTexCoords));
   // vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    //specular *= attenuation;
    float shadow = ShadowCalculation(fragPos,light.position);
    vec3 debug = vec3(ShadowDebug(fragPos,light.position));
    vec3 text = textureDebug(fragPos,light.position);
    return vec3 ( diffuse*(1-shadow));
} 
//vec3 CalcSpotLight( SpotLight spotLight,vec3  normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 ambientLight = spotLight.ambient * vec3(texture(material.diffuse, diffuseTexCoords)); 
//	vec3 norm = normalize(normal);
//	vec3 lightDir = normalize(spotLight.position - fragPos);
//    vec3 specular;
//    vec3 diffuse;
//	float  theta  = dot(spotLight.direction,-lightDir);
//	if (spotLight.cutOff < theta)
//	{
//	float diff = max( 0.0,dot(norm, lightDir));
//	diffuse = diff * spotLight.diffuse * vec3(texture(material.diffuse, diffuseTexCoords)); 
//	
//	vec3 viewDir = normalize(cameraPos - fragPos);
//	vec3 reflectDir = reflect(-lightDir, norm);  
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//	specular = material.specular * spec * light.specular;  
//    }
//	vec3 outColor =  ambientLight+diffuse +specular;
//    return outColor;
//}