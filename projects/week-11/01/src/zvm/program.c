#include "../../include/zvm/zvm.h"

bool zvm_program_segment_write(zvm_program_t *program, blb_range_t *segment,
                               uint8_t offset, uint8_t value) {
  if (!program || !segment || !program->memory)
    return false;
  uint32_t address = segment->start + offset;
  if (!blb_range_in(segment, address)) {
    return false;
  }

  if (!blb_blob_block_write_at(program->memory, address, value)) {
    return false;
  }

  return true;
}

bool zvm_program_segment_read(zvm_program_t *program, blb_range_t *segment,
                              uint8_t offset, uint8_t *out) {
  if (!program || !segment || !out || !program->memory)
    return false;
  uint32_t address = segment->start + offset;

  if (!blb_range_in(segment, address)) {
    return false;
  }

  if (!blb_blob_block_read_at(program->memory, address, out)) {
    return false;
  }

  return true;
}

bool zvm_program_load_code_mem(zvm_program_t *program, uint8_t opcode,
                               uint8_t left, uint8_t right, uint8_t output) {
  if (!zvm_program_code_write(program, program->code_size + 0, opcode))
    return false;

  if (!zvm_program_code_write(program, program->code_size + 1, left))
    return false;

  if (!zvm_program_code_write(program, program->code_size + 2, right))
    return false;

  if (!zvm_program_code_write(program, program->code_size + 3, output))
    return false;

  program->code_size += ZVM_INSTRUCTION_SIZE;

  return true;
}