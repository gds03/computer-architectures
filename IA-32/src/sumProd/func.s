.intel_syntax noprefix
.global _sumProd,sumProd

_sumProd:
sumProd:

	prologue:
		push ebp			/* Save olded ebp register */
		mov ebp, esp		/* Set current frame on ebp */
		
		push ebx			
		push edi
		push esi			
		
		mov ebx, [ebp+8] 	/* ebx now refers the structures */
		mov esi, [ebp+12]	/* esi -> data1 */
		mov edi, [ebp+16]	/* edi -> data2 */
		
		mov dword ptr[ebx], 0
		mov dword ptr[ebx + 4], 0
		mov dword ptr[ebx + 8], 0
		
		mov ecx, 0			/* i = 0 */
		
		for_loop:
			for_loop_condition:
				cmp ecx, [ebp+20]
				je adjust_averages
			
			for_loop_body:
				/* r.res = r.res + (data1[i] * data2[i]); */
				
				mov edx, [esi+4*ecx]
				mov eax, [edi+4*ecx]
				mul edx
				add [ebx], eax
				
				/* r.average1 = r.average1 + data1[i]; */
				mov edx, [esi+4*ecx]
				add [ebx+4], edx
				
				/* r.average2 = r.average2 + data2[i]; */
				mov edx, [edi+4*ecx]
				add [ebx+8], edx
							
			for_loop_increment:
				inc ecx
				jmp for_loop
				
		
		adjust_averages:
			mov edx, 0						/* Preparing edx for division */
			mov eax, [ebx+4]
			div dword ptr[ebp+20]
			mov [ebx+4], eax
			
			mov edx, 0
			mov eax, [ebx+8]
			div dword ptr[ebp+20]
			mov [ebx+8], eax

	epilogue:
		pop esi
		pop edi
		pop ebx
		pop ebp
		ret
	