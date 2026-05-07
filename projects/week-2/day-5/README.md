# Week 2 — Read/Write Binary Files

This project demonstrates how to read and write binary files in C using `fopen`, `fwrite`, and `fread`. It defines a `Color` struct with two unsigned integers representing primary and secondary colors, and shows how to serialize this struct to a file and print it back.

## Files

| File                   | Role                                                                                        |
| ---------------------- | ------------------------------------------------------------------------------------------- |
| [main.c](main.c)       | Main program that defines the `Color` struct, writes it to a binary file, and reads it back |
| [color.cpt](color.cpt) | Binary file created by the program containing the serialized `Color` struct                 |

## Compilation

```bash
gcc main.c -o main
./main
```
## Output

When you run the program, it will create a file named `color.cpt` containing the binary representation of the `Color` struct. The program will also print the primary and secondary color values to the console:

```
Primary Color: 0xFFFFFFFF
Secondary Color: 0x000000FF
```