#ifndef N_BODIES_SYSTEM_VISUALIZATION_BRIDGE_HEADER
#define N_BODIES_SYSTEM_VISUALIZATION_BRIDGE_HEADER

#if defined(__APPLE__) || defined(MACOSX)
	#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h> // GLFW helper library
#include "../physics/NBodiesSystem.h"

#include <iostream>



class GLVisualization_NBodiesSystem {
public:
	GLfloat* points;
	GLuint vbo;
	GLuint vao;

	void zip_modelPositions_to_points() {
		Params<position_type>* model_positions = nbodiesSystem->getPositions();

		int ipoints = 0;
		for (int i = 0; i < N; ++i)
			for (int d = 0; d < D; ++d) {
				points[ipoints++] = 
					(*model_positions).getVal(d, i);
			}
	}

	GLVisualization_NBodiesSystem(
		NBodiesSystem* nbodiesSystem,
		unsigned D, unsigned N ) 
	{
		points = NULL;
		vbo = 0;
		vao = 0;

		this->nbodiesSystem = nbodiesSystem;
		this->N = N;
		this->D = D;
		this->points = new GLfloat[N*D];

		zip_modelPositions_to_points();
	}

	void initOpenGLPart () {
		glGenBuffers (1, &vbo);
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
	}

	~GLVisualization_NBodiesSystem() { delete [] this->points; }

	void stepPositions (time_type delta) {
		nbodiesSystem->step(delta);
		zip_modelPositions_to_points();
		initOpenGLPart();
	}

	unsigned getN() { return N; }
	unsigned getD() { return D; }

private:
	unsigned N;
	unsigned D;
	NBodiesSystem* nbodiesSystem;
};

#endif