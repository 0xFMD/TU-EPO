#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*opcode_function_t)(unsigned char, unsigned char);

#define OPCODE_ADD 0
#define OPCODE_SUB 1
#define OPCODE_MUL 2
#define OPCODE_DIV 3
#define OPCODE_MOD 4
#define OPCODE_STP 5
#define OPCODE_LDI 6
#define OPCODE_ADR 7
#define OPCODE_SUR 8
#define OPCODE_INC 9
#define OPCODE_DEC 10
#define OPCODE_JMP 11
#define OPCODE_CMP 12

#define OPCODE_JE 13
#define OPCODE_JZ 13
#define OPCODE_JG 14
#define OPCODE_JGE 15
#define OPCODE_JL 16
#define OPCODE_JLE 17
#define OPCODE_JNE 18

#define OPCODE_LDM 19
#define OPCODE_STI 20
#define OPCODE_STR 21

#define OPCODE_MULR 22
#define OPCODE_DIVR 23
#define OPCODE_MODR 24

#define OPCODE_ADM 25
#define OPCODE_SUBM 26
#define OPCODE_MUM 27
#define OPCODE_DUM 28

#define OPCODE_AND 29
#define OPCODE_OR 30
#define OPCODE_XOR 31
#define OPCODE_SHL 32
#define OPCODE_SHR 33
#define OPCODE_NOT 34

#define OPCODE_CALL 35
#define OPCODE_RET 36

#define DATA_SIZE 10
#define STACK_SIZE 20

#define STACK_BASE_POINTER STACK_SIZE - 1

#define MEMORY_SIZE 1024
#define MEMORY_BASE STACK_SIZE

#define CODE_SIZE sizeof(program)

#define INSTRUCTIONS_COUNT 37
#define INSTRUCTION_SIZE 3

#define LEFT_OPERAND IP + 1
#define RIGHT_OPERAND IP + 2

#define RX_COUNT 4

static unsigned char program[] = {
    /* 30 */ 6,  0,  5,    /* LDI R0, 5    */
    /* 33 */ 6,  1,  10,   /* LDI R1, 10   */
    /* 36 */ 0,  3,  4,    /* ADD 3, 4      */
    /* 39 */ 35, 18, 0,    /* CALL 18       */
    /* 42 */ 9,  1,  0,    /* INC R1        */
    /* 45 */ 5,  0,  0,    /* STP 0         */
    /* 48 */ 9,  0,  0,    /* func - INC R0  */
    /* 51 */ 36, 0,  0,    /* func - RET    */
};

static unsigned char memory[STACK_SIZE + DATA_SIZE + CODE_SIZE];

/* Registers */
static int IP = 0;
static unsigned char IR[INSTRUCTION_SIZE] = {0, 0, 0};
static int OUTPUT = 0;

static int STACK_POINTER = STACK_SIZE - 1;


static unsigned char FLAGS = 0;

#define FLAG_POSITIVE 0
#define FLAG_ZERO 1
#define FLAG_NEGATIVE 2
#define FLAG_OVERFLOW 4

/* 0000 0000 */
/* xxxx xxNZ */

static unsigned char R0 = 0;
static unsigned char R1 = 0;
static unsigned char R2 = 0;
static unsigned char R3 = 0;

static opcode_function_t current_instruction = NULL;

void push_stack(unsigned char value) {
  if (STACK_POINTER < 0) {
    printf("stack overflow!");
    exit(1);
  };

  memory[STACK_POINTER--] = value;
}

int pop_stack() {
  if (STACK_POINTER >= STACK_SIZE) {
    printf("stack empty!");
    exit(1);
  };

  unsigned char out = memory[++STACK_POINTER];
  memory[STACK_POINTER] = 0;

  return out;
}

static void is_overflow(int result) {
  FLAGS = FLAGS & ~FLAG_OVERFLOW;
  if (result > 255) {
    FLAGS |= FLAG_OVERFLOW;
    printf("%d - overflow!", result);
    exit(1);
  }
}

