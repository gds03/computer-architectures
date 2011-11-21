.intel_syntax noprefix
.global _getStackInfo,getStackInfo

_getStackInfo:
getStackInfo:

	mov eax, esp
	add eax, 8		/* eax discart IP and *stackSize */
	
	mov edx, ebp
	sub edx, eax
	
	mov ecx, esi
	mov esi, [esp+4]
	mov [esi], edx
	mov esi, ecx
	
	mov eax, [ebp+4]
	ret
	
	
	
		
		
	