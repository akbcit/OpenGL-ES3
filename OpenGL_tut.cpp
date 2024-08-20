#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>
#include "Shader.h"
#include "DrawTriangle.h"

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_GLContext context = NULL;

    // Initialize SDL with video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 10;
    }

    // Ensure that SDL_Quit is called when the program exits
    atexit(SDL_Quit);

    // Request OpenGL ES 3.0 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Force the usage of the GLES backend
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");

    // Enable double buffering to prevent flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create a window with the specified width, height, and OpenGL support
    window = SDL_CreateWindow(
        "GLES3+SDL2 Tutorial",               // Window title
        SDL_WINDOWPOS_UNDEFINED,             // Window position (x)
        SDL_WINDOWPOS_UNDEFINED,             // Window position (y)
        800,                                 // Window width
        600,                                 // Window height
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // Flags to indicate OpenGL support and showing the window
    );

    // Check if the window creation succeeded
    if (!window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create the main window.", NULL);
        return EXIT_FAILURE;
    }

    // Create an OpenGL context associated with the window
    context = SDL_GL_CreateContext(window);
    if (!context) {
        fprintf(stderr, "Failed to create OpenGL context: %s\n", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create an OpenGL context.", NULL);
        return EXIT_FAILURE;
    }

    // Load the shader program and set it for use
    GLuint shaderProg = shaderProgLoad("Simple2D.vert", "Simple2D.frag");

    if (!shaderProg) {
        // Error messages already displayed...
            return EXIT_FAILURE;
    }
    glUseProgram(shaderProg);

    // Define the vertex data for the triangle
    Vertex vertices[] = {
        { 0.0f, -0.9f },
        { 0.9f, 0.9f },
        {-0.9f, 0.9f }
    };

    // Create the VBO and set up the vertex attributes
    GLuint triangleVBO = drawTriangle(vertices, 3);
    if (!triangleVBO) {
        return EXIT_FAILURE;
    }

    // Variable to keep track of whether the user wants to quit
    bool quit = false;

    // Main loop: wait for the user to close the window
    while (!quit) {

        // Event handler
        SDL_Event event; 

        // Clear the screen to black
        glClearColor(0.1f, 0.0f, 0.0f, 1.0f); // Set the clear color (RGBA)
        glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer with the clear color

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap the buffers to display the drawn triangle
        SDL_GL_SwapWindow(window);

        // Wait for an event (blocking)
        if (SDL_WaitEvent(&event) != 0) {
            // Check if the event is of type SDL_QUIT
            if (event.type == SDL_QUIT) {
                // If the user closed the window, set quit to true to exit the loop
                quit = true;
            }
        }
    }

    // Cleanup: destroy the OpenGL context and the window
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);

    // Return success code
    return EXIT_SUCCESS;
}