int opcode_add(unsigned char left_operand, unsigned char right_operand) {
  return left_operand + right_operand;
}

int opcode_sub(unsigned char left_operand, unsigned char right_operand) {
  return left_operand - right_operand;
}

int opcode_mul(unsigned char left_operand, unsigned char right_operand) {
  return left_operand * right_operand;
}

int opcode_div(unsigned char left_operand, unsigned char right_operand) {
  if (right_operand == 0) {
    printf("Exception: divide by zero\n");
    exit(1);
  }
  return left_operand / right_operand;
}

int opcode_mod(unsigned char left_operand, unsigned char right_operand) {
  return left_operand % right_operand;
}

static unsigned char get_rx_value(unsigned char operand) {
  switch (operand) {
  case 0:
    return R0;
  case 1:
    return R1;
  case 2:
    return R2;
  case 3:
    return R3;
    // skip default case
  }

  return 255;
}

static void set_rx_value(unsigned char operand, unsigned char value) {
  switch (operand) {
  case 0:
    R0 = value;
    return;
  case 1:
    R1 = value;
    return;
  case 2:
    R2 = value;
    return;
  case 3:
    R3 = value;
    return;
  default:
    printf("Invalid register address\n");
    exit(0);
  }
}

int opcode_ldi(unsigned char left_operand, unsigned char right_operand) {
  switch (left_operand) {
  case 0:
    R0 = right_operand;
    break;
  case 1:
    R1 = right_operand;
    break;
  case 2:
    R2 = right_operand;
    break;
  case 3:
    R3 = right_operand;
    break;
  default:
    printf("Invalid register address\n");
    exit(0);
  }

  return right_operand;
}

int opcode_stp(unsigned char left_operand, unsigned char right_operand) {
  OUTPUT = left_operand;
  exit(left_operand);
}

int opcode_adr(unsigned char left_operand, unsigned char right_operand) {
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  unsigned char Rx = get_rx_value(left_operand) + right_operand;
  is_overflow(Rx);
  set_rx_value(left_operand, Rx);
  return Rx;
}

int opcode_sur(unsigned char left_operand, unsigned char right_operand) {
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  unsigned char Rx = get_rx_value(left_operand) - right_operand;
  is_overflow(Rx);
  set_rx_value(left_operand, Rx);
  return Rx;
}

int opcode_inc(unsigned char left_operand, unsigned char right_operand) {
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  unsigned char Rx = get_rx_value(left_operand);
  Rx++;
  set_rx_value(left_operand, Rx);
  return Rx;
}

int opcode_dec(unsigned char left_operand, unsigned char right_operand) {
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  unsigned char Rx = get_rx_value(left_operand);
  Rx--;
  set_rx_value(left_operand, Rx);
  return Rx;
}

int opcode_jmp(unsigned char left_operand, unsigned char right_operand) {
  IP = MEMORY_BASE + DATA_SIZE + left_operand;
  return left_operand;
}

int opcode_cmp(unsigned char left_operand, unsigned char right_operand) {
  unsigned char Rx = get_rx_value(left_operand);
  unsigned char Ry = get_rx_value(right_operand);

  OUTPUT = Rx - Ry;
  FLAGS = FLAG_POSITIVE;

  if (OUTPUT == 0) {
    FLAGS = FLAG_ZERO;
  } else if (OUTPUT < 0) {
    FLAGS = FLAG_NEGATIVE;
  }

  return FLAGS;
}

int opcode_je(unsigned char left_operand, unsigned char right_operand) {

  if ((FLAGS & 0x3) == FLAG_ZERO) {
    IP = MEMORY_BASE + DATA_SIZE + left_operand;
    return left_operand;
  }

  return 255;
}

