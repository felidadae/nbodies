#include "NBodiesSystem.h"
#include <cmath>


const position_type NBodiesSystem::G = 0.3;
const position_type NBodiesSystem::efactor = 0.1;

NBodiesSystem::NBodiesSystem(
	unsigned D, unsigned N, 
	Params<position_type>& p_0,
	Params<position_type>& v_0,
	Params<mass_type>& m_0
	):
		p_prev(D, N, ALIGN), p_curr(D, N, ALIGN), 
		v_prev(D, N, ALIGN), v_curr(D, N, ALIGN),
		a(D, N, ALIGN), m(1, N, ALIGN)	
{
	this->D = D;
	this->N = N;
	p_prev = p_0;
	p_curr = p_0;
	v_prev = v_0;
	v_curr = v_0;
	m = m_0;

	for (int d = 0; d < D; ++d)
		for (int i = 0; i < N; ++i)
			a.setVal(d, i, 0.0f);
}


void NBodiesSystem::step( time_type delta_t ) {
	p_prev = p_curr;
	v_prev = v_curr;

	//--------------------------------------------------------------
	/*
		UPDATE v SPEED and p POSITION
	*/

	for (int d = 0; d < D; ++d)
		for (int i = 0; i < N; ++i) {
			v_curr.setVal(d, i, v_prev.getVal(d, i)  +  a.getVal(d, i) * delta_t);
			p_curr.setVal(d, i, p_prev.getVal(d, i)  +  (v_prev.getVal(d, i) + v_curr.getVal(d, i)) * 0.5 * delta_t);
			if (p_curr.getVal(d, i) > 1 || p_curr.getVal(d, i) < -1) 
				v_curr.setVal(d, i, -v_curr.getVal(d, i));
		}
	//--------------------------------------------------------------
	

	//--------------------------------------------------------------
	/*
		UPDATE a ACCELERATION
		For each two bodies i,j where i != j;
	*/

	for (int d = 0; d < D; ++d)
		for (int i = 0; i < N; ++i)
			a.setVal(d, i, 0.0f);

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if ( i == j ) continue;

			/*
				delta X, delta Y, delta Z	
			*/
			position_type* r_axis = new position_type[D];
			
			position_type r_squared = 0;
			for (int d = 0; d < D; ++d) {
				r_axis[d] = (p_curr.getVal(d, i) - p_curr.getVal(d, j));
				r_squared += r_axis[d] * r_axis[d];
			}

			position_type a_scalar = 
				G * m.getVal(0, j) / pow(r_squared + efactor, 1.5);

			for (int d = 0; d < D; ++d) {
				/*
					If both objects positions are the same
					there is division by zero; what to do then?
				 	I just set then acceleration to 0;
				*/
				if (r_axis[d]) {
					a.setVal(d, i, 
						a.getVal(d,i) - 
							a_scalar * 
							(r_axis[d]/sqrt(r_squared)));
				}	
			}

			if ( a.getVal(X, i) !=  a.getVal(X, i)) {
				//**Breakpoint
				int k = 8;
			}

			delete [] r_axis;
		}
	}
	//--------------------------------------------------------------

	#ifdef DEBUG_PRINT_POINTS_POSITIONS
	printState();
	#endif
} 

Params<position_type>* NBodiesSystem::getPositions() {
	return &p_curr;
}

#ifdef DEBUG_PRINT_POINTS_POSITIONS
void NBodiesSystem::printState() {
	for (int i = 0; i < N; ++i) {
		printf("CPU_Body(%d): \n", i);
		
		printf("\t p <- (");
		for (int d = 0; d < D; ++d)
			printf("%f ", p_curr.getVal(d, i));
		printf(")\n");

		printf("\t v <- (");
		for (int d = 0; d < D; ++d)
			printf("%f ", v_curr.getVal(d, i));
		printf(")\n");

		printf("\t a <- (");
		for (int d = 0; d < D; ++d)
			printf("%f ", a.getVal(d, i));
		printf(")\n");
	}
	printf("\n");
}
#endif
