/*
   Instituto Superior de Engenharia de Lisboa
   Arquitectura de Computadores - 2007
*/

#include <stdio.h>
#include "convertions.h"

#define DIM_RAM 256
#define DIM_CODE 2048
#define byte unsigned char
#define word unsigned int

#define RAM_OFFSET 100





/* 
	Opcode
*/

#define mov_addr_rn 0x00		// 0000
#define mov_rn_addr 0x04		// 0100
#define mov_rn_const8 0x02		// 0010
#define mov_rn_ptrRm 0x03		// 0011
#define add_rn_rm 0x0B			// 1011
#define sub_rn_rm 0x09			// 1001
#define jnz_rn_offset8 0x0A		// 1010
#define djnz_rn_offset8 0x08	// 1000
#define push_rn 0x0F			// 1111
#define pop_rn 0x0D				// 1101
#define sjmp_offset8 0x0E		// 1110
#define lcall_caddr 0x0C		// 1100
#define ret 0x01				// 0001
#define hack_to_stop 0x05		// 0101

/* 
	Registers
*/

byte A;					// Acumulator
byte SP;				// Stack Pointer 			
byte MAR;				// Memory *** Address *** Register  
word MBR;				// Memory *** Buffer *** Register   - stores the data being transferred to and from the immediate access store
byte IR;				// Instruction Register		
byte TMP;				// Temp Register

word RAM[DIM_RAM];		// RAM
word REG[4];			// Registers [0..3]
byte CODE[DIM_CODE];	// CODE
word PC;				// Program Counter 	


word SHUTDOWN;			// PROGRAM CONTROL

/* 
	Operators From EFI
*/ 

/* PC -  holds the address of nxt instruction to be executed. */
void RESET() { PC = 0; }
void INC_PC() { PC = (PC + 1) & 0x7FF; }				// & 0111 1111 1111
void RDC() { MAR = CODE[PC]; }							// Read Code -> MAR = Code[PC]
void LD_MBR_PC0_7() { MBR = PC & 0xFF; }				// Loads PC Low part (2bytes) to MBR
void LD_MBR_PC8_10() { MBR = ( PC & 0x700 ) >> 8; }		// Loads PC High part (3 bits) to MBR

/* MAR - stores the memory address from which data will be fetched to the CPU, or the address to which data will be sent and stored. */  
void WDD() { RAM[MAR] = MBR; }	// Write data
void RDD() { MBR = RAM[MAR]; }	// Read data

void WDR() { REG[MAR] = MBR; }	// Write register
void RDR() { MBR = REG[MAR]; }	// Read register

void LD_MBR_MAR() { MBR = MAR; }
void LD_TMP_MAR() { TMP = MAR; }

void LD_A_MAR() { A = MAR; }
void LD_IR_MAR() { IR = MAR; }

void INC_MAR() { MAR = (MAR + 1); }
void DEC_MAR() { MAR = (MAR - 1); }

void ADD() { PC = (PC + ((MAR & 0x80) == 0 ? MAR : 0x700 | MAR )) & 0x7FF;}
								  
/* SP - The data stored in the stack frame may sometimes be accessed directly via the stack pointer register */
void INIT_SP(){ SP = 0x10; }
void INC_SP() { SP = (SP + 1); }
void DEC_SP() { SP = (SP - 1); }
void LD_MAR_SP() { MAR = SP; }

/* IR - stores the instruction currently being executed or decoded */
void LD_MAR_IR3_2() { MAR = (IR >> 2) & 0x03; }
void LD_MAR_IR0_1() { MAR = IR & 0x03; }
void LD_PC_CADDR() { PC = ( ((IR & 0x07 ) << 8) | TMP ) & 0x7FF; }

/* A */
void LD_MAR_A() { MAR = A; }

/* MBR */
void LD_A_MBR() { A = MBR; }
void LD_MAR_MBR() { MAR = MBR; }
void LD_PC_SP_ALL() { PC = ( ((TMP & 0x07) << 8) | MBR ) & 0x7FF; }
void LD_TMP_MBR() { TMP = MBR; }
void DEC_MBR() { MBR = MBR - 1; }

/* Others... */
byte DOIS_BYTES() { return ((IR >> 4) & 0x01) == 0; }
byte FLAG_ZERO() { return A; }

