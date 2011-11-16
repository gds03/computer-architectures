/*
   Instituto Superior de Engenharia de Lisboa
   Arquitectura de Computadores - 2007
*/

#include <stdio.h>

#define DIM_RAM 256
#define DIM_CODE 2048
#define byte unsigned char
#define word unsigned int

#define RAM_OFFSET 100


/* 
	Opcode
*/

#define instr1 0x00		// 0000
#define instr2 0x04		// 0100
#define instr3 0x02		// 0010
#define instr4 0x03		// 0011
#define instr5 0x0B		// 1011
#define instr6 0x09		// 1001
#define instr7 0x0A		// 1010
#define instr8 0x08		// 1000
#define instr9 0x0F		// 1111
#define instr10 0x0D	// 1101
#define instr11 0x0E	// 1110
#define instr12 0x0C	// 1100
#define instr13 0x01	// 0001

/* 
	Registers
*/

byte A;					// Acumulator
byte SP;				// Stack Pointer 			
byte MAR;				// Memory *** Address *** Register  
byte MBR;				// Memory *** Buffer *** Register   - stores the data being transferred to and from the immediate access store
byte IR;				// Instruction Register		
byte TMP;				// Temp Register

byte RAM[DIM_RAM];		// RAM
byte REG[4];			// Registers [0..3]
byte CODE[DIM_CODE];	// CODE
word PC;				// Program Counter 			

/* 
	Operators From EFI
*/ 

/* PC -  holds the address of nxt instruction to be executed. */
void RESET() { PC = 0; }
void INC_PC() { PC = (PC + 1) & 0x7FF; }				// & 0111 1111 1111
void RDC() { MAR = CODE[PC]; }							// Read Code -> MAR = Code[PC]
void LD_MBR_PC0_7() { MBR = PC & 0xFF; }				// Carrega parte baixa (2 bytes do PC) para MBR
void LD_MBR_PC8_10() { MBR = ( PC & 0x700 ) >> 8; }		// Carrega parte alta (3 bits do PC) para MBR

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

/* ALU */

void ALU()
{
    switch ((IR >> 4) & 0x0F)
	{
      case instr5 :
        MBR = MBR + A;
        break;
      case instr6 :
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
  	case instr1:	   					// MOV addr, Rn
		LD_A_MAR();
		LD_MAR_IR0_1();
		RDR();
		LD_MAR_A();
		WDD();
		break;	 // .. a funcionar

	case instr2:					   // MOV Rn, addr
		RDD();				// MBR = RAM[MAR]
		LD_MAR_IR0_1();		// MAR = Rn
		WDR();				// REG[Rn] = MBR
		break;	 // .. a funcionar

	case instr3:					  // MOV Rn, #const8
		LD_MBR_MAR();
		LD_MAR_IR0_1();
		WDR();
		break;	 // .. a funcionar

	case instr4:					  // MOV Rn, @Rm
		LD_MAR_IR3_2();		// MAR = Rm
		RDR();				// MBR = REG[MAR]
		LD_MAR_MBR();		// MAR = MBR
		RDD();				// MBR = RAM[MAR]
		LD_MAR_IR0_1();		// MAR = Rn
		WDR();				// Rn = MBR (RAM[MAR])
		break;	// .. a funcionar

	case instr5:					  // ADD Rn, Rm
	case instr6:					  // SUB  Rn, Rm
		LD_MAR_IR3_2();
		RDR();
		LD_A_MBR();
		LD_MAR_IR0_1();
		RDR();
		ALU();
		WDR();
		break;  // .. a funcionar

	case instr7:					  // JNZ Rn, Offset8
		LD_A_MAR();
		LD_MAR_IR0_1();
		RDR();
		LD_MAR_A();
		LD_A_MBR();

		if ( FLAG_ZERO() )
			ADD();
		break;	// .. a funcionar
	case instr8:					  // DJNZ Rn, Offset8
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
	case instr9:					 // PUSH Rn
		LD_MAR_IR0_1();
		RDR();
		LD_MAR_SP();
		WDD();
		INC_SP();
		break;   // .. a funcionar
	case instr10:					 // POP Rn
		DEC_SP();
		LD_MAR_SP();
		RDD();
		LD_MAR_IR0_1();
		WDR();
		break;	// .. a funcionar
	case instr11:					 // SJMP Offset8
		ADD();
		break;	// .. a funcionar
	case instr12:					 // LCALL c_addr
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
	case instr13:
		DEC_SP();					 // RET
		LD_MAR_SP();
		RDD();
		LD_TMP_MBR();
		DEC_SP();
		DEC_MAR();
		RDD();
		LD_PC_SP_ALL();

		// Comment this line, is for debug purposes..
		printf("Current value: %d \n", RAM[0]);

		break;	// .. a funcionar
	}
}

