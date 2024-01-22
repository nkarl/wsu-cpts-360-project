This document contains all the information pertaining to the rewrite of this project.

This project is originally the semester project for Systems Programming in C (CS 360 at WSU). The goal of this project is to create an EXT2 filesystem. A set of commands are required to interface with the filesystem, including  `ls`, `cd`, `mkdir`, `touch` and so on. These commands will later be integrated into a shell program. This project also includes simple process management; the shell should be able to fork and execute a command.


### The problem previously

While working on the project, there was not very clear direction on what I was supposed to do.

There was too much information required for the project, and that was just the tip of the iceberg. Furthermore, the pieces were scattered and not connected in a meaningful way. Considering how complex systems programming is, I believe very strongly that I would have had a much better learning experience if the project had been laid out model-first at the beginning of the semester. For learning, a top-down approach keeps the eyes on the ball. Once the model has been *cached*, we would use the enter semester to build up to that goal by implementing the pieces.

Unfortunately that was not my experience. I suffered trying to understand the goal while implementing the pieces in the dark.

This experience gave me PTSD. It turned me away from the subject of systems programming, which I actually enjoy because I enjoy the process of thinking about and modeling systems.


## Instructions to Begin Developing

Clone the repo and the `app-c` submodule. Ignore the `app-rs` module.

```sh
git clone https://github.com/nkarl/wsu-cpts-360-project.git
```
