#include "InitialState.h"
#include <cmath>


NBS_State::NBS_State(int D, int N):
	p_0(D, N, ALIGN), 
	v_0(D, N, ALIGN), 
	m_0(1, N, ALIGN) 
{
	this->D = D;
	this->N = N;
}



NBS_State NBS_InitialStateGenerator::rand(
	int D, int N, 
	Ranges& ranges, 
	int steps
) 
{	
	NBS_State state(D, N);

	//rand position and speed
	for (int d = 0; d < D; ++d)
		for (int i = 0; i < N; ++i) {
			state.p_0.setVal(d, i, 
				randgen_position.randInRange(
					ranges.min__p_0, ranges.max__p_0, steps));
			state.v_0.setVal(d, i, 
				randgen_position.randInRange(
					ranges.min__v_0, ranges.max__v_0, steps));
		}

	for (int i = 0; i < N; ++i) {
		state.p_0.setVal(2, i, 0.0);
		state.v_0.setVal(2, i, 0.0);
	}

	//rand mass
	for (int i = 0; i < N; ++i) {
			state.m_0.setVal(0, i, randgen_mass.randInRange(
					ranges.min__m_0, ranges.max__m_0, steps));
	}

	return state;
}



NBS_State NBS_InitialStateGenerator::symetricalInCircle(
	int D, int N,
	position_type middleX,
	position_type middleY,
	position_type R
) 
{	
	NBS_State state(D, N);

	position_type radiusStep = 2 * M_PI / N;

	//	Rand position and speed
	for (int i = 0; i < N; ++i) {
		position_type alpha = i*radiusStep;

		position_type deltaX = sin(alpha) * R;
		position_type deltaY = cos(alpha) * R; 

		state.p_0.setVal(X, i, deltaX + middleX);
		state.p_0.setVal(Y, i, deltaY + middleY);
	}

	//	For 3 dimensions zeros
	for (int i = 0; i < N; ++i) {
		state.p_0.setVal(Z, i, 0.0);
		state.v_0.setVal(Z, i, 0.0);
	}

	//	Mass
	for (int i = 0; i < N; ++i)
			state.m_0.setVal(0, i, 1.0f);

	return state;
}




NBS_State NBS_InitialStates::random1() {
	unsigned D = 3;
	unsigned N = 120;
	unsigned stepsNum = 30;
	NBS_InitialStateGenerator god;
	NBS_InitialStateGenerator::Ranges ranges;
	ranges.min__p_0 = -0.1;
	ranges.max__p_0 =  0.1;
	ranges.min__v_0 = -0.1;
	ranges.max__v_0 = 0.1;
	ranges.min__m_0 =  0.5;
	ranges.max__m_0 =  2;

	return 
		god.rand(
			D, N, ranges, stepsNum);
}

NBS_State NBS_InitialStates::inCircle1() {
	NBS_InitialStateGenerator god;

	int D = 3;
	int N = 8*4;
	position_type middleX = 0.0f;
	position_type middleY = 0.0f;
	position_type R = 0.5f;

	return god.symetricalInCircle(
		D, N,
		middleX,
		middleY,
		R
	);
}

NBS_State NBS_InitialStates::_2bodies() {
	unsigned N = 2, D = 3;
	NBS_State state(D, N);

	state.m_0.setVal(0, 0, 1);
	state.m_0.setVal(0, 1, 100);

	state.p_0.setVal(X, 0, -0.4);
	state.p_0.setVal(Y, 0, -0.4);
	state.p_0.setVal(Z, 0, 0);
	//-
	state.p_0.setVal(X, 1, 0.4);
	state.p_0.setVal(Y, 1, 0.4);
	state.p_0.setVal(Z, 1, 0);

	state.v_0.setVal(X, 0, 5.7);
	state.v_0.setVal(Y, 0, 0);
	state.v_0.setVal(Z, 0, 0);
	//-
	state.v_0.setVal(X, 1, -5.7);
	state.v_0.setVal(Y, 1, 0);
	state.v_0.setVal(Z, 1, 0);

	return state;
}