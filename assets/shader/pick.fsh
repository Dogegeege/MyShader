#version 330 core
out uvec4 FragColor;

uniform uint objectID; // 从CPU传递的对象ID

void main()
{
    //默认背景为0
    FragColor = uvec4(objectID, 0.0, 0.0, 1.0);
}