#-------------------------------------------------------------
include cuda.md
include opengl.md

DIR_BIN := bin
#-------------------------------------------------------------



#-------------------------------------------------------------
#***
# PHYSICS ONLY
#***

DIR_SRC_PHYSICS := src/physics
DIR_BUILD_PHYSICS := build/physics
INCLUDE_PHYSICS :=
SRC_PHYSICS := $(shell find $(DIR_SRC_PHYSICS) -type f -name '*.cpp')
OBJ_PHYSICS := $(SRC_PHYSICS:$(DIR_SRC_PHYSICS)/%.cpp=$(DIR_BUILD_PHYSICS)/%.o)

physics-only: $(DIR_BIN)/nbodies_physicsOnly

$(DIR_BIN)/nbodies_physicsOnly: $(OBJ_PHYSICS)
	$(EXEC) $(NVCC) $(INCLUDE_PHYSICS) $(ALL_CCFLAGS) -o $@ $+

$(DIR_BUILD_PHYSICS)/%.o: $(DIR_SRC_PHYSICS)/%.cpp
	$(EXEC) $(NVCC) $(INCLUDE_PHYSICS) $(ALL_CCFLAGS) -o $@ -c $<; echo "";
#-------------------------------------------------------------	



#-------------------------------------------------------------
#***
# VISUALIZATION ONLY
#***

DIR_SRC_VISUALIZATION := src/visualization
DIR_BUILD_VISUALIZATION := build/visualization
SRC_VISUALIZATION := $(shell find $(DIR_SRC_VISUALIZATION) -type f -name '*.cpp')
OBJ_VISUALIZATION := $(SRC_VISUALIZATION:$(DIR_SRC_VISUALIZATION)/%.cpp=$(DIR_BUILD_VISUALIZATION)/%.o)

visualization-only: $(DIR_BIN)/nbodies_visualizationOnly

$(DIR_BIN)/nbodies_visualizationOnly: $(OBJ_VISUALIZATION)
	$(EXEC) $(NVCC) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ $+ $(LIBRARIES) $(LIBRARIES_OPENGL)

$(DIR_BUILD_VISUALIZATION)/%.o: $(DIR_SRC_VISUALIZATION)/%.cpp
	$(EXEC) $(NVCC) $(INCLUDES_OPENGL) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -o $@ -c $<; echo "";
#-------------------------------------------------------------



#-------------------------------------------------------------
#***
# ALL
#***

# nbodies: $(OBJ_VISUALIZATION) $(OBJ_PHYSICS)

#-------------------------------------------------------------



#-------------------------------------------------------------
#***
.PHONY: clean physics-only visualization-only

clean:
	rm $(DIR_BUILD_VISUALIZATION)/*; rm $(DIR_BUILD_PHYSICS)/*; rm $(DIR_BIN)/*;

.PRECIOUS: $(DIR_BUILD_VISUALIZATION)/%.o $(DIR_BUILD_PHYSICS)/%.o
#***
#-------------------------------------------------------------

