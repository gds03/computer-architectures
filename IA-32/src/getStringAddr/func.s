.intel_syntax noprefix
.text
.global _getStringAddr,getStringAddr

_getStringAddr:
getStringAddr:

	prologue:
	
		push ebp			/* Save caller stack base pointer   */
		mov ebp, esp		/* ebp now refers the callee 		*/
		
		push esi			
		push edi

		mov esi, [ebp+16]	/* Esi points to *sample */
		mov edi, [ebp+20]	/* First var str parameter */
		
		mov eax, 0			/* result = 0 */
		
	
	loop:
		
		loop_break_if:
			cmp byte ptr[edi], 0
			je end_loop
		
		loop_if:
			push eax			/* save current result */
			push [esi]
			push [edi]
			
			call strstr
			
			add esp, 8			/* Discart parameters used by strstr (__cdecl)
			cmp eax, 0
			pop eax				/* restore result */
			je loop_advance_arg
			
			loop_if_inside:
				push esi
				mov esi, [ebp+8+4*eax]
				mov [esi], edi
				pop esi
				inc eax		
		
	loop_advance_arg:
		add edi, 4		
		jmp loop	
	
	end_loop: 
	epilogue:		
		pop edi
		pop esi
		
		pop ebp
		ret
	
