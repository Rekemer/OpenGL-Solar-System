#version 330 

struct Material
{
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 cameraPos;

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
	vec3 ambientLight = light.ambient * material.ambient;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max( 0.0,dot(norm, lightDir));
	vec3 diffuse = diff * light.diffuse * material.diffuse;
	
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular;  

	outColor = vec4(0.2,0.8,0.5,1.0) * vec4(ambientLight+diffuse +specular,1.0);
	//outColor = vec4(0.2,0.8,0.5,1.0);
	//outColor = texture(uTexture,fragTexCoord);
}