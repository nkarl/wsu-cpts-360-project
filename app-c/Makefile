##################################################################
## MAKEFILE                                                     ##
##################################################################
## project directories
hdr       			:= src/hdr
src         		:= src
lib             := src/lib
tests           := src/tests
obj_dir     		:= build/obj
bin_dir     		:= build/bin

## compiled and assembled object output directory
objects     		:= main.o utils.o add.o sub.o
objects     		:= ${addprefix ${obj_dir}/, ${objects}}

## binary output directory
app 						:= ${bin_dir}/app

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

# 3. link *.o into a binary app.
${app} 					: ${objects} 
	${CXX} ${LINKER_FLAGS} $^ -o $@

# 2. compile the main code. 
${obj_dir}/%.o 	: ${src}/%.cc
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

# 1. compile the lib test code.
${obj_dir}/%.o  : ${tests}/%.cc
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

# 1. compile the lib code.
${obj_dir}/%.o  : ${lib}/%.cc
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

# 0. set up the build directories
$(info $(shell mkdir -p ${obj_dir} ${bin_dir}))

##################################################################
## CLEAN TASK                                                   ##
##################################################################
.PHONY: clean
clean:
	rm -r ${build} ${obj_dir} ${bin_dir}

