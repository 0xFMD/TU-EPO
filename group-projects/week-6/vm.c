#include <inttypes.h>
#include <stdio.h>

#define MEMORY_SIZE 1024
#define NUM_OF_INSTRUCTIONS 8

typedef struct Instruction {
  uint16_t op_code : 3;
  uint16_t in_reg_0 : 2;
  uint16_t in_reg_1 : 2;
  uint16_t out_reg : 2;
} Instruction;

typedef enum Instruction_set {
  OPCODE_ADD,
  OPCODE_SUB,
  OPCODE_AND,
  OPCODE_OR,
  OPCODE_NOT,
  OPCODE_XOR,
  OPCODE_SHL,
  OPCODE_SHR
} Instruction_set;

typedef struct Memory {
  uint8_t memory[MEMORY_SIZE];
  int allocated;
} Memory;

typedef struct CPU {
  uint16_t ip;
  Instruction ir;
  uint32_t reg[4];
} CPU;

typedef struct Process {
  int16_t offset;
  int16_t data_size;
  int16_t code_size;
  Instruction instructions[NUM_OF_INSTRUCTIONS];
} Process;

typedef struct VM {
  CPU cpu;
  Memory memory;
} VM;

void fetch(VM *vm, Process *process) {
  vm->cpu.ir = process->instructions[vm->cpu.ip];
  vm->cpu.ip++;
}

void execute(VM *vm, Process *process) {

  switch (vm->cpu.ir.op_code) {
  case OPCODE_ADD:
    vm->cpu.reg[vm->cpu.ir.out_reg] =
        vm->cpu.reg[vm->cpu.ir.in_reg_0] + vm->cpu.reg[vm->cpu.ir.in_reg_1];
    printf("ADD(+):  %u\n", vm->cpu.reg[vm->cpu.ir.out_reg]);
    break;
  case OPCODE_SUB:
    vm->cpu.reg[vm->cpu.ir.out_reg] =
        vm->cpu.reg[vm->cpu.ir.in_reg_0] - vm->cpu.reg[vm->cpu.ir.in_reg_1];
    printf("SUB(-):  %u\n", vm->cpu.reg[vm->cpu.ir.out_reg]);
    break;
  case OPCODE_AND:
    vm->cpu.reg[vm->cpu.ir.out_reg] =
        vm->cpu.reg[vm->cpu.ir.in_reg_0] & vm->cpu.reg[vm->cpu.ir.in_reg_1];
    printf("AND(&):  %u\n", vm->cpu.reg[vm->cpu.ir.out_reg]);
    break;
  case OPCODE_OR:
    vm->cpu.reg[vm->cpu.ir.out_reg] =
        vm->cpu.reg[vm->cpu.ir.in_reg_0] | vm->cpu.reg[vm->cpu.ir.in_reg_1];
    printf("OR(|):   %u\n", vm->cpu.reg[vm->cpu.ir.out_reg]);
    break;
  case OPCODE_NOT:
    vm->cpu.reg[vm->cpu.ir.out_reg] = ~vm->cpu.reg[vm->cpu.ir.in_reg_0];
    printf("NOT(~):  %u\n", vm->cpu.reg[vm->cpu.ir.out_reg]);
    break;
  case OPCODE_XOR:
    vm->cpu.reg[vm->cpu.ir.out_reg] =
        vm->cpu.reg[vm->cpu.ir.in_reg_0] ^ vm->cpu.reg[vm->cpu.ir.in_reg_1];
    printf("XOR(^):  %u\n", vm->cpu.reg[vm->cpu.ir.out_reg]);
    break;
  case OPCODE_SHL:
    vm->cpu.reg[vm->cpu.ir.out_reg] = vm->cpu.reg[vm->cpu.ir.in_reg_0]
                                      << vm->cpu.reg[vm->cpu.ir.in_reg_1];
    printf("SHL(<<): %u\n", vm->cpu.reg[vm->cpu.ir.out_reg]);
    break;
  case OPCODE_SHR:
    vm->cpu.reg[vm->cpu.ir.out_reg] =
        vm->cpu.reg[vm->cpu.ir.in_reg_0] >> vm->cpu.reg[vm->cpu.ir.in_reg_1];
    printf("SHR(>>): %u\n", vm->cpu.reg[vm->cpu.ir.out_reg]);
    break;
  }
}

void run(VM *vm, Process *process) {
  vm->memory.allocated = process->data_size + process->code_size;
  vm->cpu.ip = 0;
  while (vm->cpu.ip < NUM_OF_INSTRUCTIONS) {
    fetch(vm, process);
    execute(vm, process);
  }
}

int main(void) {
  VM vm;
  vm.cpu.reg[0] = 1;
  vm.cpu.reg[1] = 1;

  Instruction program[NUM_OF_INSTRUCTIONS] = {
      {.op_code = OPCODE_ADD, .in_reg_0 = 0, .in_reg_1 = 1, .out_reg = 2},
      {.op_code = OPCODE_SUB, .in_reg_0 = 0, .in_reg_1 = 1, .out_reg = 2},
      {.op_code = OPCODE_AND, .in_reg_0 = 0, .in_reg_1 = 1, .out_reg = 2},
      {.op_code = OPCODE_OR, .in_reg_0 = 0, .in_reg_1 = 1, .out_reg = 2},
      {.op_code = OPCODE_NOT, .in_reg_0 = 1, .out_reg = 2},
      {.op_code = OPCODE_XOR, .in_reg_0 = 0, .in_reg_1 = 1, .out_reg = 2},
      {.op_code = OPCODE_SHL, .in_reg_0 = 0, .in_reg_1 = 1, .out_reg = 2},
      {.op_code = OPCODE_SHR, .in_reg_0 = 0, .in_reg_1 = 1, .out_reg = 2},
  };

  Process process;
  process.offset = 0;
  process.data_size = 0;
  process.code_size = NUM_OF_INSTRUCTIONS * sizeof(Instruction);

  for (int i = 0; i < NUM_OF_INSTRUCTIONS; i++)
    process.instructions[i] = program[i];

  run(&vm, &process);

  return 0;
}