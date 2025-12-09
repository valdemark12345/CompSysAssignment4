#include "disassemble.h"
#include "memory.h"
#include "simulate.h"
#include <stdint.h>
#include <stdio.h>

// ABI names ordered
static const char *reg_names[32] = {"zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                                    "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                                    "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                                    "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

struct CPU
{
  uint32_t registers[32];
  struct memory *mem;
  uint32_t pc;
  int cpu_running;
};

typedef struct
{
  uint32_t opcode;
  uint32_t rd;
  uint32_t rs1;
  uint32_t rs2;
  uint32_t funct3;
  uint32_t funct7;
  int32_t imm;
} rv_fields_t;

static void decode_r(uint32_t inst, rv_fields_t *f)
{
  f->opcode = inst & 0x7F;
  f->rd = (inst >> 7) & 0x1F;
  f->funct3 = (inst >> 12) & 0x07;
  f->rs1 = (inst >> 15) & 0x1F;
  f->rs2 = (inst >> 20) & 0x1F;
  f->funct7 = (inst >> 25) & 0x7F;
}

static void decode_i(uint32_t inst, rv_fields_t *f)
{
  f->opcode = inst & 0x7F;
  f->rd = (inst >> 7) & 0x1F;
  f->funct3 = (inst >> 12) & 0x07;
  f->rs1 = (inst >> 15) & 0x1F;
  f->imm = (int32_t)(inst) >> 20;
}

static void decode_s(uint32_t inst, rv_fields_t *f)
{
  f->opcode = inst & 0x7F;
  f->funct3 = (inst >> 12) & 0x07;
  f->rs1 = (inst >> 15) & 0x1F;
  f->rs2 = (inst >> 20) & 0x1F;
  int32_t imm = ((inst >> 7) & 0x1F) | (((inst >> 25) & 0x7F) << 5);
  imm = (imm << 20) >> 20; // sign-extend 12 bits
  f->imm = imm;
}

static void decode_b(uint32_t inst, rv_fields_t *f)
{
  f->opcode = inst & 0x7F;
  f->funct3 = (inst >> 12) & 0x07;
  f->rs1 = (inst >> 15) & 0x1F;
  f->rs2 = (inst >> 20) & 0x1F;

  // First define all different imms
  int32_t imm11 = (inst >> 7) & 0x01;
  int32_t imm4_1 = (inst >> 8) & 0x0F;
  int32_t imm10_5 = (inst >> 25) & 0x3F;
  int32_t imm12 = (inst >> 31) & 0x01;

  // Then connect
  int32_t buf_imm = 0;
  buf_imm |= (imm12 << 12);
  buf_imm |= (imm11 << 11);
  buf_imm |= (imm10_5 << 5);
  buf_imm |= (imm4_1 << 1);

  buf_imm = (buf_imm << 19) >> 19;
  f->imm = buf_imm;  
}

static void decode_u(uint32_t inst, rv_fields_t *f)
{
  f->opcode = inst & 0x7F;
  f->rd = (inst >> 7) & 0x1F;
  f->imm = ((inst >> 12) & 0xFFFFF);
}

static void decode_j(uint32_t inst, rv_fields_t *f)
{
  f->opcode = inst & 0x7F;
  f->rd = (inst >> 7) & 0x1F;

  // First define all different imms
  int32_t imm19_12 = (inst >> 12) & 0xFF;
  int32_t imm11 = (inst >> 20) & 0x01;
  int32_t imm10_1 = (inst >> 21) & 0x3FF;
  int32_t imm20 = (inst >> 31) & 0x01;

  // Then connect
  int32_t buf_imm = 0;
  buf_imm |= (imm20 << 20);
  buf_imm |= (imm19_12 << 12);
  buf_imm |= (imm11 << 11);
  buf_imm |= (imm10_1 << 1);
  buf_imm = (buf_imm << 11) >> 11;
  f->imm = buf_imm;
}