void START() { SHUTDOWN = 0; }
void END()   { SHUTDOWN = 1; }

/* ALU */

void ALU()
{
    switch ((IR >> 4) & 0x0F)
	{
      case add_rn_rm :
        MBR = MBR + A;
        break;
      case sub_rn_rm :
        MBR = MBR - A;
        break;
      default: MBR = A;
     }
 }

/*
	Fetch
*/

void fetch()
{
	RDC();				// MAR = CODE[PC]
	INC_PC();
	LD_IR_MAR();
	if ( DOIS_BYTES() )
	{
		RDC();
		INC_PC();
	}
}

/* Fase Execute */


void execute()
{
  switch ((IR >> 4) & 0x0F)
  {
  	case mov_addr_rn:	   					// MOV addr, Rn
		LD_A_MAR();
		LD_MAR_IR0_1();
		RDR();
		LD_MAR_A();
		WDD();
		break;	 // .. a funcionar

	case mov_rn_addr:					   // MOV Rn, addr
		RDD();				// MBR = RAM[MAR]
		LD_MAR_IR0_1();		// MAR = Rn
		WDR();				// REG[Rn] = MBR
		break;	 // .. a funcionar

	case mov_rn_const8:					  // MOV Rn, #const8
		LD_MBR_MAR();
		LD_MAR_IR0_1();
		WDR();
		break;	 // .. a funcionar

	case mov_rn_ptrRm:					  // MOV Rn, @Rm
		LD_MAR_IR3_2();		// MAR = Rm
		RDR();				// MBR = REG[MAR]
		LD_MAR_MBR();		// MAR = MBR
		RDD();				// MBR = RAM[MAR]
		LD_MAR_IR0_1();		// MAR = Rn
		WDR();				// Rn = MBR (RAM[MAR])
		break;	// .. a funcionar

	case add_rn_rm:					  // ADD Rn, Rm
	case sub_rn_rm:					  // SUB  Rn, Rm
		LD_MAR_IR3_2();
		RDR();
		LD_A_MBR();
		LD_MAR_IR0_1();
		RDR();
		ALU();
		WDR();
		break;  // .. a funcionar

	case jnz_rn_offset8:					  // JNZ Rn, Offset8
		LD_A_MAR();
		LD_MAR_IR0_1();
		RDR();
		LD_MAR_A();
		LD_A_MBR();

		if ( FLAG_ZERO() )
			ADD();
		break;	// .. a funcionar
	case djnz_rn_offset8:					  // DJNZ Rn, Offset8
		LD_A_MAR();
		LD_MAR_IR0_1();
		RDR();
		DEC_MBR();
		WDR();
		LD_MAR_A();
		LD_A_MBR();

		if ( FLAG_ZERO() )
			ADD();
		break;	 // .. a funcionar
	case push_rn:					 // PUSH Rn
		LD_MAR_IR0_1();
		RDR();
		LD_MAR_SP();
		WDD();
		INC_SP();
		break;   // .. a funcionar
	case pop_rn:					 // POP Rn
		DEC_SP();
		LD_MAR_SP();
		RDD();
		LD_MAR_IR0_1();
		WDR();
		break;	// .. a funcionar
	case sjmp_offset8:					 // SJMP Offset8
		ADD();
		break;	// .. a funcionar
	case lcall_caddr:					 // LCALL c_addr
		LD_TMP_MAR();
		LD_MAR_SP();
		LD_MBR_PC0_7();
		WDD();
		INC_MAR();
		INC_SP();
		LD_MBR_PC8_10();
		WDD();
		LD_PC_CADDR();
		INC_SP();
		break;	// .. a funcionar
	case ret:
		DEC_SP();					 // RET
		LD_MAR_SP();
		RDD();
		LD_TMP_MBR();
		DEC_SP();
		DEC_MAR();
		RDD();
		LD_PC_SP_ALL();
		break;	// .. a funcionar

	case hack_to_stop:
		END();
		break;
		
	
	}	
}

/* Program */

