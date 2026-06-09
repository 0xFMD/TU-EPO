# Week 7-01 - Virtual Machine

## Files

| File           | Role                                 |
| -------------- | ------------------------------------ |
| [vm8.c](vm8.c) | VM with memory, registers, and flags |
| [vm.c](vm.c)   | Compact VM                           |

## Build

```bash
gcc -g -Wall -Wextra vm8.c  -o vm8
gcc -g -Wall -Wextra vm.c -o vm
```

## Output

Each instruction prints its result after execution:

```
output -> 16
output -> 4
output -> 95
```
