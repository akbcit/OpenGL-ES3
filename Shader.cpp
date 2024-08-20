// Shader .cpp

#include "Shader.h"
#include <cstdio>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#ifdef _MSC_VER
#pragma warning(disable:4996) // allows us to use the portable fopen() function without warnings
#endif

/**Function that gets file name 
* 
* @param file the file
* 
* @return size_t the file's length in bytes
*/

static size_t fileGetLength(FILE* file) {
	size_t length;

	size_t currPos = ftell(file);
	fseek(file, 0, SEEK_END);
	length = ftell(file);

	// return the fie to its current position 
	fseek(file, currPos, SEEK_SET);

	return length;
}

/**
* Function that loads and compiles a shader from a file
* 
* @param filename the shader's filename
* @param shaderType the shader type (e.g., GL_VERTEX_SHADER)
* 
* @return GLuint the shader's ID, or 0 if failed
* 
* The first part (upto flcose loads the shader file into a buffer).
* Next and empty shader is created, teh source code is added and compiled.
* After that we check if compilation succeeded and get teh compilation log if it failed. 
* 
*/

static GLuint shaderLoad(const char * filename, GLenum shaderType) {

	// open file and store in a pointer
	FILE* file = fopen(filename, "r");

	// if no file with that filename 
	if (!file) {
		SDL_Log("Can't open file: %s\n", filename);
		return 0;
	}

	// get length of file
	size_t length = fileGetLength(file);

	// Alloc space for the file (plus '\0' termination)
	GLchar* shaderSrc = (GLchar*)calloc(length + 1, 1);

	// if could not allocate space
	if (!shaderSrc) {
		SDL_Log("Out of memory when reading file: %s\n", filename);
		fclose(file);
		file = NULL;

		return 0;
	}

	// read file
	fread(shaderSrc, 1, length, file);

	// done with file
	fclose(file);
	file = NULL;

	// Create a shader object of the specified shader type (e.g., GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
	// This object will be used to store the source code for the shader and compile it later.
	GLuint shader = glCreateShader(shaderType);

	// Attach the shader source code (written in GLSL) to the shader object created above.
	// 'shader' is the handle returned by glCreateShader, representing the shader object.
	// The second parameter, '1', specifies that we're passing a single string containing the source code.
	// '&shaderSrc' is a pointer to the source code string. This string will be used as the shader's source code.
	// The final parameter, 'NULL', indicates that the string is null-terminated, meaning no additional length array is needed.
	glShaderSource(shader, 1, (const GLchar**)&shaderSrc, NULL);

	// Free the memory allocated for shaderSrc to avoid memory leaks.
	// After calling glShaderSource, the shader object holds the source code, so the original string can be safely freed.
	// Setting shaderSrc to NULL afterward is a good practice to avoid dangling pointers.
	free(shaderSrc);
	shaderSrc = NULL;

	// compile the shader
	glCompileShader(shader);
	GLint compileSucceeded = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSucceeded);
	if (!compileSucceeded) {
		// compilation failed, print error info
		SDL_Log("Compilation of shader %s failed:\n", filename);
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* errLog = (GLchar*)malloc(logLength);
		if (errLog) {
			glGetShaderInfoLog(shader, logLength, &logLength, errLog);
			SDL_Log("%s\n", errLog);
				free(errLog);
		}
		else {
			SDL_Log("Couldn't get shader log; out of memory\n");
		}
		glDeleteShader(shader);
		shader = 0;
	}
	return shader;
}

/**
* Destroys a shader
*/

static void shaderDestroy(GLuint shaderID) {
	glDeleteShader(shaderID);
}


/**
* So far we can load individual shaders now its time to link shaders into a shader program 
* This function starts by loading the two shaders (shaderLoad()).
* If that’s successful then it creates a shader program (glCreateProgram()),
* attaches the shaders to the program (glAttachShader()),
* and links them (glLinkProgram()).Next, it checks if linking succeeded 
* (glGetProgramiv() with GL_LINK_STATUS) and prints out an error log obtained
* via glGetProgramInfoLog(). If linking succeeded, then the shader objects
* can be deleted (shaderDestroy()). Finally, the newly created shader program is returned to the code that called shaderProgLoad().
* to the code that called shaderProgLoad().
*/

GLuint shaderProgLoad(const char* vertFilename, const char* fragFilename) {

	// get vertex shader
	GLuint vertShader = shaderLoad(vertFilename, GL_VERTEX_SHADER);

	if (!vertShader) {
		SDL_Log("Couldn't load vertex shader: %s\n", vertFilename);
		return 0;
	}

	// get fragment shader
	GLuint fragShader = shaderLoad(fragFilename, GL_FRAGMENT_SHADER);

	if (!fragShader) {
		SDL_Log("Couldn't load fragment shader: %s\n", fragFilename);
		shaderDestroy(vertShader);
		vertShader = 0;
		return 0;
	}

	// create shader program
	GLuint shaderProg = glCreateProgram();

	if (shaderProg) {
		// attach shaders 
		glAttachShader(shaderProg, vertShader);
		glAttachShader(shaderProg, fragShader);

		// link program
		glLinkProgram(shaderProg);
		GLint linkingSucceeded = GL_FALSE;
		glGetProgramiv(shaderProg, GL_LINK_STATUS, &linkingSucceeded);
		if (!linkingSucceeded) {
			SDL_Log("Linking shader failed (vert. shader: %s, frag. shader: %s)\n",
				vertFilename, fragFilename);
			GLint logLength = 0;
			glGetProgramiv(shaderProg, GL_INFO_LOG_LENGTH, &logLength);
			GLchar* errLog = (GLchar*)malloc(logLength);

			if (errLog) {
				glGetProgramInfoLog(shaderProg, logLength, &logLength, errLog);
				SDL_Log("%s\n", errLog);
				free(errLog);
			}
			else {
				SDL_Log("Couldn't get shader link log; out of memory\n");
			}
			glDeleteProgram(shaderProg);
			shaderProg = 0;
		}

		// Don't need these shaders anymore after linking
		shaderDestroy(vertShader);
		shaderDestroy(fragShader);

		// Return the successfully created shader program
		return shaderProg;
	}
	else {
		SDL_Log("Couldn't create shader program\n");
	}

	// If we reached here, shader program creation failed
	return 0;
}

/**
* Destroys the shader progam
*/

void shaderProgDestroy(GLuint shaderProg) {
	glDeleteProgram(shaderProg);
}