void loadProgram()	
{	
	//
	//  -- Pass position of the array to ACUMULA --

	// mov r0, #100 (2 instructions)
	CODE[0] = convDecimal("00100000");
	CODE[1] = 100;

	// push r0 (1 instruction)
	CODE[2] = convDecimal("11110000");


	//
	// -- Function ACUMULA (receives by parameter pointer to array) --

	// pop r0 (1 instruction)
	CODE[3] = convDecimal("11010000");

	// mov addr0, r0
	CODE[4] = convDecimal("00000000");
	CODE[5] = 0;

	// mov r0, #0 (2 instructions)
	CODE[6] = convDecimal("00100000");
	CODE[7] = 0;

	// mov addr1, r0 (2 instructions)
	CODE[8] = convDecimal("00000000");
	CODE[9] = 1;

	// mov r1, #100 (2 instructions)
	CODE[10] = convDecimal("00100001");
	CODE[11] = 100;

	//
	// FOR_LOOP

	// djnz r1, LOOP_INSIDE (2 instructions)
	CODE[12] = convDecimal("10000001");
	CODE[13] = 2;

	//
	// EPILOGUE

	// sjmp $ (2 instructions) / hack_to_stop
	// CODE[14] = convDecimal("11100000");
	// CODE[15] = 0xFE;
	CODE[14] = convDecimal("01010000");
	CODE[15] = 0;

	//
	// LOOP_INSIDE

	// mov r2, addr0 (2 instructions)
	CODE[16] = convDecimal("01000010");
	CODE[17] = 0;

	// add r2, r1 (1 instruction)
	CODE[18] = convDecimal("10110110");

	// mov r3, @r2 (1 instruction)
	CODE[19] = convDecimal("00111011");

	// push r3	(1 instruction)
	CODE[20] = convDecimal("11110011");

	// pop r2 (1 instruction)
	CODE[21] = convDecimal("11010010");

	// sub r2, r1 (1 instruction)
	CODE[22] = convDecimal("10010110");

	// jnz r2, IF_INSIDE (2 instructions)
	CODE[23] = convDecimal("10100010");
	CODE[24] = 2;

	// sjmp FOR_LOOP (2 instructions)
	CODE[25] = convDecimal("11100000");
	CODE[26] = convDecimal("11110001");		// Move 15 units backward

	// 
	// IF_INSIDE

	// mov r0, addr1 (2 instructions)
	CODE[27] = convDecimal("01000000");
	CODE[28] = 1;

	// push r0 (1 instruction)
	CODE[29] = convDecimal("11110000");

	// push r3 (1 instruction)
	CODE[30] = convDecimal("11110011");

	// lcall SOMA (2 instructions)
	CODE[31] = convDecimal("11000000");
	CODE[32] = 37;

	// mov addr1, r0 (2 instructions)
	CODE[33] = convDecimal("00000000");
	CODE[34] = 1;

	// sjmp FOR_LOOP (2 instructions)
	CODE[35] = convDecimal("11100000");
	CODE[36] = convDecimal("11100111");		// Move 25 units backward


	//
	// -- Function SOMA (receives by parameters array[i] and c) --

	//
	// Soma

	// pop r2 (1 instruction)
	CODE[37] = convDecimal("11010010");

	// pop r2 (1 instruction)
	CODE[38] = convDecimal("11010010");

	// pop r3 (1 instruction)
	CODE[39] = convDecimal("11010011");

	// pop r0 (1 instruction)
	CODE[40] = convDecimal("11010000");

	// add r0, r3 (1 instruction)
	CODE[41] = convDecimal("10111100");

	// mov r3, #0 (2 instructions)
	CODE[42] = convDecimal("00100011");
	CODE[43] = 0;

	// push r2	(1 instruction) 
	CODE[44] = convDecimal("11110010");

	// push r3 (1 instruction)
	CODE[45] = convDecimal("11110011");

	// ret (1 instruction)
	CODE[46] = convDecimal("00010000");


	

}

void cpu()
{
	while( !SHUTDOWN )
    {
      fetch();
      execute();
    }
}

void initArray()	  
{
	int i;
	for( i = 0; i <= 99; i++ ) {
		RAM[RAM_OFFSET + i] = i + 1;
	}
}



void main()
{
	START();

	RESET();
	INIT_SP();
	loadProgram();
	initArray();
	cpu();

	printf("Result value: %d \n", RAM[1]);
	printf("Please hit some key to close the program");
	getchar();
}

