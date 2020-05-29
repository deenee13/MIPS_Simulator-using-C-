// gcc -o main mips_core.c main.c -I .

//<TODO:- Implement for the multiple instruction same functionality>

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

/*void instruction_fetch (struct mips_core *mips_core_instance);
void instruction_decode ( struct mips_core *mips_core_instance);
void execution_stage (struct mips_core *mips_core_instance);
void mem_stage (struct mips_core *mips_core_instance);
void write_back_stage (struct mips_core *mips_core_instance);*/

void update_simulator ();

int main (int argc, char*argv[])
{
    ////struct mips_core mips_core_instance;
    char *filename = argv[1];
    /* This function reads the text file and store everything in to the flash memory*/
    read_memory_image(filename);

    
    // Five Stages of the Pipeline taking Same instance of the Struct as the argument 
 /*   instruction_fetch (&mips_core_instance);
    instruction_decode (&mips_core_instance);
    execution_stage ( &mips_core_instance);
    mem_stage (&mips_core_instance);
    write_back_stage (&mips_core_instance);*/

    update_simulator ();
 

    return 0;
}



