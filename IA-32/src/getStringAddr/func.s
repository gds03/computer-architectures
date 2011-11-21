.intel_syntax noprefix
.text
.global _getStringAddr,getStringAddr

_getStringAddr:
getStringAddr:

	prologue:
	
		push ebp			/* Save caller stack base pointer   */
		mov ebp, esp		/* ebp now refers the callee 		*/
		
		push esi			/* Save caller esi register */			
		push edi			/* Save caller edi register */

		mov esi, [ebp+16]	/* ESI -> *Sample 		*/
		mov edi, [ebp+20]	/* EDI -> *var_arg[0]   */
		
		mov eax, 0			/* result = 0 */
		mov ecx, 0			/* ECX used for indexing var_arg */
		
	
	loop:
		
		loop_break_if:
			cmp edi, 0		/* if(str == NULL) */
			je end_loop
		
		loop_if:
			push eax			
			push ecx
			push esi
			push edi
			
			call _strstr		/* call C library function */
			cmp eax, 0
			
			pop edi
			pop esi
			pop ecx
			pop eax				/* restore result */
			je loop_advance_arg	/* strstr(str, sample) != NULL */
			
			loop_if_inside_1:
				push esi				/* Save sample on stack */
				
				mov esi, [ebp+8]		/* Esi now points to dst */
				mov [esi+4*eax], edi	/* dst[result] = var_arg[ecx] */
				inc eax
				
				pop esi					/* Restore sample from stack */
				
				loop_if_inside_2:
					cmp eax, [ebp+12]
					je end_loop				
		
	loop_advance_arg:
		inc ecx
		mov edi, [ebp+20+ecx*4]		/* Move edi to the next var_arg parameter */	
		jmp loop	
	
	end_loop: 
	epilogue:		
		pop edi
		pop esi
		
		pop ebp
		ret
	
