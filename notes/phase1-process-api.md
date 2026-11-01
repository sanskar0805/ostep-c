System Notes
==============

## What is POSIX?
- Portable Operating System Interface
- set of standards specifying how operating system should behave at the API, shell and utility level.

## fork()
- Used to create a new process duplicating the existing calling process
- The fork() call is unusual because it is called once but returns twice—once in the parent and once in the child.
- Negative value (-1): The creation of the child process failed.
- Zero (0): Returned to the newly created child process.
- Positive value (>0): Returned to the parent process; this value is the specific PID of the child process.

- **calling_fork.c Code :**
```cpp
1.  #include <stdio.h> // IO
2.  #include <sys/types.h> // typedef for pid
3.  #include <stdlib.h> // exit()
4.  #include <unistd.h> // fork()

5.  int main(int argc, char * argv[]){
6.      printf("Hello, World (pid: %d)\n", (int) getpid());
7.      int rc = fork();
8.      if (rc < 0){ // fork failed; exit
9.          fprintf(stderr, "fork failed\n");
10.         exit(1);
11.     }
12.     else if (rc == 0){ // child (new process)
13.         printf("Hello I am child (pid: %d)\n", (int) getpid());
14.     }
15.     else { // parent goes down this path
16.         printf("hello, I am parent of %d (pid: %d)\n", rc, (int) getpid());
17.     }
18.     return 0;
19. }
```

- `rc = fork()` stores the values that means different things, that depends on which process is looking at it:

> `rc < 0` : fork failed (only the parent sees this)
> `rc = 0` : I am the child; rc is just the sentinel 0, not a PID. My PID comes from getpid().
> `rc > 0` : I am the parent, and rc is the child's PID. My own PID comes from getpid().

### How does the fork() actually works?
- `fork()` creates a new process whose entire state is the snapshot of the parent **_at the instant of the fork() call_** including :
  - The **memory** (Code, Data, Heap, Stack)
  - The **program counter** [Instruction pointer], i.e., which line is currently executing
  - Register values, open file descriptors, etc. 

-  Crucially the program counter is copied at the point of fork() call itself, 
which means both processes resume execution at the instruction immediately after `fork()` returns - not at the main() not at line 1.
- So the child does not rerun the program from the top. It does not hit fork() again, because the fork() call is already "behind" it in the instruction stream. 
The child is standing just past that line, exactly like the parent. 

**Tracing Line by Line**
```text
Line 6 : printf("Hello, World (pid:%d)\n", getpid());   ← only parent runs this
Line 7 : int rc = fork();                                ← PARENT executes this line
          ├── parent gets rc = 1800, resumes at line 8
          └── child  gets rc = 0,    resumes at line 8   ← NOT line 1!
Line 8: if (rc < 0) { ... }                              ← both evaluate, both false (or true for child)
Line 12: } else if (rc == 0) {                            ← child: true  → prints, returns
Line 15: } else {                                         ← parent: true → prints, returns
Line 18: return 0;                                        ← both reach here and exit
```
> Thus, fork() is executed exactly once per process, and only the original process ever reaches it.


## exec()
- Final and important process creation system call API is `exec()`.
- Useful when you want to execute a process other than the current process.
> calling fork() is only usefull when you want to keep running the copies of the current program, but sometimes you might need to execute other commands completing replacing the code line with some other command, `exec()` does that.
```c
int main(int argc, char *argv[]){}
```
- given a name of the executable (ex. /bin/ls) and some arguments (`*argv[]`) it loads code (and static data) from that executable and overwrites its current code segment (and current static data) with it;
- the heap and stack and other parts of the memory space are reinitialised.
- `exec()` does not create a new process, rather it transforms the current running process into some other process.
- After `exec()`, the parent is almost like it never ran.

```c
1  #include <stdio.h>
2  #include <stdlib.h>
3  #include <unistd.h>
4  #include <string.h>
5  #include <sys/wait.h>
6
7  int
8  main(intargc,char *argv[])
9  {
10      printf("helloworld (pid:%d)\n",(int)getpid());
11      int rc= fork();
12      if (rc< 0) { // forkfailed;exit
13          fprintf(stderr,"forkfailed\n");
14      exit(1);
15      } else if (rc== 0) {// child(new process)
16          printf("hello,Iam child(pid:%d)\n",(int)getpid());
17          char *myargs[3];
18          myargs[0]= strdup("wc"); // program:"wc" (wordcount)
19          myargs[1]= strdup("p3.c");// argument:file tocount
20          myargs[2]= NULL; // marksendof array
21          execvp(myargs[0],myargs); // runswordcount
22          printf("thisshouldn’tprintout");
23      } else{ // parentgoesdown thispath(main)
24          intwc = wait(NULL);
25          printf("hello,Iam parentof %d(wc:%d)(pid:%d)\n",
26              rc,wc, (int)getpid());
27      }
28      return0;
29 }
```

