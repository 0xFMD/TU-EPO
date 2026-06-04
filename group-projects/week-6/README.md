# Simple VM (vm.c)

Small educational virtual machine implemented in C. It demonstrates a fetch-decode-execute cycle with a small instruction set.

## Build

```bash
gcc -Wall vm.c -o vm
```

## Run

```bash
./vm
```

Instruction set

- OPCODE_ADD, OPCODE_SUB
- OPCODE_AND, OPCODE_OR, OPCODE_NOT, OPCODE_XOR
- OPCODE_SHL, OPCODE_SHR

## Output

```bash
init: reg(0)= 1  reg(1)= 1

ADD(+):  2
SUB(-):  0
AND(&):  1
OR(|):   1
NOT(~):  4294967294
XOR(^):  0
SHL(<<): 2
SHR(>>): 0

```
