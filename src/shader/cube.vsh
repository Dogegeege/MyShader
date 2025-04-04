#version 330 core

//location 从glVertexAttribPointer 的第一个参数处接收数据
layout(location = 0)in vec3 aPos; // 位置变量的属性位置值为 0

//先不用纹理
//layout(location = 1)in vec2 aTexCoord; //纹理变量属性

layout(location = 1)in vec3 aNormal; //法线

out vec3 FragPos; //世界坐标位置
//out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * vec4(FragPos, 1.0); //顶点完成变换
    
}