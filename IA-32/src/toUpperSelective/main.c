#include <stdio.h>

int toUpperSelective(char* str, const char* selectChars);

int main() 
{
	char str[] = "remembering toUpperSelective";
	const char* findChars = "re";
	
	printf("Source String: %s \n", str);
	printf("Characters to capitalize: %s \n", findChars);
	
	int result = toUpperSelective(str, findChars);
	
	printf("Final String: %s \n", str);
	printf("Total characters capitalized: %d \n", result);
	
	getchar();
	return 0;
}

/*
int toUpperSelective(char* str, const char* selectChars)
{
	int subtitutedCharsCount = 0;
	char* strOrigin = str;
	
	for(	; *selectChars != '\0'; selectChars++) {
		for(	; *str != '\0'; str++)  {
			if(*str == *selectChars) {
				*str = *str - ('a'-'A');
				subtitutedCharsCount++;
			}		
		}
		str = strOrigin;
	}
	return subtitutedCharsCount;
}
*/