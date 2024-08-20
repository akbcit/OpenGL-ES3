// shader.h

#ifndef __SHADER_H_
#define __SHADER_H_

#include <GLES3/gl3.h>
/* * 
* 
* Loads a vertex and fragment shader from disk and compiles (&links)
* them into a single shader program
* 
* @param vertFilename filename for the vertex shader program
* @param fragFilename filename for the fragment shader program
* 
* @return GLunit the shader program's ID or 0 if failed
* 
* */

GLuint shaderProgLoad(const char* vertFilename, const char* fragFilename);

/**
* Destroys a shader program
*/

void shaderProgDestroy(GLuint shaderProg);

#endif


