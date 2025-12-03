#include "disassemble.h"
#include "memory.h"
#include <stdio.h>
#include <stdint.h>
#include "common.h"

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

        case 0x7:
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

static void disas_s_type(char *result, size_t buf_size, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t funct3, uint32_t funct7)
{
    const char *operation = NULL;

    switch (funct3) {
        case 0x0:
            operation = "sb";

            break;
        case 0x1:
            operation = "sh";

            break;
        case 0x2:
            operation = "sw";
          
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

static void disas_b_type(char *result, size_t buf_size, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t funct3, uint32_t funct7)
{
    const char *operation = NULL;

    switch (funct3) {
        case 0x0:
            operation = "beq";

            break;
        case 0x1:
            operation = "bne";

            break;
        case 0x4:
            operation = "blt";
          
            break;

        case 0x5:
            operation = "bge";
          
            break;

        case 0x6:
            operation = "bltu";
          
            break;

        case 0x7:
            operation = "bgeu";
          
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

static void disas_u_type(char *result, size_t buf_size, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t funct3, uint32_t funct7)
{
    const char *operation = NULL;

    switch (funct3) {
        case 0x0:
            operation = "beq";

            break;
        case 0x1:
            operation = "bne";

            break;
        case 0x4:
            operation = "blt";
          
            break;

        case 0x5:
            operation = "bge";
          
            break;

        case 0x6:
            operation = "bltu";
          
            break;

        case 0x7:
            operation = "bgeu";
          
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
    
    rv_fields_t f = {0};
    f.opcode = instruction & 0x7F;

    // Overwrite the content in the buffer with the operation name etc.
    switch (f.opcode) {
        case 0x33: { //R-type ALU
            decode_r(instruction, &f); 
            disas_r_type(result, buf_size,f.rd, f.rs1, f.rs2, f.funct3, f.funct7);
            }
            break;
        case 0x13: //I-type ALU
            
            break;

        case 0x03: // loads
        
            break;
    
        case 0x23: // Stores-type
             
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

}

