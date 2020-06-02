#include "mips_core.h"


/////////////////////////////////////////////
//
//
//      GLOBAL VARIBALES DECLARATION
//
//
//////////////////////////////////////////////


struct mips_register registers;
void instruction_fetch (struct mips_core *mips_core_instance);
void instruction_decode ( struct mips_core *mips_core_instance);
void execution_stage (struct mips_core *mips_core_instance);
void mem_stage (struct mips_core *mips_core_instance);
void write_back_stage (struct mips_core *mips_core_instance);
void update_pc_normal (struct mips_core *mips_core_instance);
void update_pc_unconditional_branch (struct mips_core *mips_core_instance);
void update_pc_conditional_branch (struct mips_core *mips_core_instance);





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
        printf(" Writing Instruction into the flash_memory .........................................\n\n");
         while ( fgets ( read_data, sizeof read_data, fptr ) != NULL ) 
        {
           
            address = strtol(read_data, NULL, 16);
           
            ////printf("%08x\n",address);
            flash_memory[i] = address;
           //// printf("Data in flash memory: %08x\n", flash_memory[i]);
            i++;
           
        }
        printf("-----------------------Instructions Successfully written into flash_memory------------------\n\n");
        fclose ( fptr );
    }
    /* why didn't the file open */
    else
    {
        perror ( filename ); 
    }
}

////#if 0
void update_simulator ()

{
    struct mips_core mips_core_instance;
    uint8_t i = 0;

    // Initialising the General Purpose Register
    for(i = 0; i<=31; i++ )
    {
        registers.register_array[i] = 0;
    }

    // Initialising the Program Counter and zero and jump flag
    mips_core_instance.pc = 0; 
    mips_core_instance.temp_pc = 0; 
    mips_core_instance.zero_flag = false;
    mips_core_instance.jump_flag = false;   
    mips_core_instance.arithmetic_count = 0;
    mips_core_instance.logical_count = 0;
    mips_core_instance.total_count = 0;
    mips_core_instance.control_flow_count = 0;
    mips_core_instance.memory_access_count = 0;
    while(mips_core_instance.opcode != HALT)
    {
         
        instruction_fetch (&mips_core_instance);
        instruction_decode (&mips_core_instance);
        execution_stage ( &mips_core_instance);
        mem_stage (&mips_core_instance);
        write_back_stage (&mips_core_instance);
    }
}
////#endif


void instruction_fetch (struct mips_core *mips_core_instance)
{
    
    if (mips_core_instance->zero_flag == true)
    {

      //// update_pc_conditional_branch (mips_core_instance);
       mips_core_instance->get_instruction = flash_memory[mips_core_instance->pc]; 
       


    }

    else if (mips_core_instance->jump_flag == true)
    {

      ////  update_pc_unconditional_branch (mips_core_instance);
        mips_core_instance->get_instruction = flash_memory[mips_core_instance->pc];  
        
    }
    else
    {
       //// update_pc_normal (mips_core_instance);
        mips_core_instance->get_instruction = flash_memory[mips_core_instance->pc];  
        

    }
} 

/*Updation of the PC[Program Counter ] during Conditional Branch Execution*/
void update_pc_conditional_branch (struct mips_core *mips_core_instance)
{
    // Update the PC with the immediate value to jump to the Xth instruction
    mips_core_instance->pc = (mips_core_instance->pc - 1);
    mips_core_instance->pc = mips_core_instance->immediate_type.immediate + mips_core_instance->pc;   
    printf(" Updated Value of the pc: %d\n",mips_core_instance->pc);   
    // Fetch the Instruction from the updated pc value
    mips_core_instance->get_instruction = flash_memory[mips_core_instance->pc];  
    printf("Result of get_instruction = %08X\n", mips_core_instance->get_instruction);
    // Update the value of temp_pc accoding to the PC
    mips_core_instance->temp_pc =   mips_core_instance->pc * 4;  
    printf("Updated value of the temp_pc: %d\n",mips_core_instance->temp_pc); 
    // Increment value of the temp_pc
    mips_core_instance->temp_pc =   mips_core_instance->temp_pc + 4;  
    printf("Increment value of the temp_pc: %d\n",mips_core_instance->temp_pc); 
    // Increment value of the PC
    mips_core_instance->pc =   mips_core_instance->temp_pc/4;
    printf("Increment value of the pc: %d\n",mips_core_instance->pc); 
    mips_core_instance->zero_flag = false;
}

