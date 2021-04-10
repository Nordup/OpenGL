#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Colors;

out vec2 v_TexCoord;
out vec4 v_Colors;

uniform mat4 trans;

void main()
{
   gl_Position = trans * vec4(position.x, position.y, 0, 1);
   v_TexCoord = TexCoord;
   v_Colors = vec4(Colors, 1.0);
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;


uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform vec4 u_Color;

in vec2 v_TexCoord;
in vec4 v_Colors;

void main()
{
   vec4 texColor1 = texture(u_Texture1, v_TexCoord);
   vec4 texColor2 = texture(u_Texture2, v_TexCoord);
   color = mix(mix(texColor1, texColor2, u_Color.x), v_Colors, 1 - u_Color.x);
};