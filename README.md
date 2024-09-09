## Table of Contents

- [POSIX-Semaphors](#posix-semaphors)

- [Implementation](#implementation)

- [Requirements](#requirements)

- [Run Command](#run-command)

# POSIX-Semaphors
Created and manipulated threads, as well as POSIX semaphores to synchronize different threads. This project simulates a study room where students enter, study for a random period, and then leave. Ituses threads to represent students, semaphores to manage the number of students that can be in the study room simultaneously, and mutexes to control access to shared resources.

## Implememtation

For the implementation of the exercise, I used 2 semaphores and a mutex for entry into the critical section. 

The first semaphore, `sem_t empty`, refers to the available spots in the reading room. Initially, I set it to 8 because no one is inside. The second semaphore, `sem_t full`, refers to the filled spots, which are initialized to 0 since the reading room is empty.

For the structure of the reading room and the corridor, I use a list and a queue. In the `main` function, I initialize all the variables, and through `pthread_create`, I call the `enter_study_room` function for entering the reading room. The `empty` semaphore blocks entry into the reading room if it is already full. If a thread is blocked, it waits until a `sem_post` is called. However, this means that every time a spot becomes available, the blocked thread will enter. We want the entire room to be emptied before someone else can enter.

To solve this, I use a counter `study_room_length` that tracks how many people are inside the reading room. If there are 8 people, I set `flag=1`. In case `flag=1`, what I do is set the semaphore to 0. When `empty` has a value of 0, all incoming threads are blocked, even if someone leaves, as long as `flag` is 1. To unblock those waiting, the value of `empty` needs to increase. This happens when the reading room is completely emptied, i.e., when the counter `study_room_length==0`.

In this case, I set `flag=2`, and I divide it into two cases. If the number of students waiting outside is less than or equal to 8 (e.g., 5), then all can fit, and `empty` is set to 5. However, if more than 8 are waiting, only the first 8 are allowed in based on priority order, and the rest are blocked again by the semaphore until the room empties once more.

Lastly, I store everyone waiting in the `waiting_room` queue because when threads are unblocked, they don’t get unblocked in the order they arrived. That’s why I insert them into the reading room through the queue, to maintain the FIFO (First In, First Out) order.


## Run Command

To execute this program run this command:

```bash
make
```

## Requirements

To compile and run this project, you will need:

- **GCC (GNU Compiler Collection)**: Ensure you have GCC installed for compiling the C code.
- **POSIX Threads (pthreads)**: The program uses the pthread library for threading.
- **POSIX Semaphores (semaphore.h)**: The program uses semaphores for synchronization.
- **Standard C Libraries**:
  - `stdio.h` for standard input/output functions.
  - `stdlib.h` for memory allocation and process control.
  - `semaphore.h` for semaphore operations.
  - `time.h` for time-related functions.
  - `pthread.h` for threading.
  - `unistd.h` for accessing the POSIX API.