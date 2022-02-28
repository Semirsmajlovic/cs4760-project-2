Semir Smajlovic
CMPSCI 4760
Project 2

Task:
Generate twenty processes using a master program, called master, and make them write into a file called cstest in their current working directory. Needless to say that all processes will use the same working directory. Each child process will be executed by an executable called slave. The message to be written into the file is HH:MM:SS Queue nn File modified by process number xx.

HH:MM:SS - Current system time.
nn - Number of process picked when it entered its intention to write into the file.
xx - Process number as specified by the master program. (The value of xx is 1 and 20).

Child commands will be run by command: slave xx.

Execution: 
1. Run 'make' to start execution process.
2. Then navigate to use './master -t {Timeout} {Number of Processes}
3. CTRL + C will exit the process

Issues that I've come across:
1. There are a lot of places where I've tried to properly disallocate the memory, but was having issues but I think I came across all the spotss.
2. I wanted to define a proper config.h file, but was having issues with some voids, so I included all of my includes in each file.