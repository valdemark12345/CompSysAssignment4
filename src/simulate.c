#include "simulate.h"
#include "common.h"
#include "memory.h"
#include "read_elf.h"
#include <stdio.h>

struct CPU cpu;

int load_word_from_memory(void) { return (memory_rd_w(cpu.mem, cpu.pc)); }

struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols *symbols)
{
  cpu.pc = start_addr;
  struct Stat stats;
  int instruction;
  instruction;
  while (cpu.pc != 0)
  {
    instruction = load_word_from_memory();
    // Decode instruction and do it
    //  Depending on what type of instruction, do something different with PC.
    stats.insns += 1;
    cpu.pc += 4; // Increment program counter
  }
  // Start program by loading instructions from PC
  // Write down each instruction in log file
  // Add 1 to stats per instruction
  return stats;
}

// R-types

void add(int dest, int reg1, int reg2)
{
  cpu.registers[dest] = cpu.registers[reg1] + cpu.registers[reg2];
}

void mul(int dest, int reg1, int reg2)
{
  cpu.registers[dest] = cpu.registers[reg1] * cpu.registers[reg2];
}

void sub(int dest, int reg1, int reg2)
{
  cpu.registers[dest] = cpu.registers[reg1] - cpu.registers[reg2];
}

void xor(int dest, int reg1, int reg2)
{
  cpu.registers[dest] = cpu.registers[reg1] ^ cpu.registers[reg2];
}

void or(int dest, int reg1, int reg2)
{
  cpu.registers[dest] = cpu.registers[reg1] | cpu.registers[reg2];
}

void and(int dest, int reg1, int reg2)
{
  cpu.registers[dest] = cpu.registers[reg1] & cpu.registers[reg2];
}

void sll(int dest, int reg1, int reg2)
{
  uint32_t shamt = cpu.registers[reg2] & 0x1F;
  cpu.registers[dest] = cpu.registers[reg1] << shamt;
}

void srl(int dest, int reg1, int reg2)
{
  uint32_t shamt = cpu.registers[reg2] & 0x1F;
  cpu.registers[dest] = cpu.registers[reg1] >> shamt;
}

void sra(int dest, int reg1, int reg2)
{
  uint32_t shamt = cpu.registers[reg2] & 0x1F; // RV32 shift amount is 0–31
  int32_t val = (int32_t)cpu.registers[reg1];  // reinterpret as signed
  cpu.registers[dest] = (uint32_t)(val >> shamt);
}

void slt(int dest, int reg1, int reg2)
{
  int32_t val = (int32_t)cpu.registers[reg1]; // Cast val to signed integer.
  int32_t val2 = (int32_t)cpu.registers[reg2];
  cpu.registers[dest] = (val < val2) ? 1 : 0;
}

void sltu(int dest, int reg1, int reg2)
{
  cpu.registers[dest] = (cpu.registers[reg1] < cpu.registers[reg2]) ? 1 : 0;
}

void mulh(int dest, int reg1, int reg2){
    int64_t prod = (int64_t)cpu.registers[reg1] * (int64_t)cpu.registers[reg2];
    int32_t result = prod >> 32;
    cpu.registers[dest] = result;
}

void mulsu(int dest, int reg1, int reg2){
    int64_t prod = (int64_t)cpu.registers[reg1] * (uint64_t)cpu.registers[reg2];
    int32_t result = prod >> 32;
    cpu.registers[dest] = result;
}

void mulu(int dest, int reg1, int reg2){
    uint64_t prod = (uint64_t)cpu.registers[reg1] * (uint64_t)cpu.registers[reg2];
    uint32_t result = prod >> 32;
    cpu.registers[dest] = result;
}

void div(int dest, int reg1, int reg2){
    int32_t result = (int32_t)cpu.registers[reg1] / (int32_t)cpu.registers[reg2];
    cpu.registers[dest] = result;
}

void divu(int dest, int reg1, int reg2){
    uint32_t result = (uint32_t)cpu.registers[reg1] / (uint32_t)cpu.registers[reg2];
    cpu.registers[dest] = result;
}

void rem(int dest, int reg1, int reg2){
    int32_t result = (int32_t)cpu.registers[reg1] % (int32_t)cpu.registers[reg2];
    cpu.registers[dest] = result;
}

void remu(int dest, int reg1, int reg2){
    uint32_t result = (uint32_t)cpu.registers[reg1] % (uint32_t)cpu.registers[reg2];
    cpu.registers[dest] = result;
}


// I-types

int check_immediate(int immediate)
{
  if (immediate > 0xfff)
  {
    printf("Immediate was too large! \n");
    return 0;
  }
  else
    return 1;
}

void addi(int dest, int reg, int imm)
{
  if (check_immediate(imm))
  {
    cpu.registers[dest] = cpu.registers[reg] + imm;
  }
}

