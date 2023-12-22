##################################################################
## MAKEFILE                                                     ##
##################################################################
## project directories
hdr       			:= src/hdr
src         		:= src

lib             := src/lib
build_lib     	:= build/lib
build_main      := build/main
## compiled and assembled object output directory
build_lib_o     := utils.o add.o sub.o
build_lib_o     := ${addprefix ${build_lib}/, ${build_lib_o}}
## binary output directory
app 						:= build/main/app

tests           := src/tests
build_tests     := build/tests
build_runner    := build/test
test_units_o    := fs.ext2-validation.o sample.arithmetic-operations.o
test_units_o    := ${addprefix ${build_tests}/, ${test_units_o}}
test_runner 		:= build/test/runner

## compiler flags
CXX         		:= g++ # run make CXX=clang

COMPILER_FLAGS  := -I${hdr}
LINKER_FLAGS    :=

MSG_GCC    			:= -DMESSAGE="Compiled with GCC"
MSG_CLG    			:= -DMESSAGE="Compiled with Clang"


ifeq (${CXX}, gcc)
	OMPF         := -fopenmp
	LINKER_FLAGS += ${MSG_GCC} ${OMPF}
else ifeq (${CXX}, clang)
	#OMPF         :=
	LINKER_FLAGS += ${MSG_CLG} ${OMPF} 
endif

##################################################################
## BUILD DIRECTIVE                                              ##
##################################################################
# TODO: figure out a way to select build target, aka either prod app or test runner. 
#
#.PHONY: target_app
	#make app > target_app

#.PHONY: target_test
	#make runner > target_test

.PHONY: app
app : ${app}

# 3. link *.o into a binary app.
${app} 					  : ${build_lib_o} build/main/app.o
	${CXX} ${LINKER_FLAGS} $^ -o $@

# 2. compile the main code. 
${build_main}/%.o : src/main/%.cc | setup
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

# 1. compile the lib code.
${build_lib}/%.o  : ${lib}/%.cc | setup
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

.PHONY: test
test 							: ${test_runner}

# 3. link *.o into a binary app.
${test_runner} 	  : ${build_lib_o} ${test_units_o} build/test/runner.o
	${CXX} ${LINKER_FLAGS} $^ -o $@

# 2. compile the runner code. 
${build_runner}/%.o : src/test/%.cc | setup
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

# 1. compile the unit test code.
${build_tests}/%.o  : ${tests}/%.cc | setup
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

# 1. compile the lib code.
${build_lib}/%.o  : ${lib}/%.cc | setup # the `|` and the followed argument(s) signifies required step(s)
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

.PHONY: setup
setup:
# 0. set up the build directories
	$(info $(shell mkdir -p ${build_lib} build/main/ build/test/ build/tests/))

##################################################################
## CLEAN TASK                                                   ##
##################################################################
.PHONY: clean
clean:
	rm -r build/

