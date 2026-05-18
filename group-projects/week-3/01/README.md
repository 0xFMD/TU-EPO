# Week 3 — Partitioning/merging a binary file

## Files

| File                                                 | Role                                                  |
| ---------------------------------------------------- | ----------------------------------------------------- |
| [main.c](main.c)                                     | Main program that defines the partitioning logic      |
| [edid.bin](edid.bin)                                 | Binary file containing the data to be partitioned     |
| [merged.edid.bin](merged.edid.bin)                   | Output file that will contain the merged data         |
| [rotation_merged.edid.bin](rotation_merged.edid.bin) | Output file that will contain the rotated merged data |

## Input

```bash
gcc main.c -o main
./main -p ./edid.bin {num of partitions}
```

```bash
gcc main.c -o main
./main -m ./edid.bin.pmd
```

```bash
gcc main.c -o main
./main -m ./edid.bin.pmd -r {num of rotations}
```

## Output

When you run the program, it will read the `edid.bin` file, partition the data into files:

Partitioned files:

- edid.bin.1
- edid.bin.2
- edid.bin.3
- edid.bin.4


