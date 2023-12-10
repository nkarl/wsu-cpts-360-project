##################################################################
## MAKEFILE                                                     ##
##################################################################
headers         := hdr
source          := src
obj_dir         := target/obj
bin_dir         := target/bin

objects         := main.o # add more *.o here
objects         := $(obj_dir)/$(objects)

app             := app
target 			:= $(bin_dir)/$(app)

CXX             := gcc # run make CXX=clang

COMPILER_FLAGS  := -I$(headers) -g \
					-Wall -Wextra -Wconversion -pedantic

MSG_GCC    		:= -DMESSAGE="Compiled with GCC"
MSG_CLG    		:= -DMESSAGE="Compiled with Clang"


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
all 			: $(target)

$(target) 		: $(objects)
	#
	# 3. link *.o into a binary app.
	#
	@if [ ! -d "$(bin_dir)" ]; then mkdir -p $(bin_dir); fi
	$(CXX) $(LINKER_FLAGS) $^ -o $@

$(obj_dir)/%.o 	: $(source)/%.c
	#
	# 1. set up project structure.
	#
	@if [ ! -d "$(obj_dir)" ]; then mkdir -p $(obj_dir); fi
	#
	# 2. compile and assemble but do not link yet.
	#
	$(CXX) $(COMPILER_FLAGS) -c $^ -o $@

##################################################################
## CLEAN TASK                                                   ##
##################################################################
.PHONY: clean
clean:
	rm -r $(target) $(obj_dir)/*
