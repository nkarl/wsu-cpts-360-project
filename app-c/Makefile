##################################################################
## MAKEFILE                                                     ##
##################################################################
## project directories
hdr       			:= src/hdr
src         		:= src
obj_dir     		:= target/obj
bin_dir     		:= target/bin

## compiled and assembled object output directory
objects     		:= main.o add.o sub.o # add more *.o here
objects     		:= $(addprefix $(obj_dir)/, $(objects))

## binary output directory
app 						:= $(bin_dir)/app

## compiler flags
CXX         		:= g++ # run make CXX=clang

COMPILER_FLAGS  := -I$(hdr)
LINKER_FLAGS    :=

MSG_GCC    			:= -DMESSAGE="Compiled with GCC"
MSG_CLG    			:= -DMESSAGE="Compiled with Clang"


ifeq ($(CXX), gcc)
	OMPF         := -fopenmp
	LINKER_FLAGS += $(MSG_GCC) $(OMPF)
else ifeq ($(CXX), clang)
	#OMPF         :=
	LINKER_FLAGS += $(MSG_CLG) $(OMPF) 
endif


##################################################################
## BUILD DIRECTIVE                                              ##
##################################################################
# 3. link *.o into a binary app.
$(app) 					: $(objects)
	$(CXX) $(LINKER_FLAGS) $^ -o $@

# 2. compile the main code. 
$(obj_dir)/%.o 	: $(src)/%.c
	$(CXX) $(COMPILER_FLAGS) -c $^ -o $@

# 1. compile the library code.
$(obj_dir)/%.o  : $(src)/lib/%.c
	$(CXX) $(COMPILER_FLAGS) -c $^ -o $@


##################################################################
## CLEAN TASK                                                   ##
##################################################################
.PHONY: clean
clean:
	rm -r $(target) $(obj_dir)/*
