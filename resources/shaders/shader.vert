#version 460 core

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexColor;
layout (location = 1) in vec2 VertexTexCoord;

out vec3 Color;
out vec2 TexCoord;

uniform float scr_aspect;
uniform float angle;

void main()
{
    float c = cos(angle);
    float s = sin(angle);

    vec2 rotated = vec2(
            VertexPos.x * c - VertexPos.y * s,
            VertexPos.x * s + VertexPos.y * c
    );

    gl_Position = vec4(
            rotated.x * scr_aspect,
            rotated.y,
            VertexPos.z,
            1.0
    );

    Color = VertexColor;
    TexCoord = VertexTexCoord;
}
