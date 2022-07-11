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
	
	float ambientStr = 0.2f;
	vec3 ambientLight = light.ambient * vec3(texture(material.diffuse, diffuseTexCoords)); 
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);

	float  theta  = dot(-light.direction,lightDir);
	if (light.cutOff < theta)
	{
	
	float diff = max( 0.0,dot(norm, lightDir));
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, diffuseTexCoords)); 
	
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular;  

	outColor =  vec4(ambientLight+diffuse +specular,1.0);
	}


	//outColor = vec4(0.2,0.8,0.5,1.0);
	//outColor = texture(uTexture,fragTexCoord);
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
    vec3 specular = light.specular * spec * vec3(texture(material.specular, diffuseTexCoords));
    return (ambient + diffuse + specular);
}