/*Updation of the PC[Program Counter ] during Unconditional Branch Execution*/
void update_pc_unconditional_branch (struct mips_core *mips_core_instance)
{   
    // Update the temp_pc with the Value in the Register RS
    mips_core_instance->temp_pc  = registers.register_array[mips_core_instance->immediate_type.rs];
    printf("Updated Value of the temp_pc: %d\n",mips_core_instance->temp_pc);
    // Manipulating the value of the temp_PC to get the actual Instruction from the flash_memory 
    mips_core_instance->pc = ((mips_core_instance->temp_pc)/4); 
    printf("Updated Value of the pc: %d\n",mips_core_instance->pc);
    // Fetching the Instruction from the flash memory
    mips_core_instance->get_instruction = flash_memory[mips_core_instance->pc];  
    printf("Result of get_instruction = %08X\n", mips_core_instance->get_instruction);
    // Incrementing the value of the temp_pc
    mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
    printf("Incremented value of the temp_pc: %d\n",mips_core_instance->temp_pc);
    // Incrementing the value of the PC according to the temp_pc
    mips_core_instance->pc = mips_core_instance->temp_pc/4;
    printf("Incremented value of the pc: %d\n",mips_core_instance->pc);
    // Setting the jump flag to 0 
    mips_core_instance->jump_flag = false;

}


/* Updation of the PC[Program Counter ] during normal Execution*/
void update_pc_normal (struct mips_core *mips_core_instance)
{
    // Using the PC to fetch Instruction from local Flash memory 
    mips_core_instance->get_instruction = flash_memory[mips_core_instance->pc];     
    printf("Result of get_instruction = %08X\n", mips_core_instance->get_instruction);
    printf("Current Value fo the PC: %d\n",mips_core_instance->pc);
    printf("Current value of the temp_pc: %d\n",mips_core_instance->temp_pc);
    // Incrementing the local PC by 4
    mips_core_instance->temp_pc = mips_core_instance->temp_pc +4;                   
    printf(" Incremented Value of the temp_pc: %d\n",mips_core_instance->temp_pc);
    // Manipulating the value of the temp_PC to get the actual Instruction from the flash_memory
    mips_core_instance->pc = (mips_core_instance->temp_pc/4); 
    printf("Incremented value of the PC: %d\n",mips_core_instance->pc);                    
        
}