void xori(int dest, int reg, int imm)
{
  if (check_immediate(imm))
  {
    cpu.registers[dest] = cpu.registers[reg] ^ imm;
  }
}

void ori(int dest, int reg, int imm)
{
  if (check_immediate(imm))
  {
    cpu.registers[dest] = cpu.registers[reg] | imm;
  }
}

void andi(int dest, int reg, int imm)
{
  if (check_immediate(imm))
  {
    cpu.registers[dest] = cpu.registers[reg] & imm;
  }
}

void slli(int dest, int reg, int imm)
{
  if (check_immediate(imm))
  {
    cpu.registers[dest] = cpu.registers[reg] << imm;
  }
}

void srli(int dest, int reg, int imm)
{
  if (check_immediate(imm))
  {
    cpu.registers[dest] = cpu.registers[reg] >> imm;
  }
}

void srai(int dest, int reg, int imm)
{
  if (check_immediate(imm))
  {
    uint32_t shamt = imm & 0x1F;               // RV32 shift amount is 0–31
    int32_t val = (int32_t)cpu.registers[reg]; // reinterpret as signed
    cpu.registers[dest] = (uint32_t)(val >> shamt);
  }
}

void slti(int dest, int reg1, int imm)
{
  if (check_immediate(imm))
  {
    int32_t val = (int32_t)cpu.registers[reg1]; // Cast val to signed integer.
    cpu.registers[dest] = (val < imm) ? 1 : 0;
  }
}

void sltui(int dest, int reg1, int imm)
{
  if (check_immediate(imm))
  {
    cpu.registers[dest] = (cpu.registers[reg1] < (uint32_t)imm) ? 1 : 0;
  }
}

void jalr(int dest, int reg1, int imm){
  if (check_immediate(imm))
    {
      cpu.registers[dest] = cpu.pc + 4;
      cpu.pc = cpu.registers[reg1] + imm;
    }
}

// Loading instructions. All of them are cast to a type before being properly stored as uint32_t

void lb(int dest, int imm, int reg)
{
  if (check_immediate(imm))
  {
    int addr = cpu.registers[reg] + imm;
    int8_t val = memory_rd_b(cpu.mem, addr);
    cpu.registers[dest] = (uint32_t)val;
  }
}

void lw(int dest, int imm, int reg)
{
  if (check_immediate(imm))
  {
    int addr = cpu.registers[reg] + imm;
    int32_t val = memory_rd_w(cpu.mem, addr);
    cpu.registers[dest] = (uint32_t)val;
  }
}

void lh(int dest, int imm, int reg)
{
  if (check_immediate(imm))
  {
    int addr = cpu.registers[reg] + imm;
    int16_t val = memory_rd_h(cpu.mem, addr);
    cpu.registers[dest] = (uint32_t)val;
  }
}

void lbu(int dest, int imm, int reg)
{
  if (check_immediate(imm))
  {
    int addr = cpu.registers[reg] + imm;
    uint8_t val = memory_rd_b(cpu.mem, addr);
    cpu.registers[dest] = (uint32_t)val;
  }
}

void lhu(int dest, int imm, int reg)
{
  if (check_immediate(imm))
  {
    int addr = cpu.registers[reg] + imm;
    uint16_t val = memory_rd_b(cpu.mem, addr);
    cpu.registers[dest] = (uint32_t)val;
  }
}

// S types

void sb(int reg1, int reg2, int imm){
    if (check_immediate(imm))
    {
      int addr = cpu.registers[reg1] + imm;
      uint8_t val = (uint8_t)cpu.registers[reg2];
      memory_wr_b(cpu.mem, addr, val);
  }
}

void sh(int reg1, int reg2, int imm){
    if (check_immediate(imm))
    {
      int addr = cpu.registers[reg1] + imm;
      uint16_t val = (uint16_t)cpu.registers[reg2];
      memory_wr_h(cpu.mem, addr, val);
  }
}

void sw(int reg1, int reg2, int imm){
    if (check_immediate(imm))
    {
      int addr = cpu.registers[reg1] + imm;
      uint32_t val = (uint32_t)cpu.registers[reg2];
      memory_wr_w(cpu.mem, addr, val);
  }
}


// U types

int check_upper_immediate(int upper_immediate)
{
  if (upper_immediate > 0xfffff)
  {
    printf("upper_immediate was too large! \n");
    return 0;
  }
  else
    return 1;
}

void lui(int dest, int upper_immediate)
{
  if (check_upper_immediate(upper_immediate))
  {
    cpu.registers[dest] = upper_immediate;
    cpu.registers[dest] = cpu.registers[dest] << 12;
  }
}

void auipc(int dest, int upper_immediate)
{
  if (check_upper_immediate(upper_immediate))
  {
    cpu.registers[dest] = cpu.pc + (upper_immediate << 12);
  }
}

void ecall(void) { cpu.pc = 0; }

