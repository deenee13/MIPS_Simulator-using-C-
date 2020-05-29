// gcc -o main mips_core.c main.c -I .

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// File Declaration 
#include "mips_core.h"






/////////////////////////////////////////////////
//
//
//
//         GLOBAL VARIABLES and STRUCT
//
//
///////////////////////////////////////////////////

void instruction_fetch (struct instruction_fetch if_register);
void instruction_decode ( struct instruction_fetch if_register);


int main (int argc, char*argv[])
{
    struct instruction_fetch if_register;
    char *filename = argv[1];
    /* This function reads the text file and store everything in to the flash memory*/
    read_memory_image(filename);

    /*This fucntion Initialises the PC and reads the Instruction using the value of PC from memory*/
    instruction_fetch (if_register);

    /* This function decodes the instruction by taking the Struct of IF as argument*/
    instruction_decode ( if_register);

    ////update_simulator ();
 

    return 0;
}



