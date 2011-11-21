.intel_syntax noprefix
.global _getStackInfo,getStackInfo

_getStackInfo:
getStackInfo:

	mov eax, esp
	add eax, 8			/* eax discart IP and *stackSize */
	
	mov edx, ebp
	sub edx, eax
	
	mov ecx, [esp+4]
	mov [ecx], edx		/* 		*stackSize = ... 		*/
	
	mov eax, [ebp+4]	
	ret
	
	
	
		
		
	