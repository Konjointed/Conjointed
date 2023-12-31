#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 

/*
FLIP
void main()
{
    // Invert the y-coordinate
    vec2 flippedTexCoords = vec2(TexCoords.x, 1.0 - TexCoords.y);

    vec3 col = texture(screenTexture, flippedTexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 

DEFAULT 
void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 


INVERSION
void main()
{
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}  

GRAYSCALE
void main()
{
    FragColor = texture(screenTexture, TexCoords);
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0);
}   

BETTER GRAYSCALE?
void main()
{
    FragColor = texture(screenTexture, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
}   

KERNEL
const float offset = 1.0 / 300.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);
}  

BLUR
float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

DRUNK
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time; // Uniform to control the effect over time

void main()
{
    // Wavy effect
    float wave = sin(time + TexCoords.x * 10.0) * 0.005; 
    vec2 wavyTexCoords = TexCoords + vec2(wave, wave);

    // Duplicate vision
    vec3 color = texture(screenTexture, wavyTexCoords).rgb;
    color += texture(screenTexture, wavyTexCoords + vec2(0.01, 0.01)).rgb;
    color += texture(screenTexture, wavyTexCoords - vec2(0.01, 0.01)).rgb;
    color /= 3.0; // Average the color to prevent over-brightening

    FragColor = vec4(color, 1.0);
}
*/