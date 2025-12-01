#include "memory.h"
#include "read_elf.h"
#include "simulate.h"
#include "common.h"
#include <stdio.h>

struct CPU* cpu;


struct Stat simulate(struct memory *mem, int start_addr, FILE *log_file, struct symbols* symbols){
    cpu = init_cpu();
    struct Stat dummy;
    return dummy;
}

void add(int dest, int reg1, int reg2){
    cpu->registers[dest] = cpu->registers[reg1] + cpu->registers[reg2];
}