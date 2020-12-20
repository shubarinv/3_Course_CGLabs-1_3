#shader vertex
#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 outColor;// output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    outColor = aColor;// set ourColor to the input color we got from the vertex data
    gl_Position = projection * view * vec4(FragPos, 1.0);
}


    #shader fragment
    #version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 outColor;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * outColor;
    FragColor = vec4(result, 1.0);
}