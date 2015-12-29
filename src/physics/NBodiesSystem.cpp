#include "NBodiesSystem.h"
#include <cmath>


const position_type NBodiesSystem::G = 1;
const position_type NBodiesSystem::efactor = 0.1;

NBodiesSystem::NBodiesSystem(
	unsigned D, unsigned N, 
	Params<position_type>& p_0,
	Params<position_type>& v_0,
	Params<mass_type>& m_0
	):
		p_prev(D, N), p_curr(D, N), 
		v_prev(D, N), v_curr(D, N),
		a(D, N), m(1, N)	
{
	this->D = D;
	this->N = N;
	p_prev = p_0;
	p_curr = p_0;
	v_prev = v_0;
	v_curr = v_0;
	m = m_0;

	step_a();
}

void NBodiesSystem::step_a () {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if ( i == j ) continue;

			position_type* r_axis = new position_type[D];
			position_type r_squared = 0;
			for (int d = 0; d < D; ++d) {
				r_axis[d] = (p_curr[d][i] - p_curr[d][j]);
				r_squared += 
					r_axis[d] * r_axis[d];
			}

			position_type a_scalar = G * m[0][j] / pow(r_squared + efactor, 1.5);

			for (int d = 0; d < D; ++d)
				a[d][i] = -1 * a_scalar * (r_axis[d]/r_squared);

			delete [] r_axis;
		}
	}
}

void NBodiesSystem::step( time_type delta_t ) {
	p_prev = p_curr;
	v_prev = v_curr;

	for (int d = 0; d < D; ++d)
		for (int i = 0; i < N; ++i) {
			v_curr[d][i] = v_prev[d][i]  +  a[d][i] * delta_t;
			p_curr[d][i] = p_prev[d][i]  +  
				(v_prev[d][i] + v_curr[d][i]) * 0.5 * delta_t;
			if (p_curr[d][i] > 1 || p_curr[d][i] < -1) v_curr[d][i] = -v_curr[d][i];
		}

	step_a();
} 

Params<position_type>* NBodiesSystem::getPositions() {
	return &p_curr;
}

void NBodiesSystem::printPositions() {
	using namespace std;
	cout << "Position" << endl;
	cout << "\t" << p_curr << endl << endl ; 
}
