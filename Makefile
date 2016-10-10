CC = g++ -g
CC_FLAGS = -Wall -pedantic
INCL = -I`pkg-config --cflags glfw3` 
LIBS = -L `pkg-config --libs --static glfw3` `pkg-config --libs glew`

sources = $(shell find src \
	-path src/__experiments -prune -o \
	-name *.cpp \
	-not -name *CUDA* -not -name test* \
	-not -type d -print \
)
objects = $(sources:.cpp=.o)



echoConfig:
	@echo "CC <- $(CC)"; echo "INCL <- $(INCL)"; echo "LIBS <- $(LIBS)";

echoSources:
	@echo $(sources)

echoObjects:
	@echo $(objects)

echoFilenames:
	@echo $(filenamesonly)



all: $(objects)
	$(CC) $(CC_FLAGS) $(INCL) -o $@ $+ $(LIBS)

%.o: %.cpp
	$(CC) $(CC_FLAGS) $(INCL) -o $@ -c $<

clear:
	find . -name *.o -delete; rm all;
