#include "NBodiesSystem.h"


int main(int argc, char const *argv[])
{
	Params<position_type> 	p_0(2,2);
	Params<position_type>	v_0(2,2);
	Params<mass_type> 		m_0(1,2);

	m_0[0][0] = 1;
	m_0[0][1] = 1;

	p_0[X][0] = -1;
	p_0[Y][0] = -1;
	//-
	p_0[X][1] = 1;
	p_0[Y][1] = 1;

	v_0[X][0] = 0.1;
	v_0[Y][0] = 0;
	//-
	v_0[X][1] = -0.1;
	v_0[Y][1] = 0;

	NBodiesSystem nbodies( 2, 2, p_0, v_0, m_0);

	time_type delta_t = 0.3;

	for (int i = 0; i < 5; ++i)
	{
		nbodies.printPositions();
		nbodies.step(delta_t);
	}

	return 0;
}