/* Program */

void loadProgram()	
{	
	// Instructions codification

	CODE[0] = 0X42;		//					0100 0010 (instruction - opcode) (register 2)
	CODE[1] = 0X02;		// MOV R2, ADDR2	0000 0010 (address 2)
	
	CODE[2] = 0X23;		//
	CODE[3] = 0X64;		// MOV R3, #100
	
	CODE[4] = 0X21;		//
	CODE[5] = 0X00;		// MOV R1, #0
	
	CODE[6] = 0X38;     // MOV R0, @R2 -> LABEL FOR
	
	CODE[7] = 0X00;		//
	CODE[8] = 0X03;     // MOV ADDR3, R0
	
	CODE[9] = 0X94;		// SUB R0, R1
	
	CODE[10] = 0XA0;	//
	CODE[11] = 0X00;	// JNZ R0, SOMALAB
	
	CODE[12] = 0XC0;	// 			  -> LABEL SOMALAB
	CODE[13] = 0X1C;	// LCALL SOMA
	
	CODE[14] = 0X22;	//
	CODE[15] = 0X01;	// MOV R2, #1
	
	CODE[16] = 0XB9;	// ADD R1, R2
	
	CODE[17] = 0X20;	//
	CODE[18] = 0X01;	// MOV R0, #1	 
	
	CODE[19] = 0X42;	//
	CODE[20] = 0X02;	// MOV R2, ADDR2
	
	CODE[21] = 0XB2;	// ADD R2, R0     
	
	CODE[22] = 0X02;	//
	CODE[23] = 0X02;	// MOV ADDR2, R2
	
	CODE[24] = 0X83;	//
	CODE[25] = 0XEC;	// DJNZ R3, FOR	   // verificar..
	
	CODE[26] = 0XE0;	//
	CODE[27] = 0XFE;	// SJMP $

	// If we get here we must to stop..		 /!\
	
	CODE[28] = 0X40;	//				-> LABEL SOMA
	CODE[29] = 0X00;	// MOV R0, ADDR0
	
	CODE[30] = 0X42;	//
	CODE[31] = 0X03;	// MOV R2, ADDR3
	
	CODE[32] = 0XB8;	// ADD R0, R2
	
	CODE[33] = 0X00;	//
	CODE[34] = 0X01;	// MOV ADDR1, R0
	
	CODE[35] = 0X42;	//
	CODE[36] = 0X00;	// MOV R2, ADDR0
	
	CODE[37] = 0XB2; 	// ADD R2, R0
	
	CODE[38] = 0X02;	//
	CODE[39] = 0X00;	// MOV ADDR0, R2
	
	CODE[40] = 0X10;	// RET
}

void cpu()
{
   for ( ; ; )
   {
     fetch();
     execute();
   }
}

void initArray()	  
{
	int i;
	for( i = 0; i <= 99; i++ ) {
		RAM[RAM_OFFSET + i] = i;
	}
}

void main()
{
	RESET();
	INIT_SP();
	loadProgram();
	initArray();
	cpu();


	printf("Please hit some key to close the program");
	getchar();
}

