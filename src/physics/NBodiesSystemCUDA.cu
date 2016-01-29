/*
	In this source file there is used macro ALIGN_INT which is define in Params.h;
*/

#include "NBodiesSystemCUDA.h"

#define SIZEF(x) (sizeof(float)*(x))
#define K 512
#define G 0.1f
#define efactor 0.1f





__device__ inline float getVal(float* data, int d, int i, int D, int N)
{
#if ALIGN_INT == INTERLEAVED_INT
	return data[i*D + d];
#endif
#if ALIGN_INT == SEPERATED_INT
	return data[d*N + i];
#endif
}

__device__ inline void setVal(float* data, int d, int i, float value, int D, int N)
{
#if ALIGN_INT == INTERLEAVED_INT
	data[i*D + d] = value;
#endif
#if ALIGN_INT == SEPERATED_INT
	data[d*N + i] = value;
#endif
}

#define GETVAL(data, d, i) getVal(data, d, i, D, N)
#define SETVAL(data, d, i, value) setVal(data, d, i, value, D, N)

__global__
static void stepKernel(
	/*Delta time*/
	float delta_t,

	/*Dimensions*/
	int N,
	int D,

	/*State*/
	float* p_prev,
	float* p_curr,
	float* v_prev,
	float* v_curr,
	float* a,
	float* m
) 
{	
	int i = threadIdx.x + blockIdx.x*blockDim.x;
	if ( i >= N ) return; 


	//--------------------------------------------------------------
	/*
		UPDATE v SPEED and p POSITION
	*/
	for (int d = 0; d < D; ++d) {
		SETVAL(v_curr, d, i, GETVAL(v_prev, d, i)  +  GETVAL(a, d, i) * delta_t);
		SETVAL(p_curr, d, i, GETVAL(p_prev, d, i)  +  (GETVAL(v_prev, d, i) + GETVAL(v_curr, d, i)) * 0.5 * delta_t);
		if (GETVAL(p_curr, d, i) > 1 || GETVAL(p_curr, d, i) < -1) 
			SETVAL(v_curr, d, i, -GETVAL(v_curr, d, i));
	}
	//--------------------------------------------------------------


	//--------------------------------------------------------------
	/*
		UPDATE a ACCELERATION
	*/
	for (int d = 0; d < D; ++d)
		SETVAL(a, d, i, 0.0f);

	for (int j = 0; j < N; ++j) {
		if ( i == j ) continue;

		/*
			delta X, delta Y, delta Z	
		*/
		position_type* r_axis = new position_type[D];
		
		position_type r_squared = 0.0f;
		for (int d = 0; d < D; ++d) {
			r_axis[d] = (GETVAL(p_curr, d, i) - GETVAL(p_curr, d, j));
			r_squared += r_axis[d] * r_axis[d];
		}

		position_type a_scalar = 
			G * GETVAL(m, 0, j) / pow(r_squared + efactor, 1.5);

		/* START DEBUG */
		// if ( a_scalar != a_scalar ) {
		// 	printf("\n");
		// 	printf("D <- %d\n", D);
		// 	printf("N <- %d\n", N);
		// 	printf("G <- %f\n", G);
		// 	printf("i <- %d\n", i);
		// 	printf("j <- %d\n", j);
		// 	printf("GETVAL(m, 0, j) <- %f\n", GETVAL(m, 0, j));
		// 	printf("GETVAL(p_curr, 0, i) <- %f\n", GETVAL(p_curr, 0, i));
		// 	printf("GETVAL(p_curr, 0, j) <- %f\n", GETVAL(p_curr, 0, j));
		// 	printf("GETVAL(p_curr, 1, i) <- %f\n", GETVAL(p_curr, 1, i));
		// 	printf("GETVAL(p_curr, 1, j) <- %f\n", GETVAL(p_curr, 1, j));
		// 	printf("GETVAL(p_curr, 2, i) <- %f\n", GETVAL(p_curr, 2, i));
		// 	printf("GETVAL(p_curr, 2, j) <- %f\n", GETVAL(p_curr, 2, j));
		// 	printf("r_squared <- %f\n", r_squared);
		// 	printf("efactor <- %f\n", efactor);
		// 	printf("\n");
		// }
		/* END__ DEBUG */
		

		for (int d = 0; d < D; ++d) {
			/*
				If both objects positions are the same
				there is division by zero; what to do then?
			 	I just set then acceleration to 0;
			*/
			if (r_axis[d])
			{
				SETVAL(a, d, i, 
					GETVAL(a, d, i) - 
						a_scalar * 
						(r_axis[d]/sqrt(r_squared)));
			}
		}

		delete [] r_axis;
	}
	//--------------------------------------------------------------
}





