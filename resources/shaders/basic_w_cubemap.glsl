#shader vertex
#shader vertex
#version 410 core

layout(location=0)in vec4 position;
layout(location=1)in vec3 texCoord;

uniform mat4 u_MVP;

out vec3 textureDir;

void main(){
    gl_Position =u_MVP*position;
    textureDir =vec3(texCoord.x, 1-texCoord.y, texCoord.z);
}

    #shader fragment
    #version 410 core
out vec4 FragColor;
in vec3 textureDir;// direction vector representing a 3D texture coordinate
uniform samplerCube cubemap;// cubemap texture sampler

void main()
{
    FragColor = texture(cubemap, textureDir);
}