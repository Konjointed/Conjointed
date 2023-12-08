#version 410 core

out vec4 FragColor;

in vec3 Normal; // Normal from the vertex shader
in vec2 TexCoords; // Texture coordinates from the vertex shader

uniform vec3 uColor; // Uniform color
uniform float uTransparency; // Uniform for transparency

void main()
{
    vec3 normalizedNormal = normalize(Normal);
    // Basic lighting calculation for demonstration (you can modify this)
    float diff = max(dot(normalizedNormal, vec3(0, 0, 1)), 0.0);

    vec3 color = uColor * diff; // Apply lighting to the color
    FragColor = vec4(color, uTransparency); // Set the color with transparency
}