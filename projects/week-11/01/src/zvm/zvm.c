#include "../../include/zvm/zvm.h"

bool zvm_init(zvm_vm_t *vm) {
  vm->cpu.IP = 0;
  vm->cpu.IR = malloc(sizeof(zvm_instruction_t));
  zvm_exception_reset(vm);
  vm->cpu.R[0] = 0;
  vm->cpu.R[1] = 0;
  vm->cpu.R[2] = 0;
  vm->cpu.R[3] = 0;

  vm->cpu.FLAGS = 0;
  vm->cpu.SP = -1;

  return zvm_init_io(vm);
}

void zvm_release(zvm_vm_t *vm) {
  vm->cpu.IP = 0;
  free(vm->cpu.IR);

  vm->cpu.R[0] = 0;
  vm->cpu.R[1] = 0;
  vm->cpu.R[2] = 0;
  vm->cpu.R[3] = 0;

  vm->cpu.FLAGS = 0;
  vm->cpu.SP = -1;

  blb_blob_delete(vm->program.memory);
  blb_range_delete(vm->program.code_segment);
  blb_range_delete(vm->program.stack_segment);
  blb_range_delete(vm->program.data_segment);
}

static bool zvm_init_io(zvm_vm_t *vm) {
  vm->io_devices[0] = &keyboard;
  vm->io_devices[1] = &screen;
  vm->io_devices[2] = NULL;
  vm->io_devices[3] = NULL;
  return true;
}

void zvm_init_program(zvm_vm_t *vm) {
  vm->program.code_size = 0;

  vm->program.memory =
      blb_blob_create(ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE +
                          ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE +
                          ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE,
                      1);

  vm->program.code_segment =
      blb_range_create(0, ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE, 1, true);

  vm->program.data_segment =
      blb_range_create(ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE,
                       ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE, 1, true);

  vm->program.stack_segment =
      blb_range_create(ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE +
                           ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE,
                       ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE, 1, true);

  memset(vm->program.memory->block->base, 0,
         (vm->program.data_segment->size + vm->program.code_segment->size +
          vm->program.stack_segment->size));

  LDI(&vm->program, R3, 'Z');
  PUSH(&vm->program, R3);

  LDI(&vm->program, R0, ZVM_IO_PORT_SCREEN);
  LDI(&vm->program, R1, ZVM_IO_SERVICE_SCREEN_WRITE_CHARACTER);
  LDI(&vm->program, R2, 1);

  OUT(&vm->program, R0, R1, R2);

  LDI(&vm->program, R0, ZVM_IO_PORT_KEYBOARD);
  LDI(&vm->program, R1, ZVM_IO_SERVICE_KEYBOARD_READ_CHARACTER);
  LDI(&vm->program, R2, 3);

  IN(&vm->program, R0, R1, R2);

  POP(&vm->program, R3);
  POP(&vm->program, R3);
  POP(&vm->program, R3);
}

/* raise exception */
static bool zvm_fetch(zvm_vm_t *vm) {

  uint8_t op, v1, v2, v3;

  if (!zvm_program_code_read(&vm->program, vm->cpu.IP + 0, &op)) {
    zvm_raise(vm, FETCH, FETCH);
    return false;
  }

  if (!zvm_program_code_read(&vm->program, vm->cpu.IP + 1, &v1)) {
    zvm_raise(vm, FETCH, FETCH);
    return false;
  }

  if (!zvm_program_code_read(&vm->program, vm->cpu.IP + 2, &v2)) {
    zvm_raise(vm, FETCH, FETCH);
    return false;
  }
  if (!zvm_program_code_read(&vm->program, vm->cpu.IP + 3, &v3)) {
    zvm_raise(vm, FETCH, FETCH);
    return false;
  }

  zvm_set_instruction(vm->cpu.IR, op, v1, v2, v3);
  vm->cpu.IP += 4;

  return true;
}

static bool zvm_decode(zvm_vm_t *vm) {
  /* input */
  uint8_t val0 = vm->cpu.IR->operands[0].value;
  uint8_t val1 = vm->cpu.IR->operands[1].value;
  uint8_t val2 = vm->cpu.IR->operands[2].value;

  switch (vm->cpu.IR->metadata->handler.type) {
  case ZVM_INST_HANDLER_TYPE_RRR:
    if (val0 >= ZVM_RX_REGISTERS_COUNT || val1 >= ZVM_RX_REGISTERS_COUNT ||
        val2 >= ZVM_RX_REGISTERS_COUNT) {
      zvm_raise(vm, DECODE, INVALID_REGISTER) return false;
    }
    break;

  case ZVM_INST_HANDLER_TYPE_RI:
    if (val0 >= ZVM_RX_REGISTERS_COUNT
        //    || val1 >= 256
        || val2 != 0) {
      zvm_raise(vm, DECODE, BAD_INSTRUCTION) return false;
    }
    break;
  case ZVM_INST_HANDLER_TYPE_RM:
    if (val0 >= ZVM_RX_REGISTERS_COUNT ||
        val1 >= ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE || val2 != 0) {
      zvm_raise(vm, DECODE, BAD_INSTRUCTION) return false;
    }
    break;
  case ZVM_INST_HANDLER_TYPE_RR:
    if (val0 >= ZVM_RX_REGISTERS_COUNT || val1 >= ZVM_RX_REGISTERS_COUNT ||
        val2 != 0) {
      zvm_raise(vm, DECODE, BAD_INSTRUCTION) return false;
    }
    break;
  case ZVM_INST_HANDLER_TYPE_R:
    if (val0 >= ZVM_RX_REGISTERS_COUNT || val1 != 0 || val2 != 0) {
      zvm_raise(vm, DECODE, BAD_INSTRUCTION) return false;
    }
    break;
  }

  return true;
}

