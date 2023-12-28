##################################################################
## MAKEFILE                                                    ##
##################################################################
## project directories
hdr             := src/hdr
src             := src

# build main app
##################################################################
target_main     := build/main

src_lib         := src/lib
build_lib     	:= ${target_main}/lib
build_main      := ${target_main}

build_lib_o     := utils.o

build_lib_o     := ${addprefix ${build_lib}/, ${build_lib_o}}

app             := ${target_main}/app

# build unit tests and runner
##################################################################
target_test     := build/test

src_test_units  := src/test-units
build_units     := ${target_test}/units
build_runner    := ${target_test}

test_units_o    := fs.ext2.o fs.ext2.imap.o fs.ext2.imap.bits.o fs.ext2.inode_table.o

test_units_o    := ${addprefix ${build_units}/, ${test_units_o}}

test_runner     := ${target_test}/runner

## compiler flags
##################################################################
CXX             := g++ # run make CXX=clang

COMPILER_FLAGS  := -I${hdr} -Wall -Wextra -Wconversion -Wpedantic
LINKER_FLAGS    :=

MSG_GCC         := -DMESSAGE="Compiled with GCC"
MSG_CLG         := -DMESSAGE="Compiled with Clang"


ifeq (${CXX}, g++)
	OMPF         := -fopenmp
	LINKER_FLAGS += ${MSG_GCC} ${OMPF}
else ifeq (${CXX}, clang++)
	#OMPF         :=
	LINKER_FLAGS += ${MSG_CLG} ${OMPF} 
endif

##################################################################
## BUILD RECIPES                                               ##
##################################################################

.PHONY: app
##################################################################
app: ${app}

# 3. link *.o into a binary app.
${app}: ${build_lib_o} build/main/app.o
	${CXX} ${LINKER_FLAGS} $^ -o $@

# 2. compile the main code. 
${build_main}/%.o: src/main/%.cc | setup build_lib
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

.PHONY: test
##################################################################
test: ${test_runner}

# 3. link *.o into a binary app.
${test_runner}: ${build_lib_o} ${test_units_o} build/test/runner.o
	${CXX} ${LINKER_FLAGS} $^ -o $@

# 2. compile the runner code. 
${build_runner}/%.o: src/test/%.cc | setup
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

# 2. compile the unit test code.
${build_units}/%.o: ${src_test_units}/%.cc | setup build_lib
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

.PHONY: build_lib
##################################################################
build_lib:
# 1. compile the lib code.
${build_lib}/%.o: ${src_lib}/%.cc | setup # the `|` and the followed argument(s) signifies required step(s)
	${CXX} ${COMPILER_FLAGS} -c $^ -o $@

.PHONY: setup
##################################################################
setup:
# 0. set up the build directories
	$(info $(shell mkdir -p ${build_lib} ${build_units}))

##################################################################
## CLEAN TASK                                                  ##
##################################################################
.PHONY: clean
clean:
	rm -r build/

