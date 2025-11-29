#include "disassemble.h"
#include "memory.h"
#include <stdio.h>
#include <stdint.h>

static const char *reg_names[32] = {
    "zero","ra","sp","gp","tp","t0","t1","t2",
    "s0","s1","a0","a1","a2","a3","a4","a5",
    "a6","a7","s2","s3","s4","s5","s6","s7",
    "s8","s9","s10","s11","t3","t4","t5","t6"
};

// Disassembler for R-type instruktioner (opcode = 0x33).
static void disas_r_type(char *result, size_t buf_size, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t funct3, uint32_t funct7)
{
    const char *operation = NULL;

    switch (funct3) {
        case 0x0:
            if (funct7 == 0x00) {
                operation = "add";
            }
            else if (funct7 == 0x20) {
                operation = "sub";
            }
            else if (funct7 == 0x01) {
                operation = "mul";
            }
            break;

        case 0x1:
            if (funct7 == 0x00) {
                operation = "sll";
            } 
            else if (funct7 == 0x01) {
                operation = "mulh";
            } 
            break;

        case 0x2:
            if (funct7 == 0x00) {
                operation = "slt";
            }
            else if (funct7 == 0x01) {
                operation = "mulhsu";
            }
            break;

        case 0x3:
            if (funct7 == 0x00) {
                operation = "sltu";
            }
            else if (funct7 == 0x01) {
                operation = "mulhu";
            }
            break;

        case 0x4:
            if (funct7 == 0x00) {
                operation = "xor";
            }
            else if (funct7 == 0x01) {
                operation = "div";
            }
                break;

        case 0x5:
            if (funct7 == 0x00) {
                operation = "srl";
            }
            else if (funct7 == 0x20) {
                operation = "sra";
            }
            else if (funct7 == 0x01) {
                operation = "divu";
            }
            break;

        case 0x6:
            if (funct7 == 0x00) {
                operation = "or";
            }
            else if (funct7 == 0x01) {
                operation = "rem";
            } 
            break;

        case 0x7:  // AND / REMU
            if (funct7 == 0x00) {
                operation = "and";
            }
            else if (funct7 == 0x01) {
                operation = "remu";
            }
            break;

        default:
            break;
    }

    if (operation)
    {
        snprintf(result, buf_size,
                 "%s %s,%s,%s",
                 operation,
                 reg_names[rd],
                 reg_names[rs1],
                 reg_names[rs2]);
    }
}


void disassemble(uint32_t addr, uint32_t instruction, char* result, size_t buf_size, struct symbols* symbols){

    // RISC-V Fields
    uint32_t opcode = instruction & 0x7F;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x07;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    uint32_t funct7 = (instruction >> 25) & 0x7F;

    //Fill the buffer with the original content (Denne linje skal slettes når vi har lavet alle cases)
    snprintf(result, buf_size, "opcode=%02x rd=%u rs1=%u rs2=%u f3=%x f7=%02x", opcode, rd, rs1, rs2, funct3, funct7);

    // Overwrite the content in the buffer with the operation name etc.
    switch (opcode) {
        case 0x33: { //R-type ALU
            disas_r_type(result, buf_size,rd, rs1, rs2, funct3, funct7);
            }
            break;
        case 0x13: //I-type ALU
            
            break;

        case 0x03: // loads ALU
        
            break;
    
        case 0x23: // S-type
             
            break;

        case 0x63: //branches ALU
            
            break;

        case 0x37: //lui ALU
            
            break;

        case 0x17: //auipc ALU
         
            break;

        case 0x6F: //jal ALU
            
            break;

        case 0x67: //jalr ALU
            
            break;

        case 0x73: //ecall ALU
            
            break;
    }   

// 126
}
