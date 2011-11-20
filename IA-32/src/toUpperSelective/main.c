#include <stdio.h>

int toUpperSelective(char* str, const char* selectChars);

int main() 
{
	char str[] = "remembering toUpperSelective exercise..";
	const char* findChars = "re";
	
	int result = toUpperSelective(str, findChars);
	printf("Result Expected is %d and got %d \n", 14, result);
	printf("Printing result: %s \n", str);
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