void NBodiesSystemCUDA::mapGLtoCUDA() {
	size_t num_bytes;
	checkCudaErrors(
    	cudaGraphicsMapResources(
    		1, &cuda_vbo__p_curr, 0));
    checkCudaErrors(
    	cudaGraphicsResourceGetMappedPointer(
    		(void **)&p_curr, &num_bytes, cuda_vbo__p_curr));
}
void NBodiesSystemCUDA::unmapGLtoCUDA() {
	checkCudaErrors(
    	cudaGraphicsUnmapResources(
    		1, &cuda_vbo__p_curr, 0));
}

NBodiesSystemCUDA::NBodiesSystemCUDA(
	unsigned D, unsigned N,
	Params<float>& 	p_0,
	Params<float>& 	v_0,
	Params<float>& 	m_0,
	GLuint vbo__p_curr)
{
	/*
		save OpenGL VBO of buffor for p_curr;
		register GL buffer for CUDA usage;
	*/
	this->N = N;
	this->D = D;
	this->vbo__p_curr = vbo__p_curr;

	//CUDA
	cudaGLSetGLDevice(0);
	//.	

	if (vbo__p_curr != 0) {
		printf("vbo__p_curr %d\n", vbo__p_curr);

		checkCudaErrors(
			cudaGraphicsGLRegisterBuffer(
				&cuda_vbo__p_curr, vbo__p_curr, 
				cudaGraphicsMapFlagsWriteDiscard));
		mapGLtoCUDA();
	}
	else {
		/*
			allocate memory on device
		*/
		checkCudaErrors(cudaMalloc((void **)&p_curr, SIZEF(D*N)));
	}

	/*
		allocate memory on device
	*/
	checkCudaErrors(cudaMalloc((void **)&p_prev, SIZEF(D*N)));
	checkCudaErrors(cudaMalloc((void **)&v_prev, SIZEF(D*N)));
	checkCudaErrors(cudaMalloc((void **)&v_curr, SIZEF(D*N)));
	checkCudaErrors(cudaMalloc((void **)&a, 	 SIZEF(D*N)));
	checkCudaErrors(cudaMalloc((void **)&m, 	 SIZEF(N  )));

	/*
		copy memory from host to device
	*/
	checkCudaErrors(cudaMemcpy(p_prev, p_0.data, SIZEF(D*N), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(p_curr, p_0.data, SIZEF(D*N), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(v_prev, v_0.data, SIZEF(D*N), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(v_curr, v_0.data, SIZEF(D*N), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(m,      m_0.data, SIZEF(N),   cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemset(a, 0, SIZEF(D*N)));
	checkCudaErrors(cudaDeviceSynchronize());

	if (vbo__p_curr != 0)
		unmapGLtoCUDA();
}

void NBodiesSystemCUDA::step( float delta_t ) {
	/**
		CUDA OpenGL interoperability; 
	**/
	if (vbo__p_curr != 0) 
		mapGLtoCUDA();

	/*
		Set curr to prev;

		p_prev = p_curr;
		v_prev = v_curr;
	*/
	checkCudaErrors(cudaMemcpy(p_prev, p_curr, SIZEF(D*N), cudaMemcpyDeviceToDevice));
	checkCudaErrors(cudaMemcpy(v_prev, v_curr, SIZEF(D*N), cudaMemcpyDeviceToDevice));

	/*
		1) UPDATE v SPEED and p POSITION

		For each body update v SPEED and p POSITION;
		N*D threads;


		2)UPDATE a ACCELERATION for each body;

		we have N bodies;
		square matrix;
		a_from_i_j
	*/
	stepKernel <<<(N+K-1)/K, K>>> (
		delta_t,
		N, D,
		p_prev, p_curr,
		v_prev, v_curr,
		a, m
	);

	checkCudaErrors(cudaDeviceSynchronize());

	#ifdef DEBUG_PRINT_POINTS_POSITIONS
	printState();
	#endif

	/**
		CUDA OpenGL interoperability; 
	**/
	if (vbo__p_curr != 0) 
		unmapGLtoCUDA();
}

#ifdef DEBUG_PRINT_POINTS_POSITIONS
void NBodiesSystemCUDA::printState() {
	Params<float> 	p__(D,N, ALIGN);
	Params<float>	v__(D,N, ALIGN);
	Params<float> 	a__(D,N, ALIGN);

	checkCudaErrors(cudaMemcpy(p__.data, p_curr, SIZEF(D*N), cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaMemcpy(v__.data, v_curr, SIZEF(D*N), cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaMemcpy(a__.data, a,		 SIZEF(D*N), cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaDeviceSynchronize());

	for (int i = 0; i < N; ++i) {
		printf("CUDABody(%d): \n", i);
		
		printf("\t p <- (");
		for (int d = 0; d < D; ++d)
			printf("%f ", p__.getVal(d, i));
		printf(")\n");

		printf("\t v <- (");
		for (int d = 0; d < D; ++d)
			printf("%f ", v__.getVal(d, i));
		printf(")\n");

		printf("\t a <- (");
		for (int d = 0; d < D; ++d)
			printf("%f ", a__.getVal(d, i));
		printf(")\n");
	}
	printf("\n");
}
#endif
