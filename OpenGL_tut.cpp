#include <SDL2/SDL.h>           // Include the SDL2 main header file
#include <SDL2/SDL_opengles2.h> // Include the SDL2 OpenGL ES 2 header for GLES functions
#include <GLES3/gl3.h>          // Include the OpenGL ES 3.0 header file for core GLES functions
#include <cstdio>               // Include C standard I/O library for input/output operations
#include <cstdlib>              // Include C standard library for general utilities like memory management, process control

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;    // Pointer to an SDL_Window structure, which represents a window in SDL
    SDL_GLContext context = NULL; // Variable to hold the OpenGL context, which maintains all of the OpenGL state

    // Initialize SDL with the video subsystem, which is required for creating windows and handling graphics
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // If SDL initialization fails, print an error message and return a non-zero value to indicate failure
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 10;
    }

    // Ensure that SDL_Quit is called when the program exits, cleaning up all initialized subsystems
    atexit(SDL_Quit);

    // Request an OpenGL ES 3.0 context, specifying the OpenGL ES profile and version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Request OpenGL ES major version 3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0); // Request OpenGL ES minor version 0

    // Force the usage of the GLES backend, ensuring that the OpenGL ES driver is used instead of a desktop OpenGL driver
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");

    // Enable double buffering to prevent screen tearing (flickering) by setting the double-buffering attribute
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create a window with the specified width, height, and OpenGL support
    window = SDL_CreateWindow(
        "GLES3+SDL2 Tutorial",               // Window title
        SDL_WINDOWPOS_UNDEFINED,             // Initial x position (undefined means SDL will choose a default position)
        SDL_WINDOWPOS_UNDEFINED,             // Initial y position (undefined means SDL will choose a default position)
        800,                                 // Window width in pixels
        600,                                 // Window height in pixels
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // Flags to indicate that the window supports OpenGL and should be shown immediately
    );

    // Check if the window creation succeeded
    if (!window) {
        // If window creation failed, print an error message and show a simple message box with the error
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create the main window.", NULL);
        return EXIT_FAILURE; // Return a failure code
    }

    // Create an OpenGL context associated with the window, which will manage the OpenGL state
    context = SDL_GL_CreateContext(window);
    if (!context) {
        // If OpenGL context creation failed, print an error message and show a simple message box with the error
        fprintf(stderr, "Failed to create OpenGL context: %s\n", SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create an OpenGL context.", NULL);
        return EXIT_FAILURE; // Return a failure code
    }

    // Clear the screen to a specified color using OpenGL ES functions
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f); // Set the clear color to a dark red (RGBA)
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer with the clear color

    // Swap the buffers to display the cleared screen
    SDL_GL_SwapWindow(window);

    // Variable to keep track of whether the user wants to quit the program
    bool quit = false;

    // Main loop: wait for the user to close the window
    while (!quit) {
        SDL_Event event; // Event handler variable

        // Wait for an event (this function blocks until an event occurs)
        if (SDL_WaitEvent(&event) != 0) {
            // Check if the event is of type SDL_QUIT (e.g., the user closed the window)
            if (event.type == SDL_QUIT) {
                // If the user closed the window, set quit to true to exit the loop
                quit = true;
            }
        }
    }

    // Cleanup: destroy the OpenGL context and the window to free up resources
    SDL_GL_DeleteContext(context); // Delete the OpenGL context
    SDL_DestroyWindow(window);     // Destroy the window

    // Return success code (0) to indicate the program ran successfully
    return EXIT_SUCCESS;
}
