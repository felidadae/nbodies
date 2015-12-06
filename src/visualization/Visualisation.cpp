
#if defined(__APPLE__) || defined(MACOSX)
	#define _GLFW_USE_RETINA
	#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h> // GLFW helper library

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	#include <GL/wglew.h>
#endif

#include "GLShader.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

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


int main () {
	// start GL context and O/S window using the GLFW helper library
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

	GLFWwindow* window = glfwCreateWindow (800, 600, "Hello Triangle", NULL, NULL);
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
	const GLubyte* renderer = glGetString (GL_RENDERER); 	// get renderer string
	const GLubyte* version = glGetString (GL_VERSION); 		// version as a string
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable 	(GL_DEPTH_TEST); 	// enable depth-testing
	glDepthFunc (GL_LESS); 			// depth-testing interprets a smaller value as "closer"

	
	//--------------------------------------------------------
	//Model 1
	GLfloat points[] = {
	   0.0f,  1.0f,  0.0f,
	   1.0f, -1.0f,  0.0f,
	  -1.0f, -1.0f,  0.0f
	};

	GLuint vbo = 0;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);
	GLuint vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint shader_programme = 
		LoadShader("src/visualization/shader.vert", "src/visualization/shader.frag");
	glm::mat4 mvp;
	GLuint MatrixID = glGetUniformLocation(shader_programme, "MVP");
	//--------------------------------------------------------

	float angle = 0.0f;
	while (!glfwWindowShouldClose (window)) {
		//update mvp
		float deltaTime = elapsedTime();
		angle += deltaTime * 1.0f;
		//std::cout << angle << std::endl;
		mvp = glm::rotate(angle , glm::vec3(0.0f, 0.0f, -1.0f));
		
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram (shader_programme);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		//glBindVertexArray (vao);
		
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLES, 0, 3);
		
		// update other events like input handling 
		glfwPollEvents ();
		
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (window);
	}


	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}





