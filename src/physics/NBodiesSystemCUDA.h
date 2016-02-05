//#define DEBUG_PRINT_POINTS_POSITIONS

#ifndef N_BODIES_SYSTEM_CUDA_HEADER
#define N_BODIES_SYSTEM_CUDA_HEADER

#ifdef __APPLE__
	#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	# define __gl_h_
	# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif

#if defined(__APPLE__) || defined(MACOSX)
	#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h> // GLFW helper library
#if defined(WIN32) || defined(_WIN32) \
	|| defined(WIN64) || defined(_WIN64)
	#include <GL/wglew.h>
#endif

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
// helper functions for CUDA error check
#include <helper_cuda.h>  
// helper functions for CUDA/GL interop       
#include <helper_cuda_gl.h>      

#include "Params.h"



/*
	@Description
		NBodiesSystem with CUDA backend;
		p, v, a, m are all 32 bits floats;

	@Abbreviations/naming scheme
		@D <- dimensions count (2 for 2d, 3 for 3d, etc.)
		@N <- bodies count

		@p <- position
		@v <- speed
		@a <- acceleration
		@t <- time
		@m <- mass
		@r <- distance between two points
		@r_axis <- distance per axis between two points

		@i in loop: always body index
		@j in loop: always body index
			all pairs: i == const, j from 0 to N
		@d in loop: always dimension index (x,y,z ...)

	@Contructor input variables note
		Class Params<float>
*/
class NBodiesSystemCUDA {
public:
	NBodiesSystemCUDA(unsigned D, unsigned N,
		Params<float>& 	p_0,
		Params<float>& 	v_0,
		Params<float>& 	m_0,
		GLuint vbo__p_curr
	);

	void step( float delta_t );

	#ifdef DEBUG_PRINT_POINTS_POSITIONS
	void printState();
	#endif

private:
	unsigned D, N;
	float	*p_prev, *p_curr,
			*v_prev, *v_curr,
			*a,
			*m;

	/**
		CUDA OpenGL interoperability; 

		p_curr data allocated on GPU
		is used both by CUDA and OpenGL
	**/			
	GLuint vbo__p_curr;
	struct cudaGraphicsResource 
		*cuda_vbo__p_curr;
	void mapGLtoCUDA();
	void unmapGLtoCUDA();
	
};

#endif