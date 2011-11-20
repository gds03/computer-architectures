.intel_syntax noprefix
.text
.global _toUpperSelective,toUpperSelective

_toUpperSelective:
toUpperSelective:
	
							
							
	push ebp				/* save old stack frame pointer */
	mov ebp, esp			/* use ebp as the new frame pointer */
	
							/* ---------- RESERVING LOCALS (8 bytes) -------- */
							
	sub esp, 8				/* 	----------------------------------  */			
							/* [ebp-4] subtitutedCharsCount	  */
							/* [ebp-8] strOrigin */
							
							/* ------------------------------------  */
							
	push esi				/* ------------- Registers used  ----------- */
	push edi		
	push ecx				/* ----------------------------------------- */
	
	mov dword ptr[ebp-4], 0			/* int subtitutedCharsCount = 0		*/
	mov dword ptr[ebp-8], edi		/* char* strOrigin = str			*/
	mov esi, [ebp + 12]				/* esi -> selectChars (source)		*/
	mov edi, [ebp + 8]				/* edi -> str 		  (dest)		*/
	
	for_loop1:		
		for_loop1_condition:
			cmp byte ptr [esi], 0
			je prepare_end
		
		for_loop1_body:			
			for_loop2:			
				for_loop2_condition:
					cmp byte ptr [edi], 0
					je set
				
				for_loop2_body:
					for_loop2_body_if:
						mov cl, [edi]
						cmp [esi], cl
						jne for_loop2_increment
						
						sub byte ptr[edi], 'a'-'A'
						inc dword ptr[ebp-4]						
					
				for_loop2_increment:
					inc edi
					jmp for_loop2	
			
			set:			
				mov edi, [ebp-8]
			
		for_loop1_increment:
			inc esi
			jmp for_loop1
	
	prepare_end:
		mov eax, [ebp-4]
		
		pop ecx
		pop edi
		pop esi
		add esp, 8					/* Clear local variables */
		pop ebp						/* Restore caller stack frame pointer */
		ret
		