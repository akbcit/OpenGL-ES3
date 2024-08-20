#version 300 es

// The #version directive specifies the version of the OpenGL Shading Language (GLSL) used in this shader.
// "300 es" means OpenGL ES 3.0, a version of OpenGL for Embedded Systems, which is commonly used in mobile devices, web browsers, etc.

#ifdef GL_ES
precision highp float;
#endif

// GL_ES is a predefined macro that is defined when compiling for OpenGL ES.
// The precision qualifier specifies the precision for floating-point operations in the fragment shader.
// "highp" stands for high precision, which is often used for calculations where accuracy is important.


// 'in' is a keyword in GLSL that represents input variables to the shader. 
// The 'color' vector is passed from the previous shader stage, usually the vertex shader.
// In this case, it represents the color value to be used for the fragment.
in vec4 color;


// 'out' is a keyword in GLSL that represents output variables from the shader. 
// The 'fragColour' vector is the output of this fragment shader, 
// and it will determine the final color of the pixel that this shader is being executed for.
out vec4 fragColour;


// The fragment shader is called once for every pixel that needs to be drawn on the screen.
// The fragment shader's main purpose is to compute the color of each pixel.

// The 'main' function is the entry point of the shader program.
// This is where the logic of the shader is implemented.
void main() {
    // We set the output color of the fragment (pixel) to the input color.
    // This means that the color passed from the vertex shader will be used directly as the pixel color.
    fragColour = color;
}

// What is a Shader?
// A shader is a type of computer program that runs on the GPU (Graphics Processing Unit).
// Shaders are used to perform rendering effects like lighting, shadows, and color transformations.
// They are essential in graphics programming because they allow developers to control how vertices and pixels are processed.

// What is a Fragment Shader?
// This specific type of shader is called a Fragment Shader. 
// Fragment shaders are responsible for calculating the color and other attributes of each pixel (fragment) on the screen.
// They are called "fragment shaders" because they operate on fragments, which are essentially potential pixels before they've been fully processed and turned into actual pixels on the screen.

// Why is it required?
// The fragment shader is required to determine the color of each pixel. 
// Without it, you wouldn't have control over how the final image appears on the screen.
// It's particularly important for applying effects like texturing, lighting, and color blending.

// What's it doing?
// In this simple shader, the fragment shader's job is to take an input color and directly output it as the final color of the pixel.
// This is a very basic operation, but in more complex shaders, this could involve calculations for lighting, texture sampling, or other graphical effects.
