#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoords;
layout(location = 3)in vec3 tangent;
layout(location = 4)in vec3 bitangent;

out vec2 TexCoords;
out vec3 Normal;

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
    Normal = mat3(transpose(inverse(model))) * aNormal; //修正法线
    gl_Position = projection * view * model * (vec4(aPos, 1.0f) + vec4(0.03 * Normal, 0));
    TexCoords = aTexCoords;
    
}