## headers used
```c
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
```
- `#include <sys/types.h>` :
  - Portability!!
  - On one computer, a process ID or file size might fit perfectly inside a standard 32-bit int. On another system, it might require a 64-bit long long.
  - If programmers used hardcoded types like `int` or `long` for system properties, their code would break when compiled on different machines.
  - The header manages the entire operating system interface consisting of various different functions
  - `pid_t` is a typedef and not a fixed type. On most Linux environments it is int type but technically it could be short, long or another signed integer type depending on the platform.
  - If `pid_t` is not exactly int we explicitly convert it to `(int)` and then pass to `printf(%d)` as process IDs are small sized, the truncation won't affect the value much.<br><br>

- `#include <unistd.h>` : provides the functions and constants that actually interact with operating system
  - **System calls for File Manipulation.**
  - **Process management**
  - **System Constants and Paths.**<br><br>

- `#include <sys/wait.h>` : header for wait(NULL) command, helps in making barrier to Synchronisation.

## Learning
---
### fork()
- each time our executible gets a `new PID` assigned to it, so each execution prints new `PID` value. 
- No two processes share the same `PID`.
- use wait() using the header `#include <sys/wait.h>`. This prevents the parent process from terminating before the child process is terminated. 
- `printf()` is used for normal program output. (stdout)
- `fprintf(stderr, ...)` used for error messages, so they remain visible even if the `stdout`is redirected.
- under `<stdlib.h>` header, `exit()` function is there which helps to terminate the program and clears the IO buffer.<br>It takes `int status` as parameter.<br>**`0` :** Success<br>**`1` :** Failure<br> It also has `EXIT_SUCCESS` and `EXIT_FAILURE` constants inside the `<stdlib.h>` library.

- `exit()` (from `<stdlib.h>`) is a library function. It flushes the `stdio` buffers (writes whatever is in memory to the file/screen) and then makes the `exit_group` system call.
- `_exit()` (from `<unistd.h>`) is the raw system call. It terminates immediately and **does not** flush buffers.

- if `fork()` is successfull:
There are two cases:
fork returns 0; This means that fork now has value 0. This just tell that we are in the child process.
To get the process id of the child, just use `getpid()`.
And if the fork returns a value > 0, then, we are inside the parent process and fork has the value of `Child's PID`. 

- **Race condition**
  - Without synchronization, the output order is indeterministic.
  - processes **do not** share registers or memory. When a context switch happens, the OS saves the registers of Process A and restores the registers of Process B.
  - After `fork()` both the parent and the child are sitting in the ready queue.<br>The Scheduler picks one and you cannot predict which it will be.
  - **Solution**: `wait(NULL)` act as a Synchronization Barrier. 

- **Why There is a need to prevent parent from terminating before the child finishes ?**
  - _Pipelining_ : There are cases where the output of the child is being utilised by the parent. So, we need to hault the parent until the child finishes its execution.
  - _zombie process_ : When the process finishes the program execution, some details of the process gets stored in the kernel (its pid, resource usage, exit status etc.). This starts accumulating waste in kernel. 
  - _Correct sequencing_ : If the parent terminates before the child, then the child gets reparented to `init(PID 1)`. This would further can break the logic, if the child needs its actual parent pid.
- **How `wait(NULL)` guarantees the order ?**
  - *Scenario A (Parent runs first)*
    - The parent reaches `wait(NULL)` and then immediately gets blocked.
    - The scheduler then runs the child.
    - The child prints its line, exits and its termination then **wakes up** the parent. Only then does the parent executes its printf().
    This waking up, is what is called **Context Switching.**

  - *Scenario B (Child runs first)*
    - The child runs, prints its line and ends. 
    - When the parent is eventually scheduled, it calls wait(NULL). 
    - Since, the child has already terminated, the wait(NULL) does not block; it returns immediately. The parent then proceeds to its printf.

- `wait()` performs two curcial things:
  - **_Synchronization_**: it blocks the parent, enforcing the desired execution order.
  - **_Reaping (Preventing Zombies)_** : when child exits, the kernel keeps a small record of it (its PID, exit status, resource usage) in the process table.
  This defunct process is called **zombie.**
  wait() (or waitpid()) is the mechanism that allows the parent to collect the information and tell the kernel to **release the zombie's record**, freeing up system resources.<br><br>

