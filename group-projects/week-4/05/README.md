# Week 4 - XOR file encoder

## Files

| File             | Role                                                         |
| ---------------- | ------------------------------------------------------------ |
| [main.c](main.c) | XOR-encodes a file using a derived hash, and decodes it back |

## Input

```bash
gcc main.c -o main
./main <file>                  # encode a file
./main <hash>                  # decode and print
./main -f <hash> <output>      # decode to a file
```

## Output

The program takes a file and XORs its content with a key derived from the file, storing the result under `data/<hash>.gen`. Running it with the hash reads the file back and recovers the original data.
