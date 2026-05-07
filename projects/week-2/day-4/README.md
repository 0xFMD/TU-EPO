# Week 2 — C Macros

Demonstrates conditional compilation using `#define`, `#ifdef`, and `#ifndef`.

The project uses `#include` to compose nested loop logic from separate `.c` files, with macro guards controlling which branches are active.

## Files

| File | Role |
|------|------|
| [main.c](main.c) | Entry point — defines macros and includes `main.a.c` |
| [main.a.c](main.a.c) | Outer loop logic with `#ifdef OUTER_LOOP` guard |
| [main.b.c](main.b.c) | Inner loop body with `#ifdef`/`#ifndef OUTER_LOOP` guards |

## Compilation

```bash
gcc main.c -o main
./main
```

## Macro Combinations

Comment out either `#define` in `main.c` to change behavior:

| `OUTER_LOOP` | `INNER_LOOP` | Output |
|---|---|---|
| defined | defined | outer + inner loops, prints `outerloop: y` and `inner - result = i*y` |
| defined | not defined | outer loop only, prints `outerloop: y` |
| not defined | defined | inner loop only, prints `innerloop: i` |
| not defined | not defined | no output |
