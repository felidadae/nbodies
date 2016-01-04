



#-------------------------------------------------------------
include cuda.md
include opengl.md

DIR_BIN := bin
SHELL := /bin/bash

all: nbodies
doc: doc/report.pdf
doc/report.pdf: doc/report.tex
	pdflatex doc/report.tex
#-------------------------------------------------------------



#-------------------------------------------------------------
#***
# Class tests
#***
CLASS_TEST_FLAG := -DCLASS_TEST

#Params
bin/.tests/test__params: build/.testsfiles/test__params.o
	$(EXEC) $(NVCC) $(ALL_CCFLAGS) -o $@ $+
build/.testsfiles/test__params.o: src/physics/test__params.cpp
	$(EXEC) $(NVCC) $(CLASS_TEST_FLAG) $(ALL_CCFLAGS) -o $@ -c $<; \
	echo "Class tests Params";
#-------------------------------------------------------------



#-------------------------------------------------------------
#***
# PHYSICS ONLY
#***

DIR_SRC_PHYSICS := src/physics
DIR_BUILD_PHYSICS := build/physics
INCLUDE_PHYSICS :=
SRC_PHYSICS := $(shell find $(DIR_SRC_PHYSICS) -type f -name '*.cpp' | perl -ne 'print if $$_ !~ /test/' )
OBJ_PHYSICS := $(SRC_PHYSICS:$(DIR_SRC_PHYSICS)/%.cpp=$(DIR_BUILD_PHYSICS)/%.o)

physics-only: $(DIR_BIN)/nbodies_physicsOnly

$(DIR_BIN)/nbodies_physicsOnly: $(OBJ_PHYSICS) $(DIR_BUILD_PHYSICS)/test__physics.o
	$(EXEC) $(NVCC) $(INCLUDE_PHYSICS) $(ALL_CCFLAGS) -o $@ $+

$(DIR_BUILD_PHYSICS)/%.o: $(DIR_SRC_PHYSICS)/%.cpp
	$(EXEC) $(NVCC) $(INCLUDE_PHYSICS) $(ALL_CCFLAGS) -o $@ -c $<; \
	echo "physics rule 2";
#-------------------------------------------------------------	



#-------------------------------------------------------------
#***
# VISUALIZATION
#***

DIR_SRC_VISUALIZATION := src/visualization
DIR_BUILD_VISUALIZATION := build/visualization
SRC_VISUALIZATION := $(shell find $(DIR_SRC_VISUALIZATION) -type f -name '*.cpp' | perl -ne 'print if $$_ !~ /test/')
OBJ_VISUALIZATION := $(SRC_VISUALIZATION:$(DIR_SRC_VISUALIZATION)/%.cpp=$(DIR_BUILD_VISUALIZATION)/%.o)

$(DIR_BIN)/nbodies_visualizationOnly: $(OBJ_VISUALIZATION) $(DIR_BUILD_VISUALIZATION)/test__visualization.o
	$(EXEC) $(NVCC) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ $+ $(LIBRARIES) $(LIBRARIES_OPENGL)

$(DIR_BUILD_VISUALIZATION)/%.o: $(DIR_SRC_VISUALIZATION)/%.cpp
	$(EXEC) $(NVCC) $(INCLUDES_OPENGL) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ -c $<; \
	echo "visualization rule 2";
#-------------------------------------------------------------



#-------------------------------------------------------------
#***
# Experiments
#***

bin/exp%: build/__experiments/opengl%.o build/visualization/GLShader.o
	$(EXEC) $(NVCC) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ $+ $(LIBRARIES) $(LIBRARIES_OPENGL)

build/__experiments/%.o: src/__experiments/%.cpp
	$(EXEC) $(NVCC) -I$(CUDA_PATH)/samples/common/inc $(INCLUDES_OPENGL) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ -c $<; \
	echo "experiment rule 2 cpp";

build/__experiments/%.o: src/__experiments/%.cu
	$(EXEC) $(NVCC) -I$(CUDA_PATH)/samples/common/inc $(INCLUDES_OPENGL) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ -c $<; \
	echo "experiment rule 3 cu";

#-------------------------------------------------------------



#-------------------------------------------------------------
#***
# ALL
#***

nbodies: bin/nbodies

bin/nbodies: $(OBJ_VISUALIZATION) $(OBJ_PHYSICS) build/main.o
	$(EXEC) $(NVCC) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ $+ $(LIBRARIES) $(LIBRARIES_OPENGL)

build/%.o: src/%.cpp
	$(EXEC) $(NVCC) $(INCLUDES_OPENGL) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ -c $<; \
	echo "all rule 2";

#-------------------------------------------------------------



#-------------------------------------------------------------
#***
.PHONY: clean physics-only visualization-only

clean:
	rm $(DIR_BUILD_VISUALIZATION)/*; rm $(DIR_BUILD_PHYSICS)/*; rm $(DIR_BIN)/*; \
	rm build/*.o; rm build/physics/*; rm build/visualization/*;

.PRECIOUS: $(DIR_BUILD_VISUALIZATION)/%.o $(DIR_BUILD_PHYSICS)/%.o
#***
#-------------------------------------------------------------


