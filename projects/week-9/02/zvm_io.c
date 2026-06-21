#include "zvm_io.h"

bool zfnio_keyboard(zvm_vm_t *vm, uint8_t port, uint8_t operation,
                    uint8_t argc) {

  for (uint8_t i = 0; i < argc; i++) {
    scanf(" %c", &vm->io_devices[port]->O[i]);
  }

  return true;
}

bool zfnio_screen(zvm_vm_t *vm, uint8_t port, uint8_t operation, uint8_t argc) {

  /* functions:
    0: write one character
  */
  // 1, 0, argc
  uint8_t character = vm->io_devices[port]->I[0];
  printf("%c", character);
  return true;
}