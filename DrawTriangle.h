#ifndef DRAW_TRIANGLE_H_
#define DRAW_TRIANGLE_H_

#include <GLES3/gl3.h>

/**
 * Encapsulates the data for a single vertex.
 * Must match the vertex shader's input.
 */
typedef struct Vertex_s {
    float position[2];  // 2D position: x, y
} Vertex;

/**
 * Creates a Vertex Buffer Object (VBO) and loads the provided vertices into it.
 *
 * @param vertices Pointer to an array of Vertex structures.
 * @param numVertices The number of vertices in the array.
 * @return The ID of the created VBO, or 0 if the creation failed.
 */
GLuint vboCreate(const Vertex* vertices, GLsizei numVertices);

/**
 * Frees the VBO.
 *
 * @param vbo The VBO's name (ID).
 */
void vboFree(GLuint vbo);

/**
 * Sets up and draws a triangle using the provided vertices.
 * This function creates a VBO, binds the vertex attributes needed for drawing, and renders the triangle.
 *
 * @param vertices Pointer to an array of Vertex structures.
 * @param numVertices The number of vertices in the array.
 * @return The ID of the created VBO, or 0 if the creation failed.
 */
GLuint drawTriangle(const Vertex* vertices, GLsizei numVertices);

#endif // DRAW_TRIANGLE_H_
