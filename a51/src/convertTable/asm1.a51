;
; array[all_positions] tem que ficar com todos os caracteres que estão em tab
; 



dseg at 08h						  ; segmento de dados a partir de 08h por causa dos registos até R7
	i: ds 2						  ; i tem 2 bytes(parte alta + parte baixa) devido a estar em xdata (16bits)
								  
xseg at 0h						  ; segmento de memoria externa a partir de 0h

cseg at 0h						  ; segmento de codigo a partir de 0h
 
MAIN: 							  ; IN XDATA is where array will be

	MOV A, #39					  ; A = 39
	MOVX @DPTR, A				  ; XDATA[0] = 39 DPTR is a 2byte register that points to external memory XSEG?
	inc dptr
	mov a, #60					  ; A = 60
	movx @dptr, a				  ; XDATA[1] = 60
	inc dptr
	mov a, #48					  ; A = 48
	movx @dptr, a				  ; XDATA[2] = 48
	inc dptr
	MOV SP, #80h				  ; SP = 80h
	MOV R4, #0; 				  ; 
	MOV R5, #3;				 	  ; int size = 3
	MOV R6, #0;					  ;
	MOV R7, #0;					  ; char xdata array[]
	LCALL convertTable			  ; metodo convertTable
	SJMP $

convertTable:
								  
MOV DPH, R6						  ; dph = contem a posição onde se encontra XSEG(parte alta)
MOV DPL, R7					 	  ; dpl = contem a posição onde se encontra XSEG(parte baixa)

FOR: 							  ; Compara se o i < size

	 CLR C						  ; Cy = 0
	 MOV A, i+1					  ; A = i+1 (parte baixa)
	 SUBB A, R5					  ; A = (i+1) - R5 				- i < size
	 MOV A, i					  ; A = i (parte alta)
	 SUBB A, R4					  ; A = i - R4
	 JC	DO
	 RET						  ; retorna


DO:	 							  ; primeira instrução do ciclo FOR
	 MOVX A,@DPTR				  ; A = 39	; aponsta para array[0]
	 MOV R0, DPL				  ;
	 MOV R1, DPH				  ; R0 e R1 guardam o valor do Stack actual (de XDATA)
	 MOV B, #8					  ; B = 8
	 DIV AB						  ; A = 4(divisor), B = 6(rest)
	 MOV DPTR, #TAB				  ; dptr aponta para Tabela (codigo)
	 MOV A, B					  ; A = 6
	 MOVC A, @A+DPTR			  ; A = TAB[6]
 	 MOV DPL, R0				  ;
	 MOV DPH, R1				  ; dptr = ultima posição de XDATA, neste caso 0
	 MOVX @DPTR, A				  ; e guarda-se em XDATA[0] o que resulta em A
	 INC DPTR					  ; dptr++
	 CLR C						  ; Cy = 0
	 SJMP INCREMENTO			  ; i < size = i - size < 0; portanto se ouver borrow é porque i ainda é menor que size

INCREMENTO:
	 MOV A, i+1					  ; A = i+1 (parte baixa)
	 ADD A, #1					  ; A++
	 MOV i+1, A					  ; i+1 = A
	 MOV A, i					  ; A = i	(parte alta)
	 ADDC A, #0					  ; A
	 MOV i, A					  ; i = A	  (afectado com borrow caso haja)
	 SJMP FOR

tab: db '!', '@', '{', '%', '&', '*', ']', ':';

END
