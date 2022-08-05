#version 330 


struct Material
{
    sampler2D texture_diffuse1;
   // sampler2D texture_diffuse2;
   // sampler2D texture_diffuse3;
    sampler2D texture_specular1;
   // sampler2D texture_specular2;
	//sampler2D diffuse;
    vec3 specular;
    float shininess;
};
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
uniform DirLight dirLight;
uniform Material material;
#define NR_POINT_LIGHTS 3  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform samplerCube depthMap;

in vec3 normal;
in vec3 cameraPos;
in vec3 fragPos;

in vec2 diffuseTexCoords;


layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 BrightColor;


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
    return vec3(ambient+diffuse);
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
    vec3 diffuse  = diff* light.diffuse*  vec3(texture(material.texture_diffuse1, diffuseTexCoords));
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    float shadow = ShadowCalculation(fragPos,light.position);
    vec3 debug = vec3(ShadowDebug(fragPos,light.position));
    vec3 text = textureDebug(fragPos,light.position);
    return vec3(ambient+diffuse*(1-shadow));
} 


void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - fragPos);
    
    vec3 result;
//
//    // phase 1: Directional lighting
 //   result = CalcDirLight(dirLight, norm, viewDir);
//    // phase 2: Point lights
   for(int i = 0; i < NR_POINT_LIGHTS; i++)
     result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);   
     vec3 tx =texture(material.texture_diffuse1, diffuseTexCoords).rgb;
   outColor = vec4(result,1);
   float brightness = dot(outColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(outColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