static bool zvm_execute(zvm_vm_t *vm) {

  int8_t instruction_index = vm->cpu.IR->metadata->handler.type;
  if (instruction_index >= ZVM_INSTRUCTION_HANDLERS_COUNT) {
    zvm_raise(vm, EXECUTE, EXECUTE);
    return false;
  }

  zfn_instruction_handler_t handler =
      (zfn_instruction_handler_t)vm->cpu.IR->metadata->handler.handler;
  bool instruction_result = false;

  instruction_result =
      handler(vm, vm->cpu.IR->operands[0].value, vm->cpu.IR->operands[1].value,
              vm->cpu.IR->operands[2].value);

  return instruction_result;
}

bool zvm_except(zvm_vm_t *vm) {
  if (vm->has_exception) {
    int8_t code = zvm_exception_get_code(vm);

    if (zvm_exception_is_handler(code)) {
      return exception_handlers[code].handler(
          ZVM_EXCEPTION_HANDLER_FUNCTION_ARGS);
    } else {
      fprintf(stderr, "Invalid exception handler code\n");
      return false;
    }
  }

  return true;
}

int zvm_run(zvm_vm_t *vm) {
  /* execution */
  while (zvm_has_next_instruction(vm)) {
    /* fetch */
    if (!zvm_fetch(vm)) {
      zvm_raise(vm, FETCH, FETCH) goto zvm_catch;
    }
    /* decode  */
    if (!zvm_decode(vm)) {
      zvm_raise(vm, DECODE, DECODE) goto zvm_catch;
    }
    /* execute */
    if (!zvm_execute(vm)) {
      goto zvm_catch;
    }
    continue;
  /* exception handling */
  zvm_catch:
    if (!zvm_except(vm)) {
      break;
    } else {
      zvm_exception_reset(vm)
    }
  }

  if (vm->has_exception) {
    fprintf(stderr, "Exception(%d): %s\n", vm->exception_metadata.code,
            exception_handlers[vm->exception_metadata.code].message);
  }

  /* finalization */
  return 0;
}

bool zvm_load_program(zvm_vm_t *vm, const uint8_t *program,
                      uint8_t program_size) {
  if (vm == NULL || program == NULL) {
    return false;
  }

  if (program_size == 0) {
    return true;
  }

  if (((program_size % ZVM_INSTRUCTION_SIZE) != 0)) {
    zvm_raise(vm, LOAD, LOAD_PROGRAM) return false;
  }

  uint8_t instructions_count = program_size / ZVM_INSTRUCTION_SIZE;
  uint8_t opcode = 0, left_operand = 0, right_operand = 0, output_operand = 0;

  if (instructions_count > ZVM_MAX_INSTRUCTIONS_COUNT) {
    zvm_raise(vm, LOAD, LOAD_PROGRAM) return false;
  }

  vm->program.code_size = 0;

  vm->program.memory =
      blb_blob_create(ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE +
                          ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE +
                          ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE,
                      1);

  vm->program.code_segment =
      blb_range_create(0, ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE, 1, true);

  vm->program.data_segment =
      blb_range_create(ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE,
                       ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE, 1, true);

  vm->program.stack_segment =
      blb_range_create(ZVM_PROGRAM_DEFAULT_CODE_SEGMENT_SIZE +
                           ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE,
                       ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE, 1, true);

  memset(vm->program.memory->block->base, 0,
         (vm->program.data_segment->size + vm->program.code_segment->size +
          vm->program.stack_segment->size));

  for (int i = 0; i < program_size; i += ZVM_INSTRUCTION_SIZE) {
    opcode = program[0 + i];
    left_operand = program[1 + i];
    right_operand = program[2 + i];
    output_operand = program[3 + i];

    zvm_program_load_code_mem(&vm->program, opcode, left_operand, right_operand,
                              output_operand);
  }
  return true;
}

int zvm_main(zvm_vm_t *vm, uint8_t *program, uint8_t program_size) {
  int result = 0;
  if (!zvm_init(vm)) {
    zvm_raise(vm, VM, VM_INIT) return false;
  }

  if (!zvm_load_program(vm, program, program_size)) {
    zvm_except(vm);
  }

  result = zvm_run(vm);
  zvm_release(vm);
  return result;
}
