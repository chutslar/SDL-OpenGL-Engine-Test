R"(
#version 330 core
out vec4 FragColor;
uniform vec4 tgtColor;

void main()
{
    FragColor = tgtColor;
} 
)"