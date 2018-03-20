#version 150

out vec4 fColor;

in vec3 Texture;
uniform samplerCube textureID;

void main()
{
    fColor = texture(textureID,Texture);
    //fColor = vec4(0, 1, 0, 0);
    fColor.a = 1.0;
}


