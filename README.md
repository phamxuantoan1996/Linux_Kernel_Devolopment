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
- The kernel stores the list of processes in a circular doubly linked list called the task list.
- Each element in the task list is a process descriptor of the type struct task_struct, which
is defined in <linux/sched.h>.The process descriptor contains all the information about
a specific process.
# Allocating the Process Descriptor
# Storing the Process Descriptor
- The system identifies processes by a unique process identification value or PID.The PID is a
numerical value represented by the opaque type4 pid_t, which is typically an int.

- The kernel stores this value as pid inside each process descriptor.
# Process state
- The state field of the process descriptor describes the current condition of the process.
- Each process on the system is in exactly one of five different states.This value is represented by one of five flags:
+ TASK_RUNNING : The process is runnable; it is either currently running or on a run-queue waiting to run. This is the only possible state for a process executing in user-space; it can also apply to a process in kernel-space that is actively running.

+ TASK_INTERRUPTIBLE : The process is sleeping, (that is, it is blocked), waiting for some condition to exist. When this condition exists, the kernel sets process's state to TASK_RUNNING. The process also awakes prematurely and becomes runnale if it receives a signal.

+ TASK_UNINTERRUPTIBLE : This state is identical TASK_INTERRUPTIBLE except that it does not wake up and become runnable wait if it reiceives a signal. This is used in situations where the process must wait without interruption or when the event is expected to occur quite quickly. Because the task does not respond to signal in this state, TASK_UNITERRUPTIBLE is less often used than TASK_INTERRUPTIBLE.

+ __TASK_STOPPED : Process execution has stopped; the task is not running nor is it eligible to run.This occurs if the task receives the SIGSTOP, SIGTSTP, SIGTTIN, or SIGTTOU signal or if it receives any signal while it is being debugged.

# 3) Manipulating the Current Process State.
Kernel code often needs to change a process’s state.The preferred mechanism is using

set_task_state(task, state);

set_current_state(state); <=> set_task_state(current,state);

# Process Context
- Normal program execution occurs in user-space.

- When a program executes a system call or triggers an exception, it enters kernel-space. This time, the kernel is said to be "executing on behalf of the process" and is in process context.
 
- Upon exiting the kernel, the process resumes execution in user-space, unless a higher-priority process has become runnable in the interim, in which case the scheduler is invoked to select the higher priority process.

- System calls and exception handlers are well-defined interfaces into the kernel.

- A process can begin executing in kernel-space only through one of these interfaces. All access to the kernel is through these interfaces.

* Note : Other than process context there is interrupt context. In interrupt context, the system is not running on behalf of a process but is executing an interrupt handler. No process is tie to interrupt handlers.

# The Process Family Tree
- All processes are descendants of the init process, whose PID is one.

- The kernel starts init in the last step of the boot process.

- Init process reads initscripts and executes nore program, eventually completing the boot process.

- Every process has one parent process. Likewise, every process has zero or more childrent.

- The relationship between processes is stored in the process descriptor. Each task struct has a pointer to the parent's task struct, named "parent", and a list of childrent, named "childrent"










