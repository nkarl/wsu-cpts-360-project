
This project is a system with two major components:

1. a Virtual Disk (vdisk)
2. the Shell program that mounts and loops on that vdisk.


## The Virtual Disk

- The virtual disk can be created via a command line (retrieves reference from KC's book).

- The virtual disk needs to be modeled into EX2 structures in C (retrieves references from KC's book).
	- These structures will hold information pertaining to records saved as data blocks on the vdisk.

## The Shell

- The Shell is a runtime loop.
- It will always wait for a commandline from the user, execute and loop back to wait again.

