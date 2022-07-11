#version 330 

struct Material
{
	sampler2D diffuse;
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
uniform vec3 cameraPos;

in vec2 diffuseTexCoords;
out vec4 outColor;
in vec2 fragTexCoord;
in vec3 normal;
in vec3 lightPosition;
in vec3 fragPos;
uniform sampler2D uTexture;
uniform vec3 lightColor;
void main()
{
	
    // properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - fragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
      result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);    
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, norm, fragPos, viewDir);        
    outColor = glm::vec4(result,1.0f);

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
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, diffuseTexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, diffuseTexCoords));
    vec3 specular = light.specular * spec *material.specular;
    return (ambient + diffuse + specular);
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
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, diffuseTexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, diffuseTexCoords));
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 
vec3 CalcSpotLight( SpotLight spotLight,vec3  normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambientLight = spotLight.ambient * vec3(texture(material.diffuse, diffuseTexCoords)); 
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(spotLight.position - fragPos);
    vec3 specular;
    vec3 diffuse;
	float  theta  = dot(spotLight.direction,-lightDir);
	if (spotLight.cutOff < theta)
	{
	float diff = max( 0.0,dot(norm, lightDir));
	diffuse = diff * spotLight.diffuse * vec3(texture(material.diffuse, diffuseTexCoords)); 
	
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	specular = material.specular * spec * light.specular;  
    }
	vec3 outColor =  ambientLight+diffuse +specular;
    return outColor;
}