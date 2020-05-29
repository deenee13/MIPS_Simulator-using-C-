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

// Updated with the Single Struct 
void instruction_fetch (struct mips_core *mips_core_instance)
{
    ////struct instruction_fetch if_register;
    uint32_t temp_pc;
    mips_core_instance->pc = 0;   // Initialising the Program Counter
    mips_core_instance->get_instruction = flash_memory[mips_core_instance->pc]; // Using the PC to fetch Instruction from local Flash memory 
    temp_pc = temp_pc +4; // Incrementing the local PC by 4
    mips_core_instance->pc = (temp_pc/4);  // Manipulating the local PC to get the Correct instrcution fetch from the local memory 

    printf("Result of get_instruction = %08X\n", mips_core_instance->get_instruction);
}   




// Updated with the Single Struct 
void instruction_decode ( struct mips_core *mips_core_instance )
{

    printf(" Hex: %08X\n", mips_core_instance->get_instruction);
    mips_core_instance->opcode = ((mips_core_instance->get_instruction >> OPCODE) & OPCODE_MASK);  
    ////printf(" opcode value in Hex: %X\n", opcode);
    printf(" opcode value in decimal : %d\n",mips_core_instance->opcode);

    // IF for the Immediate Type
    if (mips_core_instance->opcode & 0x1)
    {
        mips_core_instance->immediate_type.immediate = ((mips_core_instance->get_instruction  ) & IMMEDIATE_VALUE_MASK);
        printf("Immediate value: %d\n",mips_core_instance->immediate_type.immediate);
        mips_core_instance->immediate_type.rs = ((mips_core_instance->get_instruction >> REGISTER_S) &  REGISTER_MASK);
        printf("RS value: %X\n",i_type.rs);
        mips_core_instance->immediate_type.rt = ((mips_core_instance->get_instruction >> REGISTER_T) &  REGISTER_MASK);
        printf("RT value: %X\n",mips_core_instance->immediate_type.rt);
    }

    // Else if block for the LOAD and STORE TYPE
    else if (mips_core_instance->opcode == 12 || mips_core_instance->opcode == 13)
    {
        mips_core_instance->immediate_type.immediate = ((mips_core_instance->get_instruction) & IMMEDIATE_VALUE_MASK);
        printf("Immediate value: %d\n",mips_core_instance->immediate_type.immediate);
        mips_core_instance->immediate_type.rs = ((mips_core_instance->get_instruction >> REGISTER_S) &  REGISTER_MASK);
        printf("RS value: %X\n",mips_core_instance->immediate_type.rs);
        mips_core_instance->immediate_type.rt = ((mips_core_instance->get_instruction >> REGISTER_T) &  REGISTER_MASK);
        printf("RT value: %X\n",mips_core_instance->immediate_type.rt);
    }

    // Else block for the Register Type
    else
    {
        mips_core_instance->register_type.rs = ((mips_core_instance->get_instruction >> REGISTER_S) &  REGISTER_MASK);
        printf("RS value: %X\n",mips_core_instance->register_type.rs);
        mips_core_instance->register_type.rt = ((mips_core_instance->get_instruction >> REGISTER_T) &  REGISTER_MASK);
        printf("RT value: %X\n",mips_core_instance->register_type.rt);
        mips_core_instance->register_type.rd = ((mips_core_instance->get_instruction >> REGISTER_D) &  REGISTER_MASK);
        printf("RD value: %X\n",mips_core_instance->register_type.rd);
    }
    
    
}


void execution_stage (struct mips_core *mips_core_instance)
{
    
    switch(mips_core_instance->opcode)
    {
        
        case(LOAD):
        mips_core_instance->memory_reference = registers.register_array[mips_core_instance->register_type.rs] + mips_core_instance->immediate_type.immediate;
        mips_core_instance->memory_reference = (mips_core_instance->memory_reference / 4);
        ////registers.register_array[mips_core_instance->immediate_type.rt] = flash_memory[memory_reference];
        break;

        case(STORE):
        mips_core_instance->memory_reference = registers.register_array[mips_core_instance->register_type.rs] + mips_core_instance->immediate_type.immediate;
        mips_core_instance->memory_reference = (mips_core_instance->memory_reference / 4);
       //// flash_memory[memory_reference] = registers.register_array[mips_core_instance->immediate_type.rt];
        break;
        //<TODO: Check the functionality of the load and store function>

        case(ADD):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] + registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in addition: %d\n",mips_core_instance->alu_temp);
        break;

        case(ADDI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] + mips_core_instance->immediate_type.immediate;
        printf("Value of Temporary register in addI: %d\n",mips_core_instance->alu_temp);
        break;

        case(SUB):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] - registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in SUB: %d\n",mips_core_instance->alu_temp);
        break;

        case(SUBI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] - mips_core_instance->immediate_type.immediate;
        printf("Value of Temporary register in SUBI: %d\n",mips_core_instance->alu_temp);
        break;
        
        case(MUL):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] * registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in Multiplication: %d\n",mips_core_instance->alu_temp);
        break;

        case(MULI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] * mips_core_instance->immediate_type.immediate;
        break;

        case(AND):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] & registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in logical AND: %d\n",mips_core_instance->alu_temp);
        break;

        case(ANDI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] & mips_core_instance->immediate_type.immediate;
        break;

        case(OR):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] | registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in logical OR: %d\n",mips_core_instance->alu_temp);
        break;

        case(ORI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] | mips_core_instance->immediate_type.immediate;
        break;

        case(XOR):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] ^ registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in logical XOR: %d\n",mips_core_instance->alu_temp);
        break;

        case(XORI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] ^ mips_core_instance->immediate_type.immediate;
        break;
    
        // Simulator Should never reach the Default state
        default:
        printf("simulator has Reached Default Case\n");
        break;

    }
}


void mem_stage (struct mips_core *mips_core_instance)
{
    switch(mips_core_instance->opcode)
    {
        case(LOAD):
        mips_core_instance->alu_temp = flash_memory[mips_core_instance->memory_reference];
        break;

        case(STORE):
        flash_memory[mips_core_instance->memory_reference] = registers.register_array[mips_core_instance->immediate_type.rt];
        break;

        default:
        printf(" It was not a Load or Store Instruction\n");
        break;
    }
}


void write_back_stage (struct mips_core *mips_core_instance)
{
    switch(mips_core_instance->opcode)
    {
        case(LOAD):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        break;

        case(ADD):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in addition: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        break;

        case(ADDI):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        printf("Value of destination register in addI: %d\n",registers.register_array[mips_core_instance->immediate_type.rt]);
        break;

        case(SUB):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in SUB: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        break;

        case(SUBI):
        registers.register_array[mips_core_instance->immediate_type.rt] =  mips_core_instance->alu_temp;
        printf("Value of destination register in SUBI: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        break;
        
        case(MUL):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in Multiplication: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        break;

        case(MULI):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        break;

        case(AND):
        registers.register_array[mips_core_instance->register_type.rd] = mips_core_instance->alu_temp;
        printf("Value of destination register in logical AND: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        break;

        case(ANDI):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        break;

        case(OR):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in logical OR: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        break;

        case(ORI):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        break;

        case(XOR):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in logical XOR: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        break;

        case(XORI):
        registers.register_array[mips_core_instance->immediate_type.rt] =  mips_core_instance->alu_temp;
        break;
    
        
        default:
        printf("simulator has Reached Default Case in Write Back Stage\n");
        break;

    }
}

