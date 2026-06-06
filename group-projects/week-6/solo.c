#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MEMORY_SIZE      4 * 1024 // 4KB
#define NUM_G_REGISTER   4
#define NUM_INSTRUCTIONS 3

typedef struct instruction_t instruction_t;
typedef struct cpu_t         cpu_t;
typedef struct memory_t      memory_t;
typedef struct process_t     process_t;
typedef struct vm_t          vm_t;

typedef enum Instruction_set {
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_NOT,
    OP_SHL,
    OP_SHR,
    OP_ADD,
    OP_SUB,
    OP_MUL,

} Instruction_set;

struct instruction_t {
    uint16_t op_code : 4;
    uint16_t in_reg_1 : 2;
    uint16_t in_reg_2 : 2;
    uint16_t out_reg : 2;
};

struct cpu_t {
    uint32_t ip;
    uint32_t ir;
    uint32_t regs[NUM_G_REGISTER];
};

struct memory_t {
    uint8_t* memory;
    int      capacity;
    size_t   allocated;
};

struct process_t {
    uint32_t       data_size;
    uint32_t       code_size;
    uint16_t       offset;

    cpu_t          cpu_state;

    uint32_t       instructions_count;
    instruction_t* instructions;
};

struct vm_t {
    cpu_t    cpu;
    memory_t Memory;
};

static inline uint32_t encode_instruction(instruction_t instruction) {
    return (instruction.op_code << 6) | (instruction.in_reg_1 << 4) | (instruction.in_reg_2 << 2) | (instruction.out_reg);
}

static inline bool decode_instruction(instruction_t* instruction, uint32_t ir) {
    if (!instruction)
        return false;

    instruction->op_code  = (ir >> 6) & 0xF;
    instruction->in_reg_1 = (ir >> 4) & 0x3;
    instruction->in_reg_2 = (ir >> 2) & 0x3;
    instruction->out_reg  = ir & 0x3;

    return true;
}

bool op_add(vm_t* vm, instruction_t instruction) {

    vm->cpu.regs[instruction.out_reg] = vm->cpu.regs[instruction.in_reg_1] + vm->cpu.regs[instruction.in_reg_2];

    return true;
}

bool op_sub(vm_t* vm, instruction_t instruction) {

    vm->cpu.regs[instruction.out_reg] = vm->cpu.regs[instruction.in_reg_1] - vm->cpu.regs[instruction.in_reg_2];

    return true;
}

bool op_mul(vm_t* vm, instruction_t instruction) {

    vm->cpu.regs[instruction.out_reg] = vm->cpu.regs[instruction.in_reg_1] * vm->cpu.regs[instruction.in_reg_2];

    return true;
}

void print_vm_info(vm_t* vm) {
    printf("\n\nCPU: ip: %x, ir: %b \n", vm->cpu.ip, vm->cpu.ir);
    for (int i = 0; i < NUM_G_REGISTER; i++) {
        printf("Reg-%d: %d\n", i, vm->cpu.regs[i]);
    }
    printf("\n\nMemory: Allocated: %zu bytes, Capacity: %d bytes", vm->Memory.allocated, vm->Memory.capacity);
}

void init_vm(vm_t* vm) {
    cpu_t cpu = {.ip = 0, .ir = 0, .regs = {0}};
    // test
    cpu.regs[0] = 1;
    cpu.regs[1] = 2;

    memory_t* memory  = malloc(sizeof(memory_t));
    memory->allocated = 0;
    memory->capacity  = MEMORY_SIZE;
    memory->memory    = malloc(memory->capacity);
    vm->cpu           = cpu;
    vm->Memory        = *memory;
    free(memory);
}

void init_process(vm_t* vm, process_t* process, instruction_t* program, size_t program_size) {
    process->data_size          = 0;
    process->code_size          = program_size;
    process->instructions_count = program_size / sizeof(instruction_t);
    process->offset             = process->data_size;
    process->instructions       = malloc(program_size);
    for (uint32_t i = 0; i < process->instructions_count; i++) {
        process->instructions[i] = program[i];
        vm->Memory.allocated += sizeof(instruction_t);
    }

    printf("\n\nProcess: data_size: %d, code_size: %d, instruction_count: %d, "
           "offset: %d ",
           process->data_size, process->code_size, process->instructions_count, process->offset);
}

void fetch(vm_t* vm, process_t* process) {
    vm->cpu.ir = encode_instruction(process->instructions[vm->cpu.ip]);
    vm->cpu.ip++;
}

void execute(vm_t* vm, instruction_t instruction, bool (*op)(vm_t* vm, instruction_t instruction)) {
    op(vm, instruction);
}

void decode(vm_t* vm) {
    instruction_t instruction;
    decode_instruction(&instruction, vm->cpu.ir);

    switch (instruction.op_code) {
        case OP_ADD:
            printf("\n\n Add - inp1: %d, inp2: %d", vm->cpu.regs[instruction.in_reg_1], vm->cpu.regs[instruction.in_reg_2]);
            execute(vm, instruction, op_add);
            printf(", output %d", vm->cpu.regs[instruction.out_reg]);
            break;
        case OP_SUB:
            printf("\n\n Sub - inp1: %d, inp2: %d", vm->cpu.regs[instruction.in_reg_1], vm->cpu.regs[instruction.in_reg_2]);
            execute(vm, instruction, op_sub);

            printf(", output %d", vm->cpu.regs[instruction.out_reg]);
            break;
        case OP_MUL:
            printf("\n\n Mul - inp1: %d, inp2: %d", vm->cpu.regs[instruction.in_reg_1], vm->cpu.regs[instruction.in_reg_2]);
            execute(vm, instruction, op_mul);
            printf(", output %d", vm->cpu.regs[instruction.out_reg]);
            break;
    }
}

void run_vm(vm_t* vm, process_t* process) {
    if (!vm || !process)
        return;
    while (vm->cpu.ip < process->instructions_count) {
        fetch(vm, process);
        decode(vm);
    }
}

int main() {

    vm_t*      vm = malloc(sizeof(vm_t));

    process_t* process = malloc(sizeof(process_t));
    init_vm(vm);
    instruction_t program[NUM_INSTRUCTIONS] = {
        {.op_code = OP_ADD, .in_reg_1 = 0, .in_reg_2 = 1, .out_reg = 2},
        {.op_code = OP_MUL, .in_reg_1 = 1, .in_reg_2 = 2, .out_reg = 3},
        {.op_code = OP_SUB, .in_reg_1 = 3, .in_reg_2 = 1, .out_reg = 3},
    };

    init_process(vm, process, program, sizeof(program));

    run_vm(vm, process);

    print_vm_info(vm);

    free(vm->Memory.memory);
    free(vm);
    free(process->instructions);
    free(process);

    return 0;
}