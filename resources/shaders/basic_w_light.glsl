#shader vertex
#version 410 core

layout(location=0)in vec4 position;
uniform mat4 u_MVP;
void main(){
    gl_Position =u_MVP*position;
}

    #shader fragment
    #version 410 core

layout(location=0)out vec4 color;

uniform vec4 u_Color;
uniform vec4 u_lightColor;

void main(){
    color =u_Color*u_lightColor;
}