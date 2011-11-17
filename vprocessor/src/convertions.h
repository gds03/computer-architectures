#include <windows.h>

byte potential(__in byte index) { return 1 << index; }

byte 
convDecimal(__in const char* ptr)
{
	byte result = 0;
	int length = 0;

	const char* origin = ptr;
	byte index = 0;
	
	// Collect length
	for( ; *ptr != '\0'; length++, ptr++);
	
	// Calcs
	for(length-- ; length >= 0; length--, index++) {
		if( (origin[length] - '0') == 1) {
			result += potential(index);
		}
	}

	return result;
}