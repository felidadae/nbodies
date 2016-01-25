#include "NBodiesSystem.h"



void test__NBodiesSystem() {
	unsigned N = 2, D = 3;

	Params<float> 	p_0(D,N, ALIGN);
	Params<float>	v_0(D,N, ALIGN);
	Params<float> 	m_0(1,2, ALIGN);

	m_0.setVal(0, 0, 1);
	m_0.setVal(0, 1, 100);

	p_0.setVal(X, 0, -0.4);
	p_0.setVal(Y, 0, -0.4);
	p_0.setVal(Z, 0, 0);
	//-
	p_0.setVal(X, 1, 0.4);
	p_0.setVal(Y, 1, 0.4);
	p_0.setVal(Z, 1, 0);

	v_0.setVal(X, 0, 5.7);
	v_0.setVal(Y, 0, 0);
	v_0.setVal(Z, 0, 0);
	//-
	v_0.setVal(X, 1, -5.7);
	v_0.setVal(Y, 1, 0);
	v_0.setVal(Z, 1, 0);



	NBodiesSystem nbodies( D, N, p_0, v_0, m_0);

	time_type delta_t = 0.3;

	for (int i = 0; i < 5; ++i)
	{
		nbodies.step(delta_t);
		nbodies.printState();
	}
}



#ifdef CLASS_TEST
int main(int argc, char const *argv[])
{
	test__NBodiesSystem();
	return 0;
}
#endif