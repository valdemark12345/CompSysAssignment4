#include "disassemble.h"
#include "memory.h"
#include <stdio.h>
#include <stdint.h>
#include "simulate.h"

//ABI names ordered
static const char *reg_names[32] = {
    "zero","ra","sp","gp","tp","t0","t1","t2",
    "s0","s1","a0","a1","a2","a3","a4","a5",
    "a6","a7","s2","s3","s4","s5","s6","s7",
    "s8","s9","s10","s11","t3","t4","t5","t6"
};

struct CPU {
    uint32_t registers[32];
    uint32_t pc;
};

struct CPU init_cpu(){
    struct CPU cpu;
    cpu.registers[0] = 0;
    return cpu;
}

typedef struct {
    uint32_t opcode;
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t funct3;
    uint32_t funct7;
    int32_t imm;
} rv_fields_t;

void decode_r(uint32_t inst, rv_fields_t *f) {
    f->opcode = inst & 0x7F;
    f->rd     = (inst >> 7)  & 0x1F;
    f->funct3 = (inst >> 12) & 0x07;
    f->rs1    = (inst >> 15) & 0x1F;
    f->rs2    = (inst >> 20) & 0x1F;
    f->funct7 = (inst >> 25) & 0x7F;
}

void decode_i(uint32_t inst, rv_fields_t *f) {
    f->opcode = inst & 0x7F;
    f->rd     = (inst >> 7)  & 0x1F;
    f->funct3 = (inst >> 12) & 0x07;
    f->rs1    = (inst >> 15) & 0x1F;
    f->imm    = (int32_t)inst >> 20;
}

void decode_s(uint32_t inst, rv_fields_t *f) {
    f->opcode = inst & 0x7F;
    f->funct3 = (inst >> 12) & 0x07;
    f->rs1    = (inst >> 15) & 0x1F;
    f->rs2    = (inst >> 20) & 0x1F;
    f->imm    = ((inst >> 7) & 0x1F) | (((int32_t)inst >> 25) << 5);
}

