#ifndef N_BODIES_SYSTEM_VISUALIZATION_BRIDGE_HEADER
#define N_BODIES_SYSTEM_VISUALIZATION_BRIDGE_HEADER

#if defined(__APPLE__) || defined(MACOSX)
	#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h> // GLFW helper library
#include "../physics/NBodiesSystem.h"

#include <iostream>
#include "../InitialState.h"
#include "GLBridgeInterface.h"



class GLBridgeCUDA: 
	public GLBridgeInterface 
{
public:
	GLBridgeCUDA(
		NBS_State& initialState):
		state_0(initialState) 
	{
		vbo = 0;
		vao = 0;

		this->N = initialState.N;
		this->D = initialState.D;
	}

	void initOpenGLPart () {
		glGenBuffers (1, &vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glBufferData (GL_ARRAY_BUFFER, 
			D * N * sizeof (GLfloat), 
			0, GL_DYNAMIC_DRAW);
		glGenVertexArrays (1, &vao);
		glBindVertexArray (vao);
		glEnableVertexAttribArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer (0, 3, GL_FLOAT, 
			GL_FALSE, 0, NULL);

		printf("vbo %d\n", vbo);

		this->nbodiesSystem = 
			new NBodiesSystemCUDA(
				state_0.D, state_0.N, 
				state_0.p_0, state_0.v_0, 
				state_0.m_0,
				vbo);
	}

	void stepPositions (time_type delta) {
		nbodiesSystem->step(delta);

		glBindBuffer (GL_ARRAY_BUFFER, vbo);
		// glBindVertexArray (vao);
		// glEnableVertexAttribArray (0);
		// glVertexAttribPointer (0, 3, GL_FLOAT, 
		// 	GL_FALSE, 0, NULL);
	}

	int getN() { return N; }
	int getD() { return D; }

private:
	NBodiesSystemCUDA* nbodiesSystem;
	NBS_State& state_0;
	GLuint vbo;
	GLuint vao;
	unsigned N;
	unsigned D;
};

#endif