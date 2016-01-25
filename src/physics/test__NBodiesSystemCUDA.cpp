
#include "NBodiesSystemCUDA.h"
#include "NBodiesSystem.h"
#include "../InitialState.h"



void test__NBodiesSYstemCUDA() {
	NBS_State state_0 = 
		NBS_InitialStates::random1();

	NBodiesSystemCUDA nbodiesCUDA( 
		state_0.D, state_0.N, 
		state_0.p_0, state_0.v_0, state_0.m_0, 
		0);

	NBodiesSystem nbodies( 
		state_0.D, state_0.N,
		state_0.p_0, state_0.v_0, state_0.m_0);

	time_type delta_t = 0.3f;

	for (int i = 0; i < 100; ++i)
		nbodiesCUDA.step(delta_t);
	std::cout << "***** " << std::endl;
	for (int i = 0; i < 100; ++i)
		nbodies.step(delta_t);
}

#ifdef CLASS_TEST
int main(int argc, char const *argv[])
{
	test__NBodiesSYstemCUDA();
	return 0;
}
#endif