int opcode_jg(unsigned char left_operand, unsigned char right_operand) {

  if ((FLAGS & 0x3) == FLAG_POSITIVE) {
    IP = MEMORY_BASE + DATA_SIZE + left_operand;
    return left_operand;
  }

  return 255;
}

int opcode_jge(unsigned char left_operand, unsigned char right_operand) {

  if ((FLAGS & 0x3) != FLAG_NEGATIVE) {
    IP = MEMORY_BASE + DATA_SIZE + left_operand;
    return left_operand;
  }

  return 255;
}

int opcode_jl(unsigned char left_operand, unsigned char right_operand) {

  if ((FLAGS & 0x3) == FLAG_NEGATIVE) {
    IP = MEMORY_BASE + DATA_SIZE + left_operand;
    return left_operand;
  }

  return 255;
}

int opcode_jle(unsigned char left_operand, unsigned char right_operand) {

  if ((FLAGS & 0x3) != FLAG_POSITIVE) {
    IP = MEMORY_BASE + DATA_SIZE + left_operand;
    return left_operand;
  }

  return 255;
}

int opcode_jne(unsigned char left_operand, unsigned char right_operand) {

  if ((FLAGS & 0x3) != FLAG_ZERO) {
    IP = MEMORY_BASE + DATA_SIZE + left_operand;
    return left_operand;
  }

  return 255;
}

int opcode_ldm(unsigned char left_operand, unsigned char right_operand) {
  // ldm, reg, address
  if (right_operand >= DATA_SIZE) {
    printf("Invalid memory address");
    exit(1);
  }
  set_rx_value(left_operand, memory[MEMORY_BASE + right_operand]);

  return get_rx_value(left_operand);
}

int opcode_sti(unsigned char left_operand, unsigned char right_operand) {
  // sti, address, imm
  if (left_operand >= DATA_SIZE) {
    printf("Invalid memory address");
    exit(1);
  }
  memory[MEMORY_BASE + left_operand] = right_operand;

  return memory[MEMORY_BASE + left_operand];
}

int opcode_str(unsigned char left_operand, unsigned char right_operand) {
  // str, reg, address
  if (right_operand >= DATA_SIZE) {
    printf("Invalid memory address");
    exit(1);
  }

  memory[MEMORY_BASE + right_operand] = get_rx_value(left_operand);

  return memory[MEMORY_BASE + right_operand];
}

int opcode_mulr(unsigned char left_operand, unsigned char right_operand) {
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  unsigned char Rx = get_rx_value(left_operand) * right_operand;
  is_overflow(Rx);
  return Rx;
}

int opcode_divr(unsigned char left_operand, unsigned char right_operand) {
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  if (right_operand == 0) {
    printf("Exception: divide by zero\n");
    exit(0);
  }

  unsigned char Rx = get_rx_value(left_operand) / right_operand;
  is_overflow(Rx);
  return Rx;
}

int opcode_modr(unsigned char left_operand, unsigned char right_operand) {
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  unsigned char Rx = get_rx_value(left_operand) % right_operand;
  is_overflow(Rx);
  return Rx;
}

int opcode_adm(unsigned char left_operand, unsigned char right_operand) {
  // ADM, reg, 3
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  if (right_operand >= DATA_SIZE) {
    printf("Invalid memory address");
    exit(1);
  }

  int res = get_rx_value(left_operand) + memory[MEMORY_BASE + right_operand];
  is_overflow(res);

  return res;
}

int opcode_subm(unsigned char left_operand, unsigned char right_operand) {
  // ADM, reg, 3
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  if (right_operand >= DATA_SIZE) {
    printf("Invalid memory address");
    exit(1);
  }

  int res = get_rx_value(left_operand) - memory[MEMORY_BASE + right_operand];
  is_overflow(res);

  return res;
}

int opcode_mum(unsigned char left_operand, unsigned char right_operand) {
  // ADM, reg, 3
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  if (right_operand >= DATA_SIZE) {
    printf("Invalid memory address");
    exit(1);
  }

  int res = get_rx_value(left_operand) * memory[MEMORY_BASE + right_operand];
  is_overflow(res);

  return res;
}

