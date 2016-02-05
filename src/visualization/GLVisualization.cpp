#include "GLVisualization.h"

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
#include "GLShader.hpp"



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





void start (GLBridgeInterface* glbridge, int maxiter) {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return;
	}

#if defined(__APPLE__) || defined(MACOSX)
	// to use the newest available Opengl
	// at least OpenGL 3.2
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	// create GLFW window
	GLFWwindow* window 
		= glfwCreateWindow (600, 600, 
			"NBodiesSystem", NULL, NULL);
	if (!window) {
		fprintf (stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent (window);

#if defined(__APPLE__) || defined(MACOSX)
#else
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();
#endif

#if defined(__DEBUG__)
	//FrameBuffer size
	int fbwidth, fbheight;
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	std::cout << "fbwidth" << fbwidth << std::endl;
	std::cout << "fbheight" << fbheight << std::endl;

	// get version info
	const GLubyte* renderer = glGetString (GL_RENDERER); 	// get renderer string
	const GLubyte* version = glGetString (GL_VERSION); 		// version as a string
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);
#endif

	// tell GL to only draw onto 
	// a pixel if the shape is closer to the viewer
	glEnable 	(GL_DEPTH_TEST); 	// enable depth-testing
	glDepthFunc (GL_LESS); 			// depth-testing interprets
									// a smaller value as "closer"

	
	//--------------------------------------------------------
	glbridge->initOpenGLPart();

	GLuint shader_programme = 
		LoadShader(
			"src/visualization/shader.vert", 
			"src/visualization/shader.frag"
		);
	glm::mat4 mvp(1.0);
	GLuint MatrixID = 
		glGetUniformLocation(shader_programme, "MVP");
	//--------------------------------------------------------

	int iiter = 0;
	while (!glfwWindowShouldClose (window)) {
		float deltaTime = elapsedTime();
		glbridge->stepPositions(deltaTime/15.0);
		glClearColor(0.43f/2.5f,0.48f/2.5f,0.59f/2.5f,1.0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (shader_programme);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		glPointSize(10);
		glDrawArrays (GL_POINTS, 0, glbridge->getN());
		glfwPollEvents ();
		glfwSwapBuffers (window);

		++iiter;
		if (iiter == maxiter) return;
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
}





