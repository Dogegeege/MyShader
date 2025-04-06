#version 330 core

//location 从glVertexAttribPointer 的第一个参数处接收数据
layout(location = 0)in vec3 aPos; // 位置变量的属性位置值为 0
layout(location = 1)in vec2 aUV; // UV 坐标
layout(location = 2)in vec3 aNormal; // 法线

out vec3 FragPos; // 世界坐标位置
out vec2 UV; // 传递给片段着色器的 UV 坐标
out vec3 Normal; // 传递给片段着色器的法线

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * vec4(FragPos, 1.0); //顶点完成变换
    
}