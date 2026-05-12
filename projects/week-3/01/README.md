# Week 3 — Partitioning a binary file

## Files

| File                 | Role                                              |
| -------------------- | ------------------------------------------------- |
| [main.c](main.c)     | Main program that defines the partitioning logic  |
| [edid.bin](edid.bin) | Binary file containing the data to be partitioned |

## Input

```bash
gcc main.c -o main
./main ./edid.bin 4
```

## Output

When you run the program, it will read the `edid.bin` file, partition the data into files:

Partitioned files:

- edid.bin.1
- edid.bin.2
- edid.bin.3
- edid.bin.4