---
### exec()
- **What if you want to execute a program that is different from the calling program?**

  - If you want to keep making copies of the same current running program then you call `fork()` system call.
  - But if you want to execute another program, then you use `exec()` system call.
  - `exec()` comes under `#include <unistd.h>` header.
  - Linux does not provide a single `exec()` system call, it provides a family of it: 
  |function|usage|
  |:------:|:---:|
  |execl()||
  |execlp()||
  |execle()||
  |execv()||
  |execvp()||
  |execvpe()||

  - All of the above commands, will eventually call the kernel's `execve()`.
```cpp
#include <unistd.h>

int execve(const char *pathname,
          const char* argv[],
          const char* envp[]);
```
  - `char* argv[]` is a list containing the arguments passed together inside the shell and kept in a note of arg0, arg1, arg2..., argn  consisting of one or more pointers null-terminated strings that represent the argument.
  - **Why there is a need of exec() ?**:
    - Suppose you want to execute `ls -a` (listing all the files and folders inside a directory including hidden) in a shell.
    Now shell is itself a process running on OS.
    `ls -a` is trying to call `ls` process command in order to execute it inside the current process. 
    Linux needs something to load an entirely different executable. 
    That mechanism is called **`exec()`**.

> Extra: 
> `symlinks` are the **Linux Symbolic Links** and are supposed to be a directory shortcut in linux. 
> Windows doesn't understand these.
> Inside the linux command line, the operating system knows exactly what a symbolic link is.
> For example:
>```bash
>/bin
>``` 
> Think of `/bin` as a signpost that says: "Hey, I don't actually hold any files here. Please automatically redirect all the traffic to `/usr/bin`.

> To prove if a link is a `symlink` use:
> ```bash
> ls -ld /bin
> ```
> **Output**: lrwxrwxrwx ... /bin -> usr/bin

- **What happens when a program consists of an `exec()` command ?**
  - The current process will run our program that consist of the shell code
  - after a successfull `exec()` the original program code is replaced by the `exec()` shell code program. 
  - The the program that consist of our shell code, now doesn't exist.
  - Note: The process that is running remains the same, thus PID remains the same. (You can verify using the `fork()` system call.)

  - But How we can prevent the shell from terminating automatically when `exec()` is run ?:
    - This is where things get interesting. **Most linux programs use :** 
    ```bash
    fork() and exec()
    ```

    - fork() creates a copy of the parent process
    ```text
    Shell 
     PID=100
      | 
      +---- Child 
      PID=101
    ```

    - Now both the process continue execution from the next line.
    - `fork()` first
    - Then child execs ls
    - final state
    ```text
    Shell (parent)
           | 
           +---- ls (child)
    ```
    - when ls exits, Shell survives (parent process running our shell).<br><br>

- **How shell executes command ?**
  Let us use `ls -l` as an example:
  - **_Step 1_**: Shell reads input
  ```bash
  ls -l
  ```
  - **_Step 2_**: Parse
  Shell Identifies:
  ```bash
  Program: ls

  Arguments:
    argv[0] = "ls"
    argv[1] = "-l"
  ```

  - **_Step 3_**: `fork()`
  ```c
  pid = fork();
  ```

  kernel creates child process.
  - **_Step 4_**: Child executes `exec()`
  Child: 
  ```c
  execvp("ls", argv);
  ```
  Kernel loads
  ```text
  /bin/ls
  ```
  over child memory
  Now child no longer bash.
  Child became:
  ```
  ls
  ```
  - **_Step 5_**: parent waits
  ```c
  waitpid(pid, ...);
  ```

  - **_Step 6_** : child exits
  After listing files:
  ```c
  exit(0);
  ```
  **Kernel destroys the child.**
  - **_Step 7_**: Shell continues
  Prompt appears:
  ```bash
  $
  ```
  again.

  > This fork() → exec → wait cycle is the heart of Unix.

- arguments in `execvp()` must end with `NULL`, so at the end of arguments, you must put `NULL`
- `execvp(program, arguments);` Here, the first argument takes the executable file and the second argument passed in execvp() is the actual code that is run in that executable.

- `strdup()` is used for duplicating a string.
  - In our case, string literals already exist in the memory, so we are not needed to copy the things.
  - that's why we don't require `strdup()` in our case right now.
  - we used `strdup()` for pedagogical reasons (according to OSTEP)
  - It emphasizes that argv is an array of **character pointers (char *)** pointing to strings.
  ```c
  myargs[0] = strdup("wc");
  ```
  - creates a real writable string in heap memory and returns a char *.