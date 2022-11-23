#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform vec3 pos;
uniform float dist;
uniform float radius;

void main()
{           
    
      vec2 offset = TexCoords-vec2(pos);
    vec2  ratio = vec2(9.f/16.f,1);
    float rad = length(offset/ratio);

    float deformation  = 1 / pow(rad * pow(dist,0.5),2) * radius * 0.1;
    offset = offset * (1 - deformation);
    offset += vec2(pos);
    //outColor = vec4(1,0,0,1);
    //fragPos
   

    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, offset).rgb;      
    vec3 bloomColor = texture(bloomBlur, offset).rgb;
    //bloomColor =pow(bloomColor, vec3(1.0 / gamma));
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
   // result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(hdrColor, 1.0);
    if (rad*dist <radius)
    {
        FragColor = vec4(1,1,1,1);
    }


  

}  

