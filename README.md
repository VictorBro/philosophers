# 🍝 Philosophers

## I never thought philosophy would be so deadly

This project explores concurrent programming concepts through the classic dining philosophers problem. It's an implementation of a synchronization problem proposed by Edsger Dijkstra in 1965 to illustrate challenges in ensuring several processes can access shared resources without causing conflicts.

## 📚 Problem Overview

* One or more philosophers sit at a round table with a large bowl of spaghetti in the middle
* The philosophers alternate between three states: eating, thinking, and sleeping
* There are as many forks as philosophers on the table
* A philosopher needs both their left and right forks to eat
* After eating, philosophers put down their forks and start sleeping
* Once awake, they start thinking again
* The simulation stops when a philosopher dies of starvation
* Every philosopher needs to eat and should never starve
* Philosophers don't communicate with each other
* Philosophers don't know if another philosopher is about to die

## 🎯 Focus on Fairness

This implementation emphasizes **fairness** to prevent philosopher deaths. The solution prioritizes equal resource distribution and optimized synchronization to minimize starvation risk.

## 🏗️ Project Structure

This project has two distinct implementations of the dining philosophers problem:

### 1. `philo/` - Multithreading with Mutexes

* Each philosopher is implemented as a separate **thread**
* Forks are represented by **mutexes**
* There is one fork between each pair of philosophers
* Each fork's state is protected by a mutex to prevent duplication

### 2. `philo_bonus/` - Multiprocessing with Semaphores

* Each philosopher is implemented as a separate **process**
* Forks are put in the middle of the table with no states in memory
* The number of available forks is represented by a **semaphore**
* The main process coordinates but doesn't act as a philosopher

## 🚀 Usage

Both programs take the same arguments:

```
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```

* **number_of_philosophers**: The number of philosophers and forks
* **time_to_die** (ms): Maximum time a philosopher can go without eating before dying
* **time_to_eat** (ms): Time it takes for a philosopher to eat (holding two forks)
* **time_to_sleep** (ms): Time a philosopher spends sleeping
* **number_of_times_each_philosopher_must_eat** (optional): If specified, simulation stops when all philosophers have eaten this many times

### Examples:

```bash
# Run the multithreaded version with 5 philosophers
./philo/philo 5 800 200 200

# Run the multiprocessing version with 4 philosophers, each must eat 10 times
./philo_bonus/philo_bonus 4 410 200 200 10
```

## 🧪 Testing and Debugging

### Data Race Detection

```bash
# Standard race detector
valgrind --tool=drd ./philo 105 800 200 200

# Stricter race detector, reports potential ABBA lock order issues
valgrind --tool=helgrind ./philo 105 800 200 200
```

### ThreadSanitizer (TSan)

If you encounter this error with ThreadSanitizer:

```bash
$ gcc -Werror -Wall -Wextra -fsanitize=thread philo_test.c -o philo
$ ./philo 5 800 200 200
FATAL: ThreadSanitizer: unexpected memory mapping 0xc82304ee0000-0xc82304ee3000
```

This is because your Linux is using high ASLR randomization (check with `sudo sysctl vm.mmap_rnd_bits`), which is incompatible with TSan (max supported is 30 bits on x86 Linux).

Fix by reducing the randomization:

```bash
$ sudo sysctl vm.mmap_rnd_bits=28
```

### Memory Leak Detection

```bash
valgrind --leak-check=full --show-leak-kinds=all ./philo 200 610 200 200
```

## ⚙️ Building

Each implementation has its own Makefile with standard rules:

```bash
# Build the multithreaded version
cd philo && make

# Build the multiprocessing version
cd philo_bonus && make

# Debug build options are also available
cd philo && make re DEBUG=1
```

## 📋 Requirements

* Written in C
* Follows the Norm coding style
* No memory leaks
* No crashes or unexpected behavior
* Proper synchronization to prevent data races
* Efficient resource management
* Accurate death detection (within 10ms)

## 🧠 Learning Outcomes

* Thread creation and management
* Process creation and management
* Mutex synchronization
* Semaphore usage
* Deadlock prevention
* Race condition prevention
* Efficient resource allocation

---

<p align="center">
  <i>This project is part of the 42 School curriculum.</i>
</p>
