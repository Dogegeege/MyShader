#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoords;

layout(location = 3)in vec3 tangent;
layout(location = 4)in vec3 bitangent;

out vec2 TexCoords;

layout(std140)uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}