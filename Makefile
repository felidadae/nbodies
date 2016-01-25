#-------------------------------------------------------------
include cuda.md
include opengl.md

DIR_BIN := bin
DIR_BUILD := build
SHELL := /bin/bash

FLAGS := $(ALL_LDFLAGS) $(GENCODE_FLAGS)
INCLUDE := -I$(CUDA_PATH)/samples/common/inc $(INCLUDES_OPENGL)
LIBRARIES += $(LIBRARIES_OPENGL)
COMPILER := $(EXEC) $(NVCC)

# When you want also to compile 
# main funtion in a test source file;
CLASS_TEST_FLAG := -DCLASS_TEST
#-------------------------------------------------------------


#-------------------------------------------------------------
#***
# Documentation
#***
doc: doc/report.pdf
doc/report.pdf: doc/report.tex
	cd doc; pdflatex doc/report.tex
#-------------------------------------------------------------


#-------------------------------------------------------------
#***
# How to build any objective file
#***

BUILDA := $(COMPILER) $(INCLUDE) $(ALL_LDFLAGS) $(GENCODE_FLAGS) 

echoSettings:
	$(COMPILER) \
	$(INCLUDE)

build/test__%.o: src/physics/test__%.cpp
	$(BUILDA) $(CLASS_TEST_FLAG) -o $@ -c $<

build/%.o: src/physics/%.cu
	$(BUILDA) -o $@ -c $<

build/%.o: src/physics/%.cpp
	$(BUILDA) -o $@ -c $<

build/%.o: src/visualization/%.cpp
	$(BUILDA) -o $@ -c $<

build/%.o: src/%.cpp
	$(BUILDA) -o $@ -c $<


build/%.o: src/__experiments/%.cpp
	$(BUILDA) -o $@ -c $<
build/%.o: src/__experiments/%.cu
	$(BUILDA) -o $@ -c $<

#-------------------------------------------------------------


#-------------------------------------------------------------
#***
# Class tests
#***

#experiments
bin/exp%: build/opengl%.o build/GLShader.o
	$(EXEC) $(NVCC) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ $+ $(LIBRARIES) $(LIBRARIES_OPENGL)


#Params
bin/test__Params: build/test__Params.o
	$(COMPILER) $(ALL_CCFLAGS) -o $@ $+ $(LIBRARIES)

#InitialState
bin/test__InitialState: build/test__InitialState.o build/InitialState.o
	$(COMPILER) $(ALL_CCFLAGS) -o $@ $+ $(LIBRARIES)

#NBodiesSystemCUDA
bin/test__NBodiesSystemCUDA: build/test__NBodiesSystemCUDA.o build/NBodiesSystemCUDA.o build/NBodiesSystem.o build/InitialState.o
	$(COMPILER) $(ALL_CCFLAGS) -o $@ $+ $(LIBRARIES)
#-------------------------------------------------------------


#-------------------------------------------------------------
#***
# allCPU allCUDA
#***

allCPU: bin/allCPU
bin/allCPU: build/main.o build/InitialState.o build/NBodiesSystem.o build/GLVisualization.o build/GLShader.o
	$(COMPILER) $(FLAGS) -o $@ $+ $(LIBRARIES)

allCUDA: bin/allCUDA
bin/allCUDA: build/mainCUDA.o build/InitialState.o build/NBodiesSystemCUDA.o build/GLVisualization.o build/GLShader.o
	$(COMPILER) $(FLAGS) -o $@ $+ $(LIBRARIES)
#-------------------------------------------------------------


#-------------------------------------------------------------
#***
.PHONY: clean doc allCPU allCUDA echoSettings

clean:
	rm -r $(DIR_BIN)/*; rm -r $(DIR_BUILD)/*;

.PRECIOUS: $(DIR_BIN)/%.o $(DIR_BUILD)/%.o
#***
#-------------------------------------------------------------


