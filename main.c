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

void instruction_fetch (struct mips_core *mips_core_instance);
void instruction_decode ( struct mips_core *mips_core_instance);
void execution_stage (struct mips_core *mips_core_instance);
void mem_stage (struct mips_core *mips_core_instance);
void write_back_stage (struct mips_core *mips_core_instance);

////void update_simulator (struct mips_core *mips_core_instance);

int main (int argc, char*argv[])
{
    struct mips_core mips_core_instance;
    char *filename = argv[1];
    /* This function reads the text file and store everything in to the flash memory*/
    read_memory_image(filename);

    
    instruction_fetch (&mips_core_instance);

    
    instruction_decode (&mips_core_instance);

    execution_stage ( &mips_core_instance);

    mem_stage ( &mips_core_instance);

    write_back_stage (&mips_core_instance);

    ////update_simulator ( &mips_core_instance);
 

    return 0;
}



