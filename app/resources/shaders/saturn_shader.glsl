//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}


//#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_sphere;

uniform vec3 lightDirection;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 texColor = texture(texture_sphere, TexCoords).rgb;


    // Ambient
    vec3 ambient = 0.09 * texColor;
    // Directional diffuse
    vec3 norm = normalize(Normal);
    vec3 dirLightDir = normalize(-lightDirection);
    float diffDir = max(dot(norm, dirLightDir), 0.0);
    vec3 diffuseDir = diffDir * texColor * 0.8;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-dirLightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.4) * spec;


    vec3 result = ambient + diffuseDir + specular;
    FragColor = vec4(result, 1.0);
}
