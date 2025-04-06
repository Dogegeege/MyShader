#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos; //世界坐标位置

struct Material {//材质
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

//in vec2 TexCoord;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform float mixValue;

void main() {
    // 环境光
    vec3 ambient = light.ambient * material.ambient;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    vec3 result = (ambient + diffuse + specular);
    
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * result;
    
    FragColor = vec4(result, 1.0);
}