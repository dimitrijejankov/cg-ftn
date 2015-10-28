#version 150

in  vec3 position;
in  vec3 normal;

out vec4 color;

uniform mat4 mvp_matrix;
uniform mat4 normal_matrix;

const vec3 diffuse_light_direction = vec3(0.45, 0.45 , 0.0);
vec4 diffuse_color = vec4(1, 1, 1, 1);
float diffuse_intensity = 0.7;

void main() 
{
    gl_Position = mvp_matrix * vec4(position.x, position.y, position.z, 1.0);

    vec3 n = vec3(normal_matrix * vec4(normal, 1.0));
    float light_intensity = dot(n, diffuse_light_direction);


    diffuse_color.x = (normal.x > 0.0f ? normal.x : 0.0f) + (normal.y < 0.0f ? -0.5f * normal.y : 0.0f) + (normal.z < 0.0f ? -0.5f * normal.z : 0.0f);
    diffuse_color.y = (normal.y > 0.0f ? normal.y : 0.0f) + (normal.z < 0.0f ? -0.5f * normal.z : 0.0f) + (normal.x < 0.0f ? -0.5f * normal.x : 0.0f);
    diffuse_color.z = (normal.z > 0.0f ? normal.z : 0.0f) + (normal.x < 0.0f ? -0.5f * normal.x : 0.0f) + (normal.y < 0.0f ? -0.5f * normal.y : 0.0f);

    color = clamp(diffuse_color * diffuse_intensity * light_intensity, 0.0, 1.0);
}
