#ifdef __APPLE__
	#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	# define __gl_h_
	# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif

#if defined(__APPLE__) || defined(MACOSX)
	#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h> // GLFW helper library
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	#include <GL/wglew.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "../visualization/GLShader.hpp"

// includes, cuda
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

// Utilities and timing functions
#include <helper_functions.h>    // includes cuda.h and cuda_runtime_api.h
#include <timer.h>               // timing functions

// CUDA helper functions
#include <helper_cuda.h>         // helper functions for CUDA error check
#include <helper_cuda_gl.h>      // helper functions for CUDA/GL interop
#include "helper_cuda.h"
#include <vector_types.h>

double elapsedTime() {
	static double previous_seconds = glfwGetTime ();
	double current_seconds = glfwGetTime ();
	double elapsedTime = current_seconds - previous_seconds;
	previous_seconds = current_seconds;
	return elapsedTime;
}

void updateWindoWFPSCounter (GLFWwindow* window) {
	static double previous_seconds = glfwGetTime ();
	static int frame_count;
	double current_seconds = glfwGetTime ();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf (tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle (window, tmp);
		frame_count = 0;
	}
	frame_count++;
}
//-------------------------------------------------------------------





//-------------------------------------------------------------------
/*
	helper
*/
#define X 0 
#define Y 1
#define Z 2
#define D 3

/*
	memory addresses ->
	X Y Z 	X Y Z 	X Y Z
	point0  point1  point2
*/
struct Points {
	GLfloat* data;
	GLfloat get(int directionDim, int pointDim) { 
		return data[directionDim + pointDim*3]; 
	}
	void 	set(int directionDim, int pointDim, GLfloat value) { 
		data[directionDim+ pointDim*3] = value;  
	}
};

/*
	create 2D circle, point[Z] == 0 for each point, where Z==2
*/
GLfloat* createCircleOfEmotions(
	GLfloat middleX, 
	GLfloat middleY, 
	GLfloat R, 
	unsigned triangleNum) 
{
	int pointsNum = triangleNum+3;

	GLfloat radiusStep = 2*M_PI / triangleNum;

	GLfloat* data = new GLfloat[pointsNum*3];
	Points points;
	points.data = data;

	points.set(X,0, middleX);
	points.set(Y,0, middleY);
	points.set(Z,0, 0.0);

	for (int it = 0; it <= triangleNum; ++it)
	{
		GLfloat alpha = it*radiusStep;
		GLfloat deltaX = sin(alpha) * R;
		GLfloat deltaY = cos(alpha) * R;
		points.set(X, 1+it, deltaX + middleX);
		points.set(Y, 1+it, deltaY + middleY);
		points.set(Z, 1+it, 0.0);
		
		using namespace std;
		cout << "alpha == " << alpha / M_PI * 180 << endl;
		cout << points.get(X, 1+it) << endl;
		cout << points.get(Y, 1+it) << endl;
		cout << points.get(Z, 1+it) << endl << endl;

	}

	return points.data;
}

__global__
static void createCircleOfEmotionsCUDA(
	GLfloat middleX, 
	GLfloat middleY, 
	GLfloat R, 
	unsigned triangleNum, 
	GLfloat* positions)
{
	int it = threadIdx.x + blockIdx.x*blockDim.x;
	if (it < triangleNum) {
		GLfloat radiusStep = 2*M_PI / triangleNum;
		GLfloat alpha = it * radiusStep;
		GLfloat deltaX = sin(alpha) * R;
		GLfloat deltaY = cos(alpha) * R;
		positions[ it*3 		] = deltaX + middleX;
		positions[ it*3 + 1 	] = deltaY + middleY;
		positions[ it*3 + 2 	] = 0.0f;
	}
}
//-------------------------------------------------------------------





//-------------------------------------------------------------------
int main () {
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

#if defined(__APPLE__) || defined(MACOSX)
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	glfwWindowHint (GLFW_SAMPLES, 4);
	GLFWwindow* window = 
		glfwCreateWindow (600, 600, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf (stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent (window);

#if defined(__APPLE__) || defined(MACOSX)
#else
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();
#endif

	int fbwidth, fbheight;
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	std::cout << "fbwidth" << fbwidth << std::endl;
	std::cout << "fbheight" << fbheight << std::endl;

	// get version info
	const GLubyte* renderer = glGetString (GL_RENDERER); 	
	const GLubyte* version = glGetString (GL_VERSION); 		
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

	glEnable 	(GL_DEPTH_TEST); 	
	glDepthFunc (GL_LESS); 			

	//---------------------------------------------------------------
	//Model with CUDA
	// vbo variables

	//CUDA
	cudaGLSetGLDevice(0);
	//.

	const int triangleNum = 100;
	const int vertexNum = triangleNum+3;

	GLuint vbo = 0;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 
		D * vertexNum * sizeof (float), 
		0, GL_DYNAMIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
	
	GLuint vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, D, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);

	//CUDA
	struct cudaGraphicsResource *cuda_vbo_resource = NULL;
	
	checkCudaErrors(
		cudaGraphicsGLRegisterBuffer(
			&cuda_vbo_resource, vbo, 
			cudaGraphicsMapFlagsWriteDiscard));

	GLfloat *dptr;
	size_t num_bytes;
    checkCudaErrors(
    	cudaGraphicsMapResources(
    		1, &cuda_vbo_resource, 0));
    checkCudaErrors(
    	cudaGraphicsResourceGetMappedPointer(
    		(void **)&dptr, &num_bytes, cuda_vbo_resource));

    createCircleOfEmotionsCUDA <<<triangleNum, 1>>> (
    	0.0f, 0.0f, 0.5f, triangleNum, dptr);

    checkCudaErrors(
    	cudaGraphicsUnmapResources(
    		1, &cuda_vbo_resource, 0));
	//CUDA.




	GLuint shader_programme =
		LoadShader(
			"src/visualization/shader.vert", 
			"src/visualization/shader.frag"
		);
	glm::mat4 mvp;
	GLuint MatrixID = glGetUniformLocation(shader_programme, "MVP");
	//---------------------------------------------------------------


	float angle = 0.0f;
	while (!glfwWindowShouldClose (window)) {
		//update mvp
		float deltaTime = elapsedTime();
		//angle += deltaTime * 1.0f;
		mvp = glm::rotate(angle , glm::vec3(0.0f, 0.0f, -1.0f));
		
		// wipe the drawing surface clear
		glClearColor(1,1,1,1);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (shader_programme);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray (vao);
		
		// draw points 0-3 from the currently 
		// bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLE_FAN, 0, vertexNum);
		
		// update other events like input handling 
		glfwPollEvents ();
		
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
}
//-------------------------------------------------------------------




