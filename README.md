# Linux_Kernel_Devolopment

# Chapter1: Process Management

# 1) The Process

# Process include:
+ the executing program code.
+ set of resources such of as open files and pending signals.
+ internal kernel data.
+ processor state.
+ a memory address space with one or more memory mappings.
+ one or more thread execution.
+ a data section containing global variables.


# Thread include:
+ a unique program counter.
+ process stack.
+ and set of processor registers.

# Note: The kernel schedules individual threads, not processes.

On modern OS, processes provide two virtualizations:
+ Virtualized processor : The virtual processor gives the process the illusion that it alone monopolizes the system, despite possibly sharing the processor among hundreds of other processes.

+ Virtual memory : Virtual memory lets the process allocate and manage memory as if it alone owned all the memory in the system.

A process begin its life when it is created. To create a process, Linux has 2 system calls:

+ fork() : this system call create a new process by duplicating a existing process. The process that calls fork() is the parent, whereas the the new process is the child. The parent resume execution and child starts execution at the same place: where the call to fork() returns. The fork() system call returns from the kernel twice : once in the parent process and again in the new born child.

+ exec() : this system call creates a new space address and loads a new program into it. 

Finally, a program exits via the exit() system call. This function terminates the process and frees all its resources. A parent process can inquire about status of a terminated child via wait() system call, which enables a process to wait for the temination of a specific process. When a process exits, it is placed into a special zombie state that represents terminated processes until the parent call wait() or waitpid().

# 2) Process Descriptor and Task Structure





