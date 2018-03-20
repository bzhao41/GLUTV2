#version 150
in vec4 vPosition;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

out vec3 Texture;

void main(){
    
    Texture = normalize(vPosition.xyz);
    
    gl_Position = proj_matrix*view_matrix*vPosition;
}

