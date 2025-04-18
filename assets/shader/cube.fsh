#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos; //世界坐标位置
in vec2 TexCoords;

struct Material {//材质
    sampler2D diffuse;
    sampler2D specular;
    
    float shininess;
};

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float intensity;
    
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // 环境光
    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 ambient = light.ambient;
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse;
    
    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * spec;
    
    //衰减
    
    float linear = 1.0f;
    float quadratic = 0.09f;
    float constant = 1.0f;
    
    float distancee = length(light.position - FragPos);
    float attenuation = 1.0 / (constant + linear * distancee + quadratic * (distancee * distancee));
    
    vec3 result = (ambient + diffuse + specular) * max(1.0, light.intensity) * attenuation;
    
    FragColor = vec4(result, 1.0);
}