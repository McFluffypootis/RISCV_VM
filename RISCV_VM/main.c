#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
/* unix only */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

#include "RISCV_VM.h"


//TODO
//1 define the I type instructions test with addi on all allowed registers

#define MEMORY_MAX 1 << 16
char memory[MEMORY_MAX];

//Registers
uint32_t reg[R_COUNT]; 

//void exec_r_type(uint32_t instr) {
//    //extract the op fields
//    char funct3 = (instr >> 11) & 0x7;
//    char funct7 = (instr >> 24);
//
//    char rs1 = (instr >> 15) & 0x1F; 
//    char rs2 = (instr >> 19) & 0x1F; 
//    char rd = (instr >> 6) & 0x1F; 
//
//    switch(funct3) {
//        case 0x0: 
//            if(funct7) {
//                reg[rd] = reg[rs1] - reg[rs2];
//            } else {
//                reg[rd] = reg[rs1] + reg[rs2];
//            }
//            break;
//        case 0x1: 
//                reg[rd] = reg[rs1] << reg[rs2];
//            break;
//        case 0x2: 
//                reg[rd] = ((int32_t)reg[rs1] < (int32_t)reg[rs2]);
//            break;
//        case 0x3: 
//                reg[rd] = (reg[rs1] < reg[rs2]);
//            break;
//        case 0x4: 
//                reg[rd] = (reg[rs1] ^ reg[rs2]);
//            break;
//        case 0x5: 
//            if(funct7) {
//                reg[rd] = ((int32_t)reg[rs1] >> reg[rs2]);
//            } else {
//                reg[rd] = (reg[rs1] >> reg[rs2]);
//            }
//            break;
//        case 0x6: 
//                reg[rd] = (reg[rs1] | reg[rs2]);
//            break;
//        case 0x7: 
//                reg[rd] = (reg[rs1] & reg[rs2]);
//            break;
//        default: 
//            printf("ERROR Invalid opcode at %u", R_PC);
//            exit(2);
//            break;
//    }
//}
//
//void exec_i1_type(uint32_t instr) {
//    //extract the op fields
//    char funct3 = (instr >> 11) & 0x7;
//    uint16_t imm = (instr >> 19);
//
//    char rs1 = (instr >> 15) & 0x1F; 
//    char rd = (instr >> 6) & 0x1F; 
//
//    switch(funct3) {
//        case 0x0: 
//            reg[rd] = memory[imm + reg[rs1]] & 0xFF;
//            break;
//        case 0x1: 
//                reg[rd] = reg[rs1] << reg[rs2];
//            break;
//        case 0x2: 
//                reg[rd] = ((int32_t)reg[rs1] < (int32_t)reg[rs2]);
//            break;
//        case 0x4: 
//                reg[rd] = (reg[rs1] ^ reg[rs2]);
//            break;
//        case 0x5: 
//            if(funct7) {
//                reg[rd] = ((int32_t)reg[rs1] >> reg[rs2]);
//            } else {
//                reg[rd] = (reg[rs1] >> reg[rs2]);
//            }
//            break;
//        default: 
//            printf("ERROR Invalid opcode at %u", R_PC);
//            exit(2);
//            break;
//    }
//}

void exec_i2_type(uint32_t instr) {
    //extract the op fields
    char funct3 = (instr >> 12) & 0x7;
    char funct7 = (instr >> 25);
    printf("funct3: %d \n", funct3);
    printf("funct7: %d \n", funct7);

    int16_t imm = ((int32_t)instr >> 20);
    printf("imm: %x \n", imm);

    char rs1 = (instr >> 15) & 0x1F; 
    char rd = (instr >> 7) & 0x1F; 

    printf("rs: %x \n", rs1);
    printf("rd: %x \n", rd);
    switch(funct3) {
        case 0x0: //addi
            reg[rd] = reg[rs1] + ((int32_t)instr >> 20);
            break;
        case 0x1: //slli
                reg[rd] = reg[rs1] << imm;
            break; //slti
        case 0x2: 
                reg[rd] = ((int32_t)reg[rs1] < (int32_t)imm);
            break; //slti
        case 0x3: 
                reg[rd] = ((uint32_t)reg[rs1] < (uint32_t)imm);
            break;
        case 0x4: 
                reg[rd] = (reg[rs1] ^ (int32_t)imm);
            break;
        case 0x5: 
            if(funct7)
                reg[rd] = ((uint32_t)reg[rs1] ^ (uint32_t)imm);
            else
                reg[rd] = ((int32_t)reg[rs1] ^ (uint32_t)imm);
            break;
        case 0x6: 
                reg[rd] = (reg[rs1] | (int32_t)imm);
            break;
        case 0x7: 
                reg[rd] = (reg[rs1] & (int32_t)imm);
            break;
        default: 
            printf("ERROR Invalid opcode at %u", R_PC);
            exit(2);
            break;
    }
}