void instruction_decode ( struct mips_core *mips_core_instance )
{

    printf(" Hex: %08X\n", mips_core_instance->get_instruction);
    mips_core_instance->opcode = ((mips_core_instance->get_instruction >> OPCODE) & OPCODE_MASK);  
    printf(" opcode value in decimal : %d\n",mips_core_instance->opcode);

    // IF for the Immediate Type
    if (mips_core_instance->opcode & 0x1)
    {
        mips_core_instance->immediate_type.immediate = ((mips_core_instance->get_instruction  ) & IMMEDIATE_VALUE_MASK);
        printf("Immediate value: %d\n",mips_core_instance->immediate_type.immediate);
        mips_core_instance->immediate_type.rs = ((mips_core_instance->get_instruction >> REGISTER_S) &  REGISTER_MASK);
        printf("RS value: %X\n",mips_core_instance->immediate_type.rs);
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

    // Else if block for the control flow instruction BEQ is taken care in the Immediate type as its opcode is 15(01111)
    else if (mips_core_instance->opcode == 14 || mips_core_instance->opcode == 16)
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
        mips_core_instance->memory_reference = registers.register_array[mips_core_instance->immediate_type.rs] + mips_core_instance->immediate_type.immediate;
        printf("Value of the register rs in register type:%d\n",registers.register_array[mips_core_instance->register_type.rs]);
        printf("Value of the register rs in immediate type: %d\n",mips_core_instance->immediate_type.rs);
        printf("Value of register type: %d\n",mips_core_instance->register_type.rs);
        printf("Value of the register r1 in array:%d\n",registers.register_array[1]);
        printf("Final value of the memory_reference variable: %d\n",mips_core_instance->memory_reference);
        mips_core_instance->memory_reference = (mips_core_instance->memory_reference / 4);
        printf("Final value of the memory_reference variable after dividing it by 4: %d\n",mips_core_instance->memory_reference);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->total_count++;
        mips_core_instance->memory_access_count++;
        break;

        case(STORE):
        mips_core_instance->memory_reference = registers.register_array[mips_core_instance->immediate_type.rs] + mips_core_instance->immediate_type.immediate;
        mips_core_instance->memory_reference = (mips_core_instance->memory_reference / 4);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->total_count++;
        mips_core_instance->memory_access_count++;
        break;
        
        case(ADD):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] + registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in addition: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->arithmetic_count++;
        mips_core_instance->total_count++;
        break;

        case(ADDI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->immediate_type.rs] + mips_core_instance->immediate_type.immediate;
        printf("Value of Temporary register in addI: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->arithmetic_count++;
        mips_core_instance->total_count++;
        break;

        case(SUB):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] - registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in SUB: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->arithmetic_count++;
        mips_core_instance->total_count++;
        break;

        case(SUBI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->immediate_type.rs] - mips_core_instance->immediate_type.immediate;
        printf("Value of Temporary register in SUBI: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->arithmetic_count++;
        mips_core_instance->total_count++;
        break;
        
        case(MUL):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] * registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in Multiplication: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->arithmetic_count++;
        mips_core_instance->total_count++;
        break;

        case(MULI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->immediate_type.rs] * mips_core_instance->immediate_type.immediate;
        printf("Value of Temporary register in MULI: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->arithmetic_count++;
        mips_core_instance->total_count++;
        break;

        case(AND):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] & registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in logical AND: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->logical_count++;
        mips_core_instance->total_count++;
        break;

        case(ANDI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->immediate_type.rs] & mips_core_instance->immediate_type.immediate;
        printf("Value of Temporary register in logical ANDI: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->logical_count++;
        mips_core_instance->total_count++;
        break;

        case(OR):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] | registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in logical OR: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->logical_count++;
        mips_core_instance->total_count++;
        break;

        case(ORI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->immediate_type.rs] | mips_core_instance->immediate_type.immediate;
        printf("Value of Temporary register in logical ORI: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->logical_count++;
        mips_core_instance->total_count++;
        break;

        case(XOR):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->register_type.rs] ^ registers.register_array[mips_core_instance->register_type.rt];
        printf("Value of Temporary register in logical XOR: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->logical_count++;
        mips_core_instance->total_count++;
        break;

        case(XORI):
        mips_core_instance->alu_temp =  registers.register_array[mips_core_instance->immediate_type.rs] ^ mips_core_instance->immediate_type.immediate;
        printf("Value of Temporary register in logical XORI: %d\n",mips_core_instance->alu_temp);
        mips_core_instance->pc = mips_core_instance->pc + 1;
        mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
        mips_core_instance->logical_count++;
        mips_core_instance->total_count++;
        break;

        case(BEQ):
        if( (registers.register_array[mips_core_instance->immediate_type.rt]) == (registers.register_array[mips_core_instance->immediate_type.rs]) )  
        {
            mips_core_instance->zero_flag = true;
            printf("Conditional BEQ Branch is taken\n");
            mips_core_instance->pc = (mips_core_instance->immediate_type.immediate + mips_core_instance->pc);
            mips_core_instance->temp_pc = (mips_core_instance->pc * 4);
            mips_core_instance->control_flow_count++;
            mips_core_instance->total_count++;
        }
        else
        {
            printf("Branch BEQ is not taken\n");
            mips_core_instance->pc = mips_core_instance->pc + 1;
            mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
            mips_core_instance->control_flow_count++;
            mips_core_instance->total_count++;
        }
        break;

        case(BZ):
        if((registers.register_array[mips_core_instance->immediate_type.rs]) == 0)
        {
            mips_core_instance->zero_flag = true;
            printf("Conditional BZ  Branch is taken\n");
            mips_core_instance->pc = (mips_core_instance->immediate_type.immediate + mips_core_instance->pc);
            mips_core_instance->temp_pc = (mips_core_instance->pc * 4);
            mips_core_instance->control_flow_count++;
            mips_core_instance->total_count++;
        }
        else
        {
            printf("Branch BZ is not taken\n");
            mips_core_instance->pc = mips_core_instance->pc + 1;
            mips_core_instance->temp_pc = mips_core_instance->temp_pc + 4;
            mips_core_instance->control_flow_count++;
            mips_core_instance->total_count++;
        }
        break;

        case(JR):
            mips_core_instance->jump_flag = true;
            printf(" Unconditional Branch is taken\n");
            mips_core_instance->temp_pc  = registers.register_array[mips_core_instance->immediate_type.rs];
            mips_core_instance->pc = (mips_core_instance->temp_pc / 4);
            mips_core_instance->control_flow_count++;
            mips_core_instance->total_count++;
        break;
        
        // Simulator Should never reach the Default state
        default:
        printf("simulator has Reached Default Case in Execute Stage\n");
        break;

    }
}


