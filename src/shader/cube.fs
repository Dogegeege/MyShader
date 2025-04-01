#version 330 core
out vec4 FragColor;

in vec3 outColor;
in vec2 TexCoord;

//纹理属于全局变量
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixValue;

void main(){
    FragColor=mix(texture(texture1,TexCoord),texture(texture2,TexCoord),mixValue);
}