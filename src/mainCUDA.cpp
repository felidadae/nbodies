#include "visualization/GLVisualization.h"
#include "physics/NBodiesSystemCUDA.h"
#include "InitialState.h"
#include "visualization/GLBridgeInterface.h"
#include "visualization/GLBridgeCUDA.h"


int main() {
	NBS_State state_0 = 
		NBS_InitialStates::random1();

	GLBridgeCUDA
		nbodiesGLBridge(state_0);
	
	start(
		(GLBridgeInterface*) 
			&nbodiesGLBridge, 
		-1);
	return 0;
}




