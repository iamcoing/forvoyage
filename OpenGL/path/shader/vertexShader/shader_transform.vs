#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoord;

out vec3 ourColor;
out vec2 outTexCoord;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(Pos, 1.0f);
    //gl_Position = vec4(Pos.x, -Pos.y, Pos.z, 1.0);
    ourColor = Color;
    outTexCoord = TexCoord;
}