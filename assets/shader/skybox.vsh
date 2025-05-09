#version 330 core
layout(location = 0)in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // 只保留 xy 分量，z 分量为 w，w 分量为 1.0
}