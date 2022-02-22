Semir Smajlovic
CMPSCI 4760
Project 2

Task:
Generate twenty processes using a master program, called master, and make them write into a file called cstest in their current working directory. Needless to say that all processes will use the same working directory. Each child process will be executed by an executable called slave. The message to be written into the file is HH:MM:SS Queue nn File modified by process number xx.

HH:MM:SS - Current system time.
nn - Number of process picked when it entered its intention to write into the file.
xx - Process number as specified by the master program. (The value of xx is 1 and 20).

Child commands will be run by command: slave xx.