#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;

uniform bool isZBufferPreview;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // 转换为 NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    vec4 texColor;
    if (isZBufferPreview == false) {
        texColor = texture(material.texture_diffuse1, TexCoords);
    }
    else {
        float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
        texColor = vec4(vec3(depth), 1.0);
    }
    
    if (texColor.a == 0.0) {
        FragColor = vec4(0.4, 0.4, 0.4, 1.0);
    }else {
        FragColor = texColor;
    }
}

