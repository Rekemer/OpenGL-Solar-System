#version 330 

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
out vec4 outColor;

in vec3 normal;
in vec3 lightPosition;
in vec3 fragPos;






vec3 random3(vec3 c) {
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}


void main()
{
	
    // properties
   vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cameraPos - fragPos);
//
//    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
//    // phase 2: Point lights
//    for(int i = 0; i < NR_POINT_LIGHTS; i++)
//      result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);    
//    // phase 3: Spot light
//    result += CalcSpotLight(spotLight, norm, fragPos, viewDir);
    //outColor = vec4(result,1.0f);
   //outColor = texture(material.texture_diffuse1,diffuseTexCoords);
   float offsetX =sin(time/10); 

   vec2 texCoord = vec2(diffuseTexCoords.x,diffuseTexCoords.y) *30 ;
   vec4 color1 = texture(material.texture_diffuse1,texCoord);
   vec4 color2 = texture(material.texture_diffuse2,texCoord/2);
   //vec4 cellColor = vec4(224/256f,180/256f,20/256f,1);
  // outColor = mix(color1, color2, 0.5);
   vec2 i_st = floor(texCoord);
   vec2 f_st = fract(texCoord);
   vec3 vec = vec3(texCoord.x,texCoord.y,0);
   vec2 randomPoint = vec2(random3(vec3(i_st,0)));
  // randomPoint =  ;
 // vec2 point = ;
 //// texCoord = fract(diffuseTexCoords*3);
     vec2 diff = randomPoint - f_st;
    float dist = length(diff);
   
   
   
   float m_dist = 1.;
    for (int y= -1; y <= 1; y++) {
    for (int x= -1; x <= 1; x++) {
        // Neighbor place in the grid
        vec2 neighbor = vec2(float(x),float(y));
        vec2 point =  vec2(random3(vec3(i_st + neighbor,0)));
        point = 0.5 + 0.5*sin(time + 6.2831*point);
         vec2 diff = neighbor + point - f_st;

        // Distance to the point
        float dist = length(diff);

        // Keep the closer distance
        m_dist = min(m_dist, dist);
    }
}


   outColor = vec4(m_dist,m_dist,m_dist,1);
  

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

//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // attenuation
//    float distance    = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + 
//  			     light.quadratic * (distance * distance));    
//    // combine results
//    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, diffuseTexCoords));
//    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, diffuseTexCoords));
//    vec3 specular = light.specular * spec * material.specular;
//    ambient  *= attenuation;
//    diffuse  *= attenuation;
//    specular *= attenuation;
//    return (ambient + diffuse + specular);
//} 
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