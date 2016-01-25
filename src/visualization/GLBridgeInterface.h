#ifndef GL_BRIDGE_INTERFACE_HEADER
#define GL_BRIDGE_INTERFACE_HEADER

#include "../types.h"


struct GLBridgeInterface {
	virtual void initOpenGLPart () = 0;
	virtual void stepPositions (time_type delta) = 0;
	virtual int getN() = 0;
};

#endif