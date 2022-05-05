#            Makefile for CPD OpenMP project
#
#     Author: Ana Martínez Saucedo (adapted from Norman Ramsey's compile script)
#
#  Maintenance targets:
#
#
#    all         - (default target) make sure everything's compiled
#    clean       - clean out all compiled object and executable files
#    integration - compile just the integration program
#    matrix      - compile just the matrix program.
#
#

# Executables to built using "make all"

OBJ = obj/
SRC = src/
BIN = bin/

EXECUTABLES = integration matrix

GCC = gcc

CFLAGS  = -fopenmp
LDFLAGS = -fopenmp

# 
#    'make all' will build all executables
#
#    Note that "all" is the default target that make will build
#    if nothing is specifically requested
#
all: $(EXECUTABLES)

# 
#    'make clean' will remove all object and executable files
#
clean:
	rm -f $(BIN)$(EXECUTABLES) $(OBJ)*.o


# 
#    To get any .o, compile the corresponding .c
#

$(OBJ)%.o : $(SRC)%.c
	mkdir -p $(OBJ)
	$(GCC) $(CFLAGS) -c $< -o $@

#
# Individual executables
#
#    Each executable depends on one or more .o files.
#    Those .o files are linked together to build the corresponding
#    executable.
#
integration: $(OBJ)integration.o
	mkdir -p $(BIN)
	$(GCC) $(LDFLAGS) -o $(BIN)integration $(OBJ)integration.o 

matrix: $(OBJ)matrix.o
	mkdir -p $(BIN)
	$(GCC) $(LDFLAGS) -o $(BIN)matrix $(OBJ)matrix.o 