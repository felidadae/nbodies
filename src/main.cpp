#include "visualization/GLVisualization.h"
#include "physics/NBodiesSystem.h"

#include <cstdlib>


/*
	Random Number Generator for floating points number (float, double, etc)
*/
template<typename T>
struct RandFloatingNumber {
	RandFloatingNumber() {
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
	1 In the beginning, God created the heavens and the earth. 
	2 The earth was without form and void, 
	and darkness was over the face of the deep ...
*/
class God {
public:
	struct State {
		State(Params<position_type>& 	p_0, 
			Params<position_type>& 		v_0, 
			Params<mass_type>&			m_0):
				p_0(p_0), v_0(v_0), m_0(m_0) {}
		Params<position_type>& 	p_0; 
		Params<position_type>& 	v_0; 
		Params<mass_type>&		m_0;
	};
	struct Ranges {
		position_type 	min__p_0;
		position_type 	max__p_0;
		position_type 	min__v_0;
		position_type 	max__v_0;
		mass_type		min__m_0;
		mass_type		max__m_0;
	};

	God(unsigned D, unsigned N) {
		this->D = D;
		this->N = N;
	}

	void randInitialValues(State& state, Ranges& ranges, int steps) 
	{	
		//rand position and speed
		for (int d = 0; d < D; ++d)
			for (int i = 0; i < N; ++i) {
				state.p_0[d][i] = 
					randgen_position.randInRange(
						ranges.min__p_0, ranges.max__p_0, steps);
				state.v_0[d][i] = 
					randgen_position.randInRange(
						ranges.min__v_0, ranges.max__v_0, steps);
			}

		for (int i = 0; i < N; ++i) {
			state.p_0[2][i] = 0.0;
			state.v_0[2][i] = 0.0;
		}

		//rand mass
		for (int i = 0; i < N; ++i) {
				state.m_0[0][i] = 
					randgen_mass.randInRange(
						ranges.min__m_0, ranges.max__m_0, steps);
		}
	}

private:
	unsigned D, N;
	RandFloatingNumber<position_type> randgen_position;
	RandFloatingNumber<mass_type> randgen_mass;
};


void _NBodies() {
	unsigned D = 3;
	unsigned N = 5;	
	unsigned stepsNum = 10;
	Params<position_type> 	p_0(D,N);
	Params<position_type>	v_0(D,N);
	Params<mass_type> 		m_0(1,N);

	God god(D, N);
	God::State state(p_0, v_0, m_0);
	God::Ranges ranges;
	ranges.min__p_0 = -1;
	ranges.max__p_0 =  1;
	ranges.min__v_0 = -0.3;
	ranges.max__v_0 =  0.3;
	ranges.min__m_0 =  0.5;
	ranges.max__m_0 =  2;

	god.randInitialValues(state, ranges, stepsNum);
	
	NBodiesSystem nbodies( D, N, p_0, v_0, m_0);
	GLVisualization_NBodiesSystem nbodiesGLBridge(&nbodies, D, N);
	start(nbodiesGLBridge);
}

void _twoBodies() {
	unsigned N = 2, D = 3;

	Params<position_type> 	p_0(D,N);
	Params<position_type>	v_0(D,N);
	Params<mass_type> 		m_0(1,2);

	m_0[0][0] = 1;
	m_0[0][1] = 1;

	p_0[X][0] = -0.8;
	p_0[Y][0] = -0.8;
	p_0[Z][0] = 0;
	//-
	p_0[X][1] = 0.8;
	p_0[Y][1] = 0.8;
	p_0[Z][1] = 0;

	v_0[X][0] = 0.1;
	v_0[Y][0] = 0;
	v_0[Z][0] = 0;
	//-
	v_0[X][1] = -0.1;
	v_0[Y][1] = 0;
	v_0[Z][1] = 0;

	NBodiesSystem nbodies( D, N, p_0, v_0, m_0);
	// nbodies.step(delta_t);

	GLVisualization_NBodiesSystem nbodiesGLBridge(&nbodies, D, N);
	

	start(nbodiesGLBridge);
}

int main() {
	_NBodies();
	return 0;
}




