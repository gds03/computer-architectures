.intel_syntax noprefix
.global countIf,_countIf

_countIf:
countIf:
	prologue:
		push ebp
		mov ebp, esp
		
		push edi			
		push esi
		
		
		
		mov esi, [ebp + 8]  /* esi -> iter -> data */
		mov edi, [ebp + 20] /* edi -> sample */
		
		mov ecx, esi
		mov eax, [ebp + 12]
		mov edx, [ebp + 16]
		mul edx				/* edx = nelem * size */
		
		mov ecx, esi
		add ecx, eax		/* ecx now is the iterEndAddr */
		
		mov eax, 0			/* result = 0 */
		
		for_loop:
			for_loop_condition:
				cmp esi, ecx
				jg end_loop
				
			for_loop_body:
				push eax
				push ecx
				push edi
				push esi
				
				call [ebp + 24]		/* call callback function */
				cmp eax, 0
				
				pop esi
				pop edi
				pop ecx
				pop eax
				
				jne for_loop_increment
				
				inc eax
				
			for_loop_increment:
				add esi, [ebp + 16]
				jmp for_loop
		
		end_loop:
		epilogue:
			pop esi
			pop edi
			pop ebp
			ret
		