#include "mips_core.h"


/////////////////////////////////////////////
//
//
//      GLOBAL VARIBALES DECLARATION
//
//
//////////////////////////////////////////////


struct mips_register registers;
struct r_type r_type;
struct i_type i_type;
struct global_variables global_variables;


struct instruction_fetch if_register;








void read_memory_image ( char *filename)
{
     
    FILE *fptr = fopen ( filename, "r" );
    unsigned int address;
    unsigned int i = 0;

    if ( fptr != NULL )
    {
        /* or other suitable maximum line size */
        //TODO: <why?>
        char read_data [ 11 ]; 
        /* read a line */
         while ( fgets ( read_data, sizeof read_data, fptr ) != NULL ) 
        {
           
            address = strtol(read_data, NULL, 16);
           
            printf("%08x\n",address);
            flash_memory[i] = address;
            printf("Data in flash memory: %08x\n", flash_memory[i]);
            i++;
           
        }
        fclose ( fptr );
    }
    /* why didn't the file open */
    else
    {
        perror ( filename ); 
    }
}

#if 0
void update_simulator ()
{

    registers.program_counter = 0;
    while(global_variables.opcode != HALT)
    {
        /* Fetching the instruction from the Flash memory */
        global_variables.get_instruction = flash_memory[registers.program_counter];
        instruction_decode(global_variables.get_instruction);
        execution_stage();
        ////printf("Result of get_instruction = %08X\n", get_instruction);
        global_variables.temp_pc = (global_variables.temp_pc + 4);
        registers.program_counter = ((global_variables.temp_pc)/4);
    }

}
#endif


void instruction_fetch (struct instruction_fetch if_register)
{
    ////struct instruction_fetch if_register;
    uint32_t temp_pc;
    if_register.pc = 0;   // Initialising the Program Counter
    if_register.get_instruction = flash_memory[if_register.pc]; // Using the PC to fetch Instruction from local Flash memory 
    temp_pc = temp_pc +4; // Incrementing the local PC by 4
    if_register.pc = (temp_pc/4);  // Manipulating the local PC to get the Correct instrcution fetch from the local memory 

    printf("Result of get_instruction = %08X\n", if_register.get_instruction);
}   





void instruction_decode ( struct instruction_fetch if_register )
{
    struct instruction_decode id_register; 

    printf(" Hex: %08X\n", if_register.get_instruction);
    id_register.opcode = ((if_register.get_instruction >> OPCODE) & OPCODE_MASK);  
    ////printf(" opcode value in Hex: %X\n", opcode);
    printf(" opcode value in decimal : %d\n",id_register.opcode);

    // IF for the Immediate Type
    if (id_register.opcode & 0x1)
    {
        id_register.immediate_type.immediate = ((if_register.get_instruction  ) & IMMEDIATE_VALUE_MASK);
        printf("Immediate value: %d\n",id_register.immediate_type.immediate);
        id_register.immediate_type.rs = ((if_register.get_instruction >> REGISTER_S) &  REGISTER_MASK);
        printf("RS value: %X\n",i_type.rs);
        id_register.immediate_type.rt = ((if_register.get_instruction >> REGISTER_T) &  REGISTER_MASK);
        printf("RT value: %X\n",id_register.immediate_type.rt);
    }

    // Else if block for the LOAD and STORE TYPE
    else if (id_register.opcode == 12 || id_register.opcode == 13)
    {
        id_register.immediate_type.immediate = ((if_register.get_instruction) & IMMEDIATE_VALUE_MASK);
        printf("Immediate value: %d\n",id_register.immediate_type.immediate);
        id_register.immediate_type.rs = ((if_register.get_instruction >> REGISTER_S) &  REGISTER_MASK);
        printf("RS value: %X\n",id_register.immediate_type.rs);
        id_register.immediate_type.rt = ((if_register.get_instruction >> REGISTER_T) &  REGISTER_MASK);
        printf("RT value: %X\n",id_register.immediate_type.rt);
    }

    // Else block for the Register Type
    else
    {
        id_register.register_type.rs = ((if_register.get_instruction >> REGISTER_S) &  REGISTER_MASK);
        printf("RS value: %X\n",id_register.register_type.rs);
        id_register.register_type.rt = ((if_register.get_instruction >> REGISTER_T) &  REGISTER_MASK);
        printf("RT value: %X\n",id_register.register_type.rt);
        id_register.register_type.rd = ((if_register.get_instruction >> REGISTER_D) &  REGISTER_MASK);
        printf("RD value: %X\n",id_register.register_type.rd);
    }
    
    
}



void execution_stage ()
{
    uint32_t memory_reference;
    switch(global_variables.opcode)
    {
        
        case(LOAD):
        memory_reference = registers.register_array[r_type.rs] + i_type.immediate;
        memory_reference = (memory_reference / 4);
        registers.register_array[i_type.rt] = flash_memory[memory_reference];
        break;

        case(STORE):
        memory_reference = registers.register_array[r_type.rs] + i_type.immediate;
        memory_reference = (memory_reference / 4);
        flash_memory[memory_reference] = registers.register_array[i_type.rt];
        break;
        //<TODO: Check the functionality of the load and store function>

        case(ADD):
        registers.register_array[r_type.rd] =  registers.register_array[r_type.rs] + registers.register_array[r_type.rt];
        printf("Value of destination register in addition: %d\n",registers.register_array[r_type.rd]);
        break;

        case(ADDI):
        registers.register_array[i_type.rt] =  registers.register_array[r_type.rs] + i_type.immediate;
        printf("Value of destination register in addI: %d\n",registers.register_array[i_type.rt]);
        break;

        case(SUB):
        registers.register_array[r_type.rd] =  registers.register_array[r_type.rs] - registers.register_array[r_type.rt];
        printf("Value of destination register in SUB: %d\n",registers.register_array[r_type.rd]);
        break;

        case(SUBI):
        registers.register_array[i_type.rt] =  registers.register_array[r_type.rs] - i_type.immediate;
        printf("Value of destination register in SUBI: %d\n",registers.register_array[r_type.rd]);
        break;
        
        case(MUL):
        registers.register_array[r_type.rd] =  registers.register_array[r_type.rs] * registers.register_array[r_type.rt];
        printf("Value of destination register in Multiplication: %d\n",registers.register_array[r_type.rd]);
        break;

        case(MULI):
        registers.register_array[i_type.rt] =  registers.register_array[r_type.rs] * i_type.immediate;
        break;

        case(AND):
        registers.register_array[r_type.rd] =  registers.register_array[r_type.rs] & registers.register_array[r_type.rt];
        printf("Value of destination register in logical AND: %d\n",registers.register_array[r_type.rd]);
        break;

        case(ANDI):
        registers.register_array[i_type.rt] =  registers.register_array[r_type.rs] & i_type.immediate;
        break;

        case(OR):
        registers.register_array[r_type.rd] =  registers.register_array[r_type.rs] | registers.register_array[r_type.rt];
        printf("Value of destination register in logical OR: %d\n",registers.register_array[r_type.rd]);
        break;

        case(ORI):
        registers.register_array[i_type.rt] =  registers.register_array[r_type.rs] | i_type.immediate;
        break;

        case(XOR):
        registers.register_array[r_type.rd] =  registers.register_array[r_type.rs] ^ registers.register_array[r_type.rt];
        printf("Value of destination register in logical XOR: %d\n",registers.register_array[r_type.rd]);
        break;

        case(XORI):
        registers.register_array[i_type.rt] =  registers.register_array[r_type.rs] ^ i_type.immediate;
        break;
    
   // <TODO: ADD Default>
    }
}


