#version 150

precision highp float;

out vec4 fragColor;
in vec4 color;

const vec4 ambient_color = vec4(1.0, 1.0, 1.0, 1.0);
const float ambient_intensity = 0.1;

void main() 
{
    fragColor = color;
    fragColor = clamp(color + ambient_color * ambient_intensity, 0.0, 1.0);
}