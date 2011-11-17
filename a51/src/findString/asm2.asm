xseg at 0h
cseg at 0h

main:
	mov sp, #80h
	LCALL findString
	sjmp $
	
	
findString:
	mov r7, #-1	  					 ; return value
	mov 12, #'0'				   	 ; addr12 = '0' - terminator of strings
	mov 20, #-1						 ; addr20 = -1 (index)
	mov dptr, #SF				     ; dptr points to 'alo0'
	movc a, @a+dptr				     ; acumulator contains 'a'
	mov 8, a						 ; addr8 = 'a'
	mov dptr, #SO					 ; dptr points now to SO 'goncalo0'
	mov r1, #0						 ; i = 0
	
	for:  
		    mov a, r1			     ; a = i
	        movc a, @a+dptr			 ; a = 'g'
	        cjne a, 12, iff			 ; if a != '0' goto iff	  (this is loop test)
			returz:     ret			 ; we must return to the caller with index on a register
			
			iff:  
				  cjne a, 8, increment ; compare 'g' is different than 'a' so we must advance (increment)
				  lcall checkIfIs	 ; else possible match of SF string me must confirm

			increment: 
				  inc r1			; i++
				  sjmp for			; goto for
	
checkIfIs: 	; bad idea calculate SF length on each loop of checkIfIs method!! 
			; i know but when i developed this assembly i don't was concerned about it! :\
			
	mov r2, #0 						 ; count = 0
	mov a, r1						 ; a = current index of for loop
	mov r6, a				 		 ; backup of index 
	mov r0, a			 			 ; 
	mov r1, #0				 		 ; i = 0
	
	; calc length block
	; -----------------------------------------------------------------------
	CalcSFLength:					 ; dptr = SF
		   mov dptr, #SF			 ; r3 = 0
		   mov r3, #0
		
	loop2:
		   mov a, r3				 ; a = r3
		   movc a, @a+dptr			 ; a = 'a'
		   cjne a, 12 , increment2			 ; check if is not in the end of SF
		   mov 10, r3						 ; na posição 10 fica o length da SF
		   sjmp afterCalcSFLength	 ; if we are here, we have on addr10 the length of SF !!!!
	
	increment2: 	
		   inc r3						 	 ; increment variable
		   sjmp loop2						 ; goto loop2
	
	; -----------------------------------------------------------------------
	; end of calc length block

	afterCalcSFLength:	  
		   mov dptr, #SF				  ; dptr = &SF
		   mov a, r1					  ; a = 0
		   movc a, @a+dptr				  ; a = 'a'
		   mov 9, a						  ; addr9 = 'a'
		   
		   mov dptr, #SO				  ; dptr = &SO
		   mov a, r0					  ; a = 4 (r0 contains the 1st finded id)
		   movc a, @a+dptr				  ; a = 'a' - SO
		   mov r5, a					  ; r5 = 'a'
		   mov a, r2					  ; a = 0 (count)
		   
		   cjne a, 10, next_avaliation	  ; count != SF.length
		   mov a, r6					  ; we are at the end, and r6 is a backup of index (1st ocorrence) we save it to addr20
		   mov 20, a					  
		   sjmp return

	next_avaliation:
		   mov a, r5					   ; a = 'a'
		   cjne a, 9, return			   ; se SF.charAt(i) != SO.charAt(id) return, else procede
		  
		   inc r0							; id++
		   inc r1							; i++
		   inc r2							; count++
		   jmp afterCalcSFLength

	return:    ret

SO: db 'g','o','n','c','a','l','o','0';			  // '0' é o terminador da string
SF: db 'a','l','o','0';

end