#include "visualization/GLVisualization.h"
#include "visualization/GLBridgeInterface.h"
#include "visualization/GLBridge.h"
#include "physics/NBodiesSystem.h"
#include "InitialState.h"



void printState( NBS_State& state ) {
	using namespace std;
	state.p_0.printAllElements("p_0");
	state.v_0.printAllElements("v_0");
	state.m_0.printAllElements("m_0");
	cout << endl;
}

int main() {
	NBS_State state_0 = 
		NBS_InitialStates::random1();

	//printState(state_0);
	// return 0;

	GLBridge
		nbodiesGLBridge(state_0);
	
	start(
		(GLBridgeInterface*) 
			&nbodiesGLBridge, 
		-1);
	return 0;
}
