#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Colors;

out vec2 v_TexCoord;
out vec4 v_Colors;

void main()
{
   gl_Position = vec4(position.x, position.y, 0, 1);
   v_TexCoord = TexCoord;
   v_Colors = vec4(Colors, 1.0);
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

in vec2 v_TexCoord;
in vec4 v_Colors;

void main()
{
   vec4 texColor = texture(u_Texture, v_TexCoord);
   color = texColor * u_Color * v_Colors;
};