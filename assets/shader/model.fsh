#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{
    vec4 texColor = texture(material.texture_diffuse1, TexCoords);
    if (texColor.a == 0.0) {
        FragColor = vec4(0.4, 0.4, 0.4, 1.0);
    }else {
        FragColor = texColor;
    }
}

