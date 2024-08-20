on 300 es

#ifdef GL_ES
precision highp float;
#endif

// Input vector for color from the vertex shader
in vec4 color;

// Output fragment color
out vec4 fragColour;

// Called for each fragment
void main() {
    // Set the output fragment color
    fragColour = color;
}
