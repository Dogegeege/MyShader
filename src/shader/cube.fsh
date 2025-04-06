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
};

//in vec2 TexCoord;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords));
    
    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    vec3 result = (ambient + diffuse + specular);
    
    FragColor = vec4(result, 1.0);
}