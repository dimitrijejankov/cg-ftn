#version 150

in  vec3 position;

uniform mat4 mvp_matrix;

void main() 
{
    gl_Position = mvp_matrix * vec4(position.x, position.y, position.z, 1.0);
}
