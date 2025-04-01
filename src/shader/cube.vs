#version 330 core

//location 从glVertexAttribPointer 的第一个参数处接收数据
layout(location=0)in vec3 aPos;// 位置变量的属性位置值为 0
//layout(location=1)in vec3 aColor;// 颜色变量的属性位置值为 1
layout(location=1)in vec2 aTexCoord;//纹理变量属性

out vec3 outColor;// 向片段着色器输出一个颜色
out vec2 TexCoord;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position=projection*view*model*vec4(aPos.x,aPos.y,aPos.z,1.);
    //outColor=aColor;// 将ourColor设置为我们从顶点数据那里得到的输入颜色
    
    //outColor=aPos;//实验3，把坐标位置转换成颜色
    
    TexCoord=vec2(aTexCoord.x,aTexCoord.y);
}