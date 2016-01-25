#include <iostream>
#include "InitialState.h"


void printState( NBS_State& state ) {
	using namespace std;
	state.p_0.printAllElements();
	cout << endl;
	state.v_0.printAllElements();
	cout << endl;
	state.m_0.printAllElements();
	cout << endl;
}

void test__InitialState() {
	NBS_State state1 = 
		NBS_InitialStates::random1();
	printState(state1);

	NBS_State state2 = 
		NBS_InitialStates::_2bodies();
	printState(state2);
}

#ifdef CLASS_TEST
int main(int argc, char const *argv[])
{
	test__InitialState();
	return 0;
}
#endif