int opcode_dum(unsigned char left_operand, unsigned char right_operand) {
  // ADM, reg, 3
  if (left_operand >= RX_COUNT) {
    printf("Invlaid Rx register address\n");
    exit(0);
  }

  if (right_operand >= DATA_SIZE) {
    printf("Invalid memory address");
    exit(1);
  }
  if (memory[MEMORY_BASE + right_operand] == 0) {
    printf("Exception: divide by zero\n");
    exit(1);
  }

  int res = get_rx_value(left_operand) / memory[MEMORY_BASE + right_operand];

  return res;
}

int opcode_and(unsigned char left_operand, unsigned char right_operand) {
  /* AND imm, imm */
  return left_operand & right_operand;
}

int opcode_or(unsigned char left_operand, unsigned char right_operand) {
  return left_operand | right_operand;
}

int opcode_xor(unsigned char left_operand, unsigned char right_operand) {
  return left_operand ^ right_operand;
}

int opcode_shl(unsigned char left_operand, unsigned char right_operand) {
  return left_operand << right_operand;
}

int opcode_shr(unsigned char left_operand, unsigned char right_operand) {
  return left_operand >> right_operand;
}

int opcode_not(unsigned char left_operand, unsigned char right_operand) {
  return ~left_operand;
}

int opcode_call(unsigned char left_operand, unsigned char right_operand) {
  push_stack(IP);
  IP = MEMORY_BASE + DATA_SIZE + left_operand;
  return IP;
}

int opcode_ret(unsigned char left_operand, unsigned char right_operand) {
  IP = pop_stack();

  return IP;
}

static const opcode_function_t opcode_functions[INSTRUCTIONS_COUNT] = {
    opcode_add,  opcode_sub, opcode_mul,  opcode_div, opcode_mod,  opcode_stp,
    opcode_ldi,  opcode_adr, opcode_sur,  opcode_inc, opcode_dec,  opcode_jmp,
    opcode_cmp,  opcode_je,  opcode_jg,   opcode_jge, opcode_jl,   opcode_jle,
    opcode_jne,  opcode_ldm, opcode_sti,  opcode_str, opcode_mulr, opcode_divr,
    opcode_modr, opcode_adm, opcode_subm, opcode_mum, opcode_dum,  opcode_and,
    opcode_or,   opcode_xor, opcode_shl,  opcode_shr, opcode_not,  opcode_call,
    opcode_ret,
};

static bool cpu_fetch(void) {
  int byte = 0;
  if (IP >= MEMORY_BASE + DATA_SIZE + CODE_SIZE) {
    /* HALT */
    exit(0);
  }

  /* Fetch */
  IR[0] = memory[IP];
  IR[1] = memory[LEFT_OPERAND];
  IR[2] = memory[RIGHT_OPERAND];

  /* Move to the next insturction */
  IP += INSTRUCTION_SIZE;

  return true;
}

static bool cpu_decode(void) {
  if (IR[0] >= INSTRUCTIONS_COUNT) {
    printf("Invalid instruction\n");
    // IR[0] = 10; // rewrite
    IR[0] = IR[1] = IR[2] = 0;
    exit(0);
  }

  current_instruction = opcode_functions[IR[0]];

  return true;
}

static bool cpu_execute(void) {
  OUTPUT = current_instruction(IR[1], IR[2]);
  return true;
}

int main(void) {
  // reset

  for (int i = 0; i < CODE_SIZE; i++) {
    memory[MEMORY_BASE + DATA_SIZE + i] = program[i];
  }
  IP = MEMORY_BASE + DATA_SIZE;
  IR[0] = IR[1] = IR[2] = 0;

  while (true) {
    cpu_fetch();
    cpu_decode();
    cpu_execute();
    printf("output -> %d\n", OUTPUT);
  }

  return 0;
}