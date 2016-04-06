#Simple OS Simulator#

##The program simulates the cycle in which processes, in the form of PCBs, are cycled through a
ready queue, single CPU, and three different types of device queues (disk, printer, and CD).
There can be any number of device queues of these three types.##

##User Input##
The program will begin by asking the user to enter the number of printer, disk, and CD queues
they want to exist in this toy OS cycle.

After this, a user can enter various commands, listed below, to create or terminate new processes, 
and create system calls.

##'A'## : Signifies the arrival of a new process, which is created with a unique PID and added
to the ready queues.

##'t'## : Destroys the process occupying the CPU. If the ready queue is not empty, one will
enter the CPU to take the destroyed process' place.

##'p<num>'## : <num> must be a valid integer greater than 0 and less than the chosen number of
printer device queues. Adds a system call to the <num>th printer device queue after removing
the process occupying the CPU. 

##'c<num>'## : <num> must be a valid integer greater than 0 and less than the chosen number of
CD device queues. Adds a system call to the <num>th cd device queue after removing
the process occupying the CPU. 

##'d<num>'## : <num> must be a valid integer greater than 0 and less than the chosen number of
disk device queues. Adds a system call to the <num>th disk device queue after removing
the process occupying the CPU.

##'P<num>'## : <num> must be a valid integer greater than 0 and less than the chosen number of
printer device queues. Removes the system call at the front of the <num>th printer device 
queue. The corresponding PCB is added to the back of the ready queue. 

##'C<num>'## : <num> must be a valid integer greater than 0 and less than the chosen number of
CD device queues. Removes the system call at the front of the <num>th CD device 
queue. The corresponding PCB is added to the back of the ready queue.

##'D<num>'## : <num> must be a valid integer greater than 0 and less than the chosen number of
disk device queues. Removes the system call at the front of the <num>th disk device 
queue. The corresponding PCB is added to the back of the ready queue.

##'q'/crt^C## : Ends the program