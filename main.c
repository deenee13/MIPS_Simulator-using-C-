// gcc -o main mips_core.c main.c -I .

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// File Declaration 
#include "mips_core.h"






       

//////////////////////////////////////////////
//
//
//          FUNCTION PROTOTYPE
//
//
///////////////////////////////////////////////

void instruction_decode (unsigned int add);



/////////////////////////////////////////////////
//
//
//
//         GLOBAL VARIABLES and STRUCT
//
//
///////////////////////////////////////////////////



int main (int argc, char*argv[])
{
    
    char *filename = argv[1];
    read_memory_image(filename);

    update_simulator ();
 

    return 0;
}



