#ifndef VM_H
#define VM_H

// define RV32I base instruction set opcodes
typedef enum{
    R_TYPE = 51,
    I1_TYPE = 3,
    I2_TYPE = 19,
    S_TYPE = 35,
    B_TYPE = 99,
    
    //U_types
    ADDUIPC = 23,
    LUI = 55,
    JALR = 103,
    JAL = 111,

}OP_RV32I;


typedef enum{
    R_ZERO = 0,
    R_PC = 32,
    R_COUNT = 33,
}REGISTERS;



#endif
