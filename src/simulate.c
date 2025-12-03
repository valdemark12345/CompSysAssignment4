#include "memory.h"
#include "read_elf.h"
#include "simulate.h"
#include "common.h"
#include <stdio.h>

struct CPU cpu;


struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols* symbols){
    cpu = init_cpu();
    cpu.pc = start_addr;
    struct Stat stats;
    int instruction;
    while(cpu.pc != 0){
        instruction = load_word_from_memory(mem);
        //Decode instruction and do it
        // Depending on what type of instruction, do something different with PC.
        stats.insns += 1;
        cpu.pc += 4; // Increment program counter
    }
    // Start program by loading instructions from PC
    // Write down each instruction in log file
    // Add 1 to stats per instruction
    return stats;
}

int load_word_from_memory(struct memory *mem){
    return (memory_rd_w(mem, cpu.pc));
}



// R-types

void add(int dest, int reg1, int reg2){
    cpu.registers[dest] = cpu.registers[reg1] + cpu.registers[reg2];
}

void mul(int dest, int reg1, int reg2){
    cpu.registers[dest] = cpu.registers[reg1] * cpu.registers[reg2];
}

void sub(int dest, int reg1, int reg2){
    cpu.registers[dest] = cpu.registers[reg1] - cpu.registers[reg2];
}

void xor(int dest, int reg1, int reg2){
    cpu.registers[dest] = cpu.registers[reg1] ^ cpu.registers[reg2];
}

void or(int dest, int reg1, int reg2){
    cpu.registers[dest] = cpu.registers[reg1] | cpu.registers[reg2];
}

void and(int dest, int reg1, int reg2){
    cpu.registers[dest] = cpu.registers[reg1] & cpu.registers[reg2];
}

void sll(int dest, int reg1, int reg2){
    uint32_t shamt = cpu.registers[reg2] & 0x1F;
    cpu.registers[dest] = cpu.registers[reg1] << shamt;
}

void srl(int dest, int reg1, int reg2){
    uint32_t shamt = cpu.registers[reg2] & 0x1F;
    cpu.registers[dest] = cpu.registers[reg1] >> shamt;
}

void sra(int dest, int reg1, int reg2){ 
    uint32_t shamt = cpu.registers[reg2] & 0x1F;   // RV32 shift amount is 0–31
    int32_t val = (int32_t)cpu.registers[reg1];    // reinterpret as signed
    cpu.registers[dest] = (uint32_t)(val >> shamt);
}

void slt(int dest, int reg1, int reg2){
    cpu.registers[dest] = (cpu.registers[reg1] < cpu.registers[reg2])?1:0;
}

void sltu(int dest, int reg1, int reg2){
    cpu.registers[dest] = (cpu.registers[reg1] < cpu.registers[reg2])?1:0;
}


//I-types

int check_immediate(int immediate){
    if (immediate > 0xfff){
        printf("Immediate was too large! \n");
        return 0;
    }
    else return 1;
}


// U types

int check_upper_immediate(int upper_immediate){
    if (upper_immediate > 0xfffff){
        printf("upper_immediate was too large! \n");
        return 0;
    }
    else return 1;
}

void lui(int dest, int upper_immediate){
    if (check_upper_immediate(upper_immediate)){
        cpu.registers[dest] = upper_immediate;
        cpu.registers[dest] << 12;
    }
}

void auipc(int dest, int upper_immediate){
    if (check_upper_immediate(upper_immediate)){
    cpu.registers[dest] = cpu.pc + (upper_immediate << 12);
    }
}

void ecall(){
    cpu.pc = 0;
}