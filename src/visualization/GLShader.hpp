#ifndef GLSHADER_H
#define GLSHADER_H

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	#include "GL/glew.h"
#endif
#include <GLFW/glfw3.h> // GLFW helper library

GLuint LoadShader(const char *vertex_path, const char *fragment_path);

#endif