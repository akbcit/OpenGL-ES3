#version 300 es

// Input vector for position 
in vec2 vertPos;

// Output for color, passed to the fragment shader
out vec4 color;

// Constant for white color
const vec4 white = vec4(1.0);

// Called for every vertex
void main() {
    // Assign the color to be passed to the fragment shader
    color = white;

    // Set the position of the vertex
    gl_Position = vec4(vertPos, 0.0, 1.0);
}
