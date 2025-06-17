#version 330 core
layout(location = 0)in vec3 aPos;

out mat4 iProjection;
out mat4 iView;

layout(std140)uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    iProjection=projection;
    iView=view;
}