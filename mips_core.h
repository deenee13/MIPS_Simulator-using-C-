#ifndef mips_core_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>




//////////////////////////////////////////////
//
//
//          FUNCTION PROTOTYPE
//
//
///////////////////////////////////////////////


void read_memory_image ( char *filename);
////void instruction_decode ( struct instruction_fetch if_register);
////void update_simulator ();
////void execution_stage();
////void instruction_fetch (struct instruction_fetch if_register)

///////////////////////////////////////////
//
//
//              DECLARATION
//
//
///////////////////////////////////////////

#define  OPCODE                     26
#define  OPCODE_MASK                0x3F


#define  REGISTER_T                 16
#define  REGISTER_D                 11
#define  REGISTER_S                 21
#define  REGISTER_MASK              0x1F

#define  IMMEDIATE_VALUE            0
#define  IMMEDIATE_VALUE_MASK       0xFFFF

/* Macros for the Each Instruction depending upon the opcode*/

// Arithmetic Instructions
#define ADD                           0
#define ADDI                          1
#define SUB                           2
#define SUBI                          3  
#define MUL                           4  
#define MULI                          5 

// Logical Instructions
#define OR                            6  
#define ORI                           7  
#define AND                           8
#define ANDI                          9
#define XOR                           10  
#define XORI                          11  

// Load and Store 
#define LOAD                          12
#define STORE                         13  

// Halt Operation
#define HALT                          17


//////////////////////////////////////////////
//
//
//   VARIABLES and STRUCT DEFINATION 
//
//
///////////////////////////////////////////////


// Store the data coming from the memory image file into the array 
unsigned int flash_memory [1024];

  

  
  struct r_type
{
    uint8_t rs;
    uint8_t rt;
    uint8_t rd;
};


struct i_type
{
    uint8_t rs;
    uint8_t rt;
    int16_t immediate;
};


struct mips_core
{
    uint32_t pc;
    unsigned int get_instruction;
    struct r_type register_type;
    struct i_type immediate_type;
    uint8_t opcode;
    uint32_t memory_reference;
    uint32_t alu_temp;
};


struct mips_register 
{
    uint32_t program_counter;
    uint32_t register_array [31];
};


#endif