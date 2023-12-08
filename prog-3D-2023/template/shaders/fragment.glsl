#version 330 core

in vec3 o_positionWorld; 
in vec3 o_normalWorld; 
in vec2 o_uv0;
in mat3 tbn;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D colorTextureNormal;

uniform vec3 positionLight;
uniform vec3 couleurLight;
uniform vec3 positionCamera;

void main() {
    vec3 newPosLight = tbn * positionLight;
    vec3 newPosCamera = tbn * positionCamera;
    vec3 newPos = tbn * o_positionWorld;
    vec3 newNormal = texture(colorTextureNormal, o_uv0).rgb;
    newNormal = newNormal * 2.0 - 1.0;
    newNormal = normalize(tbn * newNormal);
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * couleurLight;
    
    // diffuse
    vec3 lightDir = normalize(positionLight - o_positionWorld);
    float diff = max(dot(newNormal, lightDir), 0.0);
    vec3 diffuse = diff * couleurLight;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(positionCamera - o_positionWorld);
    vec3 reflectDir = reflect(-lightDir, newNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * couleurLight;
    
    
    vec3 result = (ambient + diffuse + specular);
    FragColor = texture(colorTexture, o_uv0) * vec4(result, 1.0) * color; // Rendu de l'exercice 2 (avec normale map)
    //FragColor = texture(colorTexture, o_uv0) * color; // Rendu de l'exercice 1 (juste la texture)
    
    
    
    
    //FragColor *= ;
    // DEBUG: position
    //FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    //FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    // FragColor = vec4(o_uv0, 1.0);
}
