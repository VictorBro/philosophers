# Philosophers
Dining philosophers problem

## Tests
data race detector:
```
$ valgrind --tool=drd ./philo 105 800 200 200
```
stricter race detector, reports potential ABBA lock order issues
```
$ valgrind --tool=helgrind ./philo 105 800 200 200
```
ThreadSanitizer:
if you try to compile with -fsanitize=thread and get this error:
```
$ gcc -Werror -Wall -Wextra -fsanitize=thread philo_test.c -o philo

$ ./philo 5 800 200 200
FATAL: ThreadSanitizer: unexpected memory mapping 0xc82304ee0000-0xc82304ee3000

$ sudo sysctl vm.mmap_rnd_bits                                     
vm.mmap_rnd_bits = 33
```
That shows your Linux is maxing out the randomization (33 bits), which is incompatible with TSan (max supported is 30 bits on x86 Linux).
This is related to ASLR, where the TSan supports up to 30 bits of ASLR.
So we need to put any value that works for you that is <= 30, in my case 28 worked better than 30.
```
$ sudo sysctl vm.mmap_rnd_bits=28
```
Memory leaks:
```
$ valgrind --leak-check=full --show-leak-kinds=all ./philo 200 610 200 200
```
