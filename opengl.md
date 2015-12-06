# Common includes and paths for CUDA
INCLUDES_OPENGL  := -I/usr/local/include/
LIBRARIES_OPENGL :=

################################################################################

SAMPLE_ENABLED := 1

# OpenGL specific libraries
ifeq ($(TARGET_OS),darwin)
	# Mac OSX specific libraries and paths to include
	LIBRARIES_OPENGL += -L/System/Library/Frameworks/OpenGL.framework/Libraries
	LIBRARIES_OPENGL += -lGL $(CUDA_PATH)/samples/common/lib/darwin/libGLEW.a 
	ALL_LDFLAGS += -lglfw -Xlinker -framework -Xlinker OpenGL
else
	LIBRARIES_OPENGL += -lGL -lGLEW
endif