//void exec_s_type(uint32_t instr) {
//    //extract the op fields
//    char funct3 = (instr >> 11) & 0x7;
//    char imm_2 = (instr >> 24) & 0x7F;
//    char imm_1 = (instr >> 6) & 0x1F;
//
//    uint16_t imm = (instr >> 19);
//
//    char rs1 = (instr >> 15) & 0x1F; 
//
//    switch(funct3) {
//        case 0x0: 
//            memory[rs1 + imm] = (char)reg[rs2];
//            break;
//        case 0x1: 
//            memory[rs1 + imm] = (short)reg[rs2];
//            break;
//        case 0x2: 
//            memory[rs1 + imm] = reg[rs2];
//            break;
//        default: 
//            printf("ERROR Invalid opcode at %u", R_PC);
//            exit(2);
//            break;
//    }
//}
//
//void exec_b_type(uint32_t instr) {
//    //extract the op fields
//    char funct3 = (instr >> 11) & 0x7;
//
//    uint16_t imm = (instr >> 19);
//
//    int rs1 = (instr >> 14) & 0x1F; 
//    int rs2 = (instr >> 19) & 0x1F; 
//
//    char rd = (instr >> 6) & 0x1F; 
//
//    uint32_t BTA;
//
//    switch(funct3) {
//        case 0x0: 
//            if(rs1 == rs2) reg[R_PC] = BTA; 
//            break;
//        case 0x1: 
//            if(rs1 != rs2) reg[R_PC] = BTA; 
//            break;
//        case 0x4: 
//            if(rs1 < rs2) reg[R_PC] = BTA; 
//            break;
//        case 0x5: 
//            if(rs1 >= rs2) reg[R_PC] = BTA; 
//            break;
//        case 0x6: 
//            if((uint32_t)rs1 < (uint32_t)rs2) reg[R_PC] = BTA; 
//            break;
//        case 0x7: 
//            if((uint32_t)rs1 >= (uint32_t)rs2) reg[R_PC] = BTA; 
//            break;
//        default: 
//            printf("ERROR Invalid opcode at %u", R_PC);
//            exit(2);
//            break;
//    }
//}

// flags -s -v -f
enum { SILENT, VERBOSE, FULL };
static int print_status;

// Draw the registers i in [ xi | ABI Name | HEXValue ] 
void draw_registers() {

    //zero 
    printf("[ x0 | zero | 0x%08x ]\n", reg[R_ZERO]);

    printf("[ x1 | ra   | 0x%08x ]\n", reg[1]);
    printf("[ x2 | sp   | 0x%08x ]\n", reg[2]);
    printf("[ x3 | gp   | 0x%08x ]\n", reg[3]);
    printf("[ x4 | tp   | 0x%08x ]\n", reg[4]);


    int reg_i = 5;

    //temporaries
    int t = 0;
    for(reg_i; reg_i < 8; reg_i++) {
        printf("[ x%d  | t%d  | 0x%08x ]\n",reg_i, t++, reg[reg_i]);
    }

     
    t = 0;
    for(reg_i; reg_i < 10; reg_i++) {
        printf("[ x%d  | s%d  | 0x%08x ]\n",reg_i, t++, reg[reg_i]);
    }

    t = 0;
    for(reg_i; reg_i < 18; reg_i++) {
        printf("[ x%d | a%d  | 0x%08x ]\n",reg_i, t++, reg[reg_i]);
    }

    t = 2;
    for(reg_i; reg_i < 26; reg_i++) {
        printf("[ x%d | s%d  | 0x%08x ]\n",reg_i, t++, reg[reg_i]);
    }

    t = 2;
    for(reg_i; reg_i < 28; reg_i++) {
        printf("[ x%d | s%d  | 0x%08x ]\n",reg_i, t++, reg[reg_i]);
    }

    t = 3;
    for(reg_i; reg_i < 32; reg_i++) {
        printf("[ x%d | t%d  | 0x%08x ]\n",reg_i, t++, reg[reg_i]);
    }

    //pc
    printf("[ pc  | pc  | 0x%08x ]\n", reg[R_PC]);
}



//Test instruction
const uint32_t input[] = {0xfff98993,
                          0x00499993}; // t0 += 1

int main(int argc, char* argv[]) {
    
    if(argc < 2 && argc > 4) {
        printf("riscvvm -flags [file] \n ");
        exit(2);
    } 
    
    //Defuault is silent mode
    print_status = SILENT;
    int i = 1;
    for(i; i < (argc - 1); i++ ) {
        if(strcmp(argv[i], "-s") == 0) print_status = SILENT; 
        else if(strcmp(argv[i], "-v") == 0) print_status = VERBOSE; 
        else if(strcmp(argv[i], "-f") == 0) print_status = FULL; 
        else {
            printf("Error: Unknown flag");
            exit(2);
        }
    }
    printf(" Filename:  %s \n ", argv[i]);
    if(print_status == 1)
        printf("Print mode:  verbose \n");
    else if( print_status == 2) 
        printf("Print mode: full \n");

    reg[R_PC] = 0x8000;

    exec_i2_type(input[0]); 
    exec_i2_type(input[1]); 
    //int running = 1; 
    //while(running) {
    //    uint32_t instr = ;
    //    reg[R_PC] += 4;
    //    
    //    char op = instr & 0x3F;
    //    switch(op) {
    //        case R_TYPE:
    //             exec_r_type(instr);
    //            break;
    //        case I1_TYPE:
    //             exec_i1_type(instr);
    //            break;
    //        case I2_TYPE:
    //             exec_i2_type(instr);
    //            break;

    //        case S_TYPE:
    //             exec_s_type(instr);
    //            break;
    //        
    //        case B_TYPE:
    //             exec_b_type(instr);
    //            break;

           // case ADDUIPC:
           //     rd = (char)(instr >> 6) & 0x1F;
           //     reg[rd] = ((uint32_t)(instr >> 11) << 12 )+ reg[R_PC];
           //     break;

           // case LUI:
           //     rd = (char)(instr >> 6) & 0x1F;
           //     reg[rd] = ((uint32_t)(instr >> 11) << 12 );
           //     break;

           // case JALR:
           //     reg[R_PC] = reg[rs1];
           //     rd = (char)(instr >> 6) & 0x1F;
           //     reg[rd] = reg[R_PC] + 4;
           //     break;
           // case JAL:
           // //    reg[R_PC] = JTA;
           //     rd = (char)(instr >> 6) & 0x1F;
           //     reg[rd] = reg[R_PC] + 4;
           //     break;
            //default:
           //     break;
        //}

        if( print_status > 0) draw_registers();
    //}

    return 0;
}
