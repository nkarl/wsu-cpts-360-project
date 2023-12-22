
This document contains information about Makefile and examples from basics to moderate level of complexity.

I actually had fun digging into the [GNU Make documentation](https://www.gnu.org/software/make/manual/make.html#Introduction) and build up a simple build system for myself back in the days. Unfortunately, I didn't document the process so I lost the knowledge on how to set up and adjust the build system. This knowledge is important because no two projects are the same. Knowing the rules and what to change means that I can adapt the build system to a new project.

In order to understand the Make build system, context is very important. The context here is that we need to *compile* C/C++ code into a executable binary.

## Primer

```mermaid
graph LR
code[source code] --> bin["executable (a binary file)"]
```

That is what we want to do. However, this graph abstract away the steps required to get to the binary from the source code. The steps are *compiling* and *linking*.

```mermaid
graph LR
code[source code] -->|compiled| objects["object(s)"] -->|linked| bin["executable"]
```

For a project of moderate complexity, we can go straight from source code to executable. However, as the project grows we eventually need to use external code (libraries). Ain't nobody got the time to do everything by themselves; that's a fool's errand. The general use case is we take code from somewhere else and drop in our project with very little to no changes. In fact, this is how all impactful projects including the GCC standard library, the Linux kernel and Git came to be. We need a streamlined process to accomplish this goal: compile different code separately and then link them with our existing code. GNU Make addresses this purpose.

## Using GNU Make

### A Simple Case

We make an executable `edit`, which depends on 8 objects, which themselves depend on 8 C sources and 3 headers.

```Make
edit : main.o kbd.o command.o display.o \
       insert.o search.o files.o utils.o
        cc -o edit main.o kbd.o command.o display.o \
                   insert.o search.o files.o utils.o

main.o      : main.c    defs.h
        cc -c main.c
kbd.o       : kbd.c     defs.h command.h
        cc -c kbd.c
command.o   : command.c defs.h command.h
        cc -c command.c
display.o   : display.c defs.h buffer.h
        cc -c display.c
insert.o    : insert.c  defs.h buffer.h
        cc -c insert.c
search.o    : search.c  defs.h buffer.h
        cc -c search.c
files.o     : files.c   defs.h buffer.h command.h
        cc -c files.c
utils.o     : utils.c   defs.h
        cc -c utils.c
clean       :
        rm edit main.o \
            kbd.o command.o display.o \
            insert.o search.o files.o utils.o

```


### Less Simple

### More Complex

