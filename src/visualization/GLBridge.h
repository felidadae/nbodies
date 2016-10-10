#ifndef N_BODIES_SYSTEM_VISUALIZATION_BRIDGE_HEADER
#define N_BODIES_SYSTEM_VISUALIZATION_BRIDGE_HEADER

#if defined(__APPLE__) || defined(MACOSX)
	#define GLFW_INCLUDE_GLCOREARB
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include "../physics/NBodiesSystem.h"

#include <stdio.h>
#include <iostream>
#include "../InitialState.h"
#include "GLBridgeInterface.h"



class GLBridge: 
	public GLBridgeInterface 
{
public:
	GLBridge(
		NBS_State& initialState):
		state_0(initialState)  
	{
		points = NULL;
		vbo = 0;
		vao = 0;

		this->N = initialState.N;
		this->D = initialState.D;
		this->points = new GLfloat[N*D];

		//zip_modelPositions_to_points();
	}

	void initOpenGLPart () {
		glGenBuffers (1, &vbo);
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << err << std::endl;
		}
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (GL_ARRAY_BUFFER, 
			D * N * sizeof (GLfloat), 
			points, GL_DYNAMIC_DRAW);
		glGenVertexArrays (1, &vao);
		glBindVertexArray (vao);
		glEnableVertexAttribArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer (0, 3, GL_FLOAT, 
			GL_FALSE, 0, NULL);

		this->nbodiesSystem = 
			new NBodiesSystem(
				state_0.D, state_0.N, 
				state_0.p_0, state_0.v_0, 
				state_0.m_0);
	}

	~GLBridge()
	{ 
		delete [] this->points; 
	}

	void stepPositions (time_type delta) {
		nbodiesSystem->step(delta);
		zip_modelPositions_to_points();

		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (GL_ARRAY_BUFFER, 
			D * N * sizeof (GLfloat), 
			points, GL_DYNAMIC_DRAW);
		glVertexAttribPointer (0, 3, GL_FLOAT, 
			GL_FALSE, 0, NULL);
	}

	int getN() { return N; }
	int getD() { return D; }

private:
	NBodiesSystem* nbodiesSystem;
	NBS_State& state_0;
	GLfloat* points;
	GLuint vbo;
	GLuint vao;
	int N;
	int D;

	void zip_modelPositions_to_points() {
		Params<position_type>* model_positions = 
			nbodiesSystem->getPositions();

		int ipoints = 0;
		for (int i = 0; i < N; ++i)
			for (int d = 0; d < D; ++d) {
				points[ipoints++] = 
					(*model_positions).getVal(d, i);
			}
	}
};

#endif
