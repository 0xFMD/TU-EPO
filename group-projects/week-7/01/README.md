# Week 7-01 - Virtual Machine

## Files

| File           | Role                                 |
| -------------- | ------------------------------------ |
| [vm3.c](vm3.c) | VM with memory, registers,functions, and flags |
| [vm2.c](vm2.c) | VM with memory, registers, and flags |
| [vm.c](vm.c)   | Compact VM                           |

## Build

```bash
gcc -g -Wall -Wextra vm3.c  -o vm3
gcc -g -Wall -Wextra vm2.c  -o vm2
gcc -g -Wall -Wextra vm.c -o vm
```

## Output

Each instruction prints its result after execution:

```
output -> 16
output -> 4
output -> 95
```
