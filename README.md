This document contains all the information pertaining to the rewrite of this project.

This project is originally the semester project for Systems Programming in C (CS 360 at WSU). The goal of this project is to create a functional *terminal shell* simulator. This shell includes basic commands like `ls`, `cd`, `mkdir`, `touch` and so on. This shell is also capable of simple process management; the shell should be able to fork and execute a command.

I struggled a lot while working on this project at the time for two reasons:

1. My attention was elsewhere; I was focusing on learning front-end web development,
	- which requires a very different mode of thinking, and
	- which hogs a lot of my limited attentional resource.
2. I decided to go solo working on the project.

Almost as a tradition, bits and pieces start to fall into place and the model becomes clearer for me around 6 months after the experience. This is I want to do a rewrite for this project. This time, I want to accomplish two goals:

- remap a good model of the project, with well documented notes and graphs.
- rewrite this project in Rust.

This document will facilitate this entire process.


### The problem previously

While working on the project, there was not very clear direction on what I was supposed to do.

There was too much information required for the project, and that was just the tip of the iceberg. Furthermore, the pieces were scattered and not connected in a meaningful way. Considering how complex systems programming is, I believe very strongly that I would have had a much better learning experience if the project had been laid out model-first at the beginning of the semester. For learning, a top-down approach keeps the eyes on the ball. Once the model has been *cached*, we would use the enter semester to build up to that goal by implementing the pieces.

Unfortunately that was not my experience. I suffered trying to understand the goal while implementing the pieces in the dark.

This experience gave me PTSD. It turned me away from the subject of systems programming, which I actually enjoy because I enjoy the process of thinking about and modeling systems.


### The plan

At the moment, there are a few steps I have in mind.

1. Review and analyze the source code.
	- Map out major components of the system.
		- a virtual disk, a shell that mounts and runs on that virtual disk.
		- within the shell, the user can execute terminal commands which are done via system calls.
		- the shell should have a process management scheme to handle forking new processes.
			- I considered a circular queue while working on this project back then but never got to work on that idea.
	- Map these points to data structures and functions in the old source code.
2. Develop a design/model for this system.
	- This model should be documented in details and with graph to illustrate and clarify where neccessary.
	- Refers to the Linux Programming handbook and KC's own book for references on pre-requisites.
		- How to make a vdisk.
		- How system calls are executed.
		- Identify problems in KC's source code (a global namespace highly polluted with global variables where they get mutated with no way to track).
3. Develop a plan to rewrite in Rust.
	- *Develop a basic proficiency in Rust* needed to mount the vdisk and implement the `ls` function.
		- This requires a working understanding of Rust's fundamentals like
			- *data types*,
			- *functions*,
			- *macros*,
			- *structs*,
			- *traits* and
			- the *ownship model*,
			- including *wrap* and *unwrap*.
	- Continue to implement other functionality and features, such as process and thread management.

#### details

0. Analyze and model.
1. Start with the vdisk's `struct`, because `ls` will access its structural attributes.
2. Implement `ls` such that it correctly query information about a file and a directory node.

