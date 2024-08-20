#include "DrawTriangle.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include <GLES3/gl3.h>
#include <cstdio>
#include <cstdlib>

GLuint vboCreate(const Vertex* vertices, GLsizei numVertices) {
    // Create the Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Copy the vertex data into the VBO and then unbind it
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Check for errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        // Failed to create VBO
        glDeleteBuffers(1, &vbo);
        SDL_Log("Creating VBO failed, code %u\n", err);
        vbo = 0;
    }

    return vbo;
}

void vboFree(GLuint vbo) {
    glDeleteBuffers(1, &vbo);
}

GLuint drawTriangle(const Vertex* vertices, GLsizei numVertices) {
    // Create a VBO for the triangle
    GLuint triangleVBO = vboCreate(vertices, numVertices);
    if (!triangleVBO) {
        return 0;  // VBO creation failed
    }

    // Set up the vertex attributes
    GLuint positionIdx = 0;  // Position is vertex attribute 0
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glVertexAttribPointer(positionIdx, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
    glEnableVertexAttribArray(positionIdx);

    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

    // Unbind the VBO (optional for safety)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return triangleVBO;
}
