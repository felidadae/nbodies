#ifndef GLSHADER_H
#define GLSHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW helper library

GLuint LoadShader(const char *vertex_path, const char *fragment_path);

#endif
