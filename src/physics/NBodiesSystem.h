#ifndef N_BODIES_SYSTEM_HEADER
#define N_BODIES_SYSTEM_HEADER

#include "Params.h"



/*
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

	@TODO
		foreach_el for NBodiesSystem::Params
		static cast values to position_type
		make G possible to change during runtime
*/
class NBodiesSystem {
public:
	static const position_type G;
	static const position_type efactor;

	NBodiesSystem(unsigned D, unsigned N, 
		Params<position_type>& p_0,
		Params<position_type>& v_0,
		Params<mass_type>& m_0
	);
	void step( time_type delta_t );
	Params<position_type>* getPositions();
	void printPositions();

private:
	unsigned D, N;
	Params<position_type> p_prev, p_curr, v_prev, v_curr, a;
	Params<mass_type> m;
	void step_a ();
};

#endif