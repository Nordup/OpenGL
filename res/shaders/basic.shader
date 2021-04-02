#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 vertColor;

out vec4 fragColor;

void main()
{
   gl_Position = vec4(position.x, position.y, 0, 1);
   fragColor = vec4(vertColor, 1.0);
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
in vec4 fragColor;

void main()
{
   color = u_Color * fragColor;
};