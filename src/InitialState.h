#ifndef RAND_INITIAL_STATE_HEADER
#define RAND_INITIAL_STATE_HEADER

#include <cstdlib>
#include <iostream>
#include <ctime>
#include "physics/Params.h"



/*
	Random Number Generator for 
	floating points number (float, double, etc)
*/
template<typename T>
struct FloatingNumberRandomGenerator {
	FloatingNumberRandomGenerator() {
		std::srand(std::time(0));
	}
	T randInRange (T min, T max, int steps /*how many steps per one unit*/) {
		int r = std::rand();
		int rangeInSteps = int((max - min) * T(steps));
		r = r % rangeInSteps;
		
		T tr = T(r)/T(steps);

		tr += min;
		return tr;
	}
};


/*
	p, v, m
*/
struct NBS_State {
	int D, N;
	Params<position_type> 	p_0;
	Params<position_type> 	v_0;
	Params<mass_type>		m_0;

	NBS_State(int D, int N);
};


class NBS_InitialStateGenerator {
public:

	/*
		Helper classes
	*/
	struct Ranges {
		position_type 	min__p_0;
		position_type 	max__p_0;
		position_type 	min__v_0;
		position_type 	max__v_0;
		mass_type		min__m_0;
		mass_type		max__m_0;
	};


	/*
		Return initial state (p,v,m) with random values
		in ranges given in >>ranges<< 
		with number of discrite values given in >>steps<<
	*/
	NBS_State rand(int D, int N, Ranges& ranges, int steps);

private:
	FloatingNumberRandomGenerator<position_type> 	
		randgen_position;
	FloatingNumberRandomGenerator<mass_type> 		
		randgen_mass;
};



namespace NBS_InitialStates {
	NBS_State random1();
	NBS_State _2bodies();
};


#endif