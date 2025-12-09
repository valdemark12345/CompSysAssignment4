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

/* static void disas_i_type */

// Disassembler for I-type instruktioner (opcode = 0.x13)
static void disas_i_type(char *result, size_t buf_size, uint32_t rd, uint32_t rs1, uint32_t funct3, int32_t imm)
{
    const char *operation = NULL;

    switch (funct3) {
        case 0x0: // ADDI
            operation = "addi";
            break;

        case 0x2: // SLTI
            operation = "slti";
            break;

        case 0x3: // SLTIU
            operation = "sltiu";
            break;

        case 0x4: // XORI
            operation = "xori";
            break;

        case 0x6: // ORI
            operation = "ori";
            break;

        case 0x7: // ANDI
            operation = "andi";
            break;

        case 0x1: // SLLI
            operation = "slli";
            imm &= 0x1F;
            break;

        case 0x5:
            if ((imm >> 10) & 1) {  // Bit 30 = 1 → SRAI
                operation = "srai";
            } else {
                operation = "srli";
            }
            imm &= 0x1F;     // shamt = imm[4:0]
            break;

        default:
            operation = "unknown";
            break;
        }

    snprintf(result, buf_size, "%s %s, %s, %d", operation, reg_names[rd], reg_names[rs1], imm);
        
}

static void disas_s_type(char *result, size_t buf_size, uint32_t rs1, uint32_t rs2, uint32_t funct3, uint32_t imm)
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
            "%s %s, %d(%s)",
            operation,
            reg_names[rs2],
            imm,
            reg_names[rs1]);
    }
}

static void disas_b_type(char *result, size_t buf_size, uint32_t rs1, uint32_t rs2, uint32_t funct3, uint32_t imm)
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
                 "%s %s,%s,%d",
                 operation,
                 reg_names[rs1],
                 reg_names[rs2],
                imm);
    }
}

static void disas_load_type(char *result, size_t buf_size, uint32_t rd, uint32_t rs1, uint32_t funct3, int32_t imm)
{
    const char *operation = NULL;

    switch (funct3) {
        case 0x0:
            operation = "lb";

            break;
        case 0x1:
            operation = "lh";

            break;
        case 0x2:
            operation = "lw";

            break;
        case 0x4:
            operation = "lbu";

            break;
        case 0x5:
            operation = "lhu";

            break;

        default:
            operation = "unknown";

            break;
    }

    snprintf(result, buf_size, "%s %s, %d(%s)", operation, reg_names[rd], imm, reg_names[rs1]);
}



void disassemble(uint32_t addr, uint32_t instruction, char* result, size_t buf_size){
    (void)addr;
    if (buf_size == 0) return;

    rv_fields_t f = {0};
    f.opcode = instruction & 0x7F;
    const char *operation = NULL;


    // Overwrite the content in the buffer with the operation name etc.
    switch (f.opcode) {
        case 0x33: { //R-type ALU
            decode_r(instruction, &f);
            disas_r_type(result, buf_size,f.rd, f.rs1, f.rs2, f.funct3, f.funct7);
            }
            break;
        case 0x13: {//I-type ALU
            decode_i(instruction, &f);
            disas_i_type(result, buf_size,f.rd, f.rs1, f.funct3, f.imm);
            break;
            }
        case 0x03: { // loads
            decode_i(instruction, &f);
            disas_load_type(result, buf_size, f.rd, f.rs1, f.funct3, f.imm);
            }
            break;
    
        case 0x23: { // Stores-type
            decode_s(instruction, &f);
            disas_s_type(result, buf_size, f.rs1, f.rs2, f.funct3, f.imm);
            break;
            }
        case 0x63: { //branches ALU
            decode_b(instruction, &f);
            disas_b_type(result, buf_size, f.rs1, f.rs2, f.funct3, f.imm);
            break;
        }
        case 0x37: { //lui ALU
            decode_u(instruction, &f);
            operation = "lui";
            if (operation)
            {
                snprintf(result, buf_size,
                        "%s %s,%d",
                        operation,
                        reg_names[f.rd],
                        f.imm);
            }
            break;
            }
        case 0x17: { //auipc ALU
            decode_u(instruction, &f);
            operation = "auipc";

            if (operation)
            {
                snprintf(result, buf_size,
                        "%s %s,%d",
                        operation,
                        reg_names[f.rd],
                        f.imm);
            }
            break;
            }

        case 0x6F: { //jal ALU
            decode_j(instruction, &f);

            operation = "jal";

            if (operation)
            {
                snprintf(result, buf_size,
                        "%s %s,%d",
                        operation,
                        reg_names[f.rd],
                        f.imm);
            }
            break;
            }
        case 0x67: { //jalr ALU
            decode_i(instruction, &f);

            operation = "jalr";

            if (operation)
            {
                snprintf(result, buf_size,
                        "%s %s, %s, %d",
                        operation,
                        reg_names[f.rd],
                        reg_names[f.rs1],
                        f.imm);
            }
            break;
            }
        case 0x73: { //ecall ALU
            snprintf(result, buf_size, "ecall");    
            break;
            }
    }   
}