void mem_stage (struct mips_core *mips_core_instance)
{
    switch(mips_core_instance->opcode)
    {
        case(LOAD):
        printf("-------------------MEM STAGE-------------------\n");
        printf("Value of the memory reference: %d\n",mips_core_instance->memory_reference);
        mips_core_instance->alu_temp = flash_memory[mips_core_instance->memory_reference];
        printf("Value in the alu_temp %d\n",mips_core_instance->alu_temp);
        break;

        case(STORE):
        flash_memory[mips_core_instance->memory_reference] = registers.register_array[mips_core_instance->immediate_type.rt];
        printf("Value in the STORE Instruction: %d\n",flash_memory[mips_core_instance->memory_reference] );
        printf("\n");
        break;

        default:
        printf(" It was not a Load or Store Instruction\n");
        printf("\n");
        break;
    }
}


void write_back_stage (struct mips_core *mips_core_instance)
{
    switch(mips_core_instance->opcode)
    {
        
        case(LOAD):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        printf("Value in the LOAD Instruction: %d\n",registers.register_array[mips_core_instance->immediate_type.rt]);
        printf("\n");
        break;

        case(ADD):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in addition: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        printf("\n");
        break;

        case(ADDI):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        printf("Value of destination register in addI: %d\n",registers.register_array[mips_core_instance->immediate_type.rt]);
        printf("\n");
        break;

        case(SUB):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in SUB: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        printf("\n");
        break;

        case(SUBI):
        registers.register_array[mips_core_instance->immediate_type.rt] =  mips_core_instance->alu_temp;
        printf("Value of destination register in SUBI: %d\n",registers.register_array[mips_core_instance->immediate_type.rt]);
        printf("\n");
        break;
        
        case(MUL):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in Multiplication: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        printf("\n");
        break;

        case(MULI):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        printf("Value of destination register in MULI: %d\n",registers.register_array[mips_core_instance->immediate_type.rt]);
        printf("\n");
        break;

        case(AND):
        registers.register_array[mips_core_instance->register_type.rd] = mips_core_instance->alu_temp;
        printf("Value of destination register in logical AND: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        printf("\n");
        break;

        case(ANDI):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        printf("Value of destination register in logical ANDI: %d\n",registers.register_array[mips_core_instance->immediate_type.rt]);
        printf("\n");
        break;

        case(OR):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in logical OR: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        printf("\n");
        break;

        case(ORI):
        registers.register_array[mips_core_instance->immediate_type.rt] = mips_core_instance->alu_temp;
        printf("Value of destination register in logical ORI: %d\n",registers.register_array[mips_core_instance->immediate_type.rt]);
        printf("\n");
        break;

        case(XOR):
        registers.register_array[mips_core_instance->register_type.rd] =  mips_core_instance->alu_temp;
        printf("Value of destination register in logical XOR: %d\n",registers.register_array[mips_core_instance->register_type.rd]);
        printf("\n");
        break;

        case(XORI):
        registers.register_array[mips_core_instance->immediate_type.rt] =  mips_core_instance->alu_temp;
        printf("Value of destination register in logical XORI: %d\n",registers.register_array[mips_core_instance->immediate_type.rt]);
        printf("\n");
        break;
    
        default:
        printf("simulator has Reached Default Case in Write Back Stage\n");
        printf("\n");
        printf("logical inst: %d\n",mips_core_instance->logical_count);
        printf("Arithmetic inst: %d\n",mips_core_instance->arithmetic_count);
        printf("Total inst: %d\n",mips_core_instance->total_count);
        printf("Control inst: %d\n",mips_core_instance->control_flow_count);
        printf("Memory access inst: %d\n",mips_core_instance->memory_access_count);
     /*   for(int j = 0; j<=31; j++ )
        {
           printf("Register [%d]: %d\n",j ,registers.register_array[j]);
        }
        printf("content of flash memory: %d\n",flash_memory[1400/4]);
        printf("content of flash memory: %d\n",flash_memory[1404/4]);
        printf("content of flash memory: %d\n",flash_memory[1408/4]);*/

        break;

    }
}

