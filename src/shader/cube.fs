#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos; //世界坐标位置
//in vec2 TexCoord;

//纹理属于全局变量
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float mixValue;

void main() {
    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (ambient + diffuse) * objectColor;
    
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * result;
    
    FragColor = vec4(result, 1.0);
}