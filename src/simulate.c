#include "memory.h"
#include "read_elf.h"
#include "simulate.h"
#include "common.h"
#include <stdio.h>

struct CPU cpu;


struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols* symbols){
    cpu = init_cpu();
    struct Stat dummy;
    return dummy;
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