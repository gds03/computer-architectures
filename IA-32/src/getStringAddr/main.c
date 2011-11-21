#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// int getStringAddr(char *dst[], int size, const char *sample, ...);

/*
int getStringAddr(char *dst[], int size, const char *sample, ...)
{
	int dstIdx = 0;
	int result = 0;
	va_list arguments;
	char* str;
	
	
	va_start(arguments, sample);
	do
	{
		str = va_arg(arguments, char*);
		if(str == NULL)
			break;
		
		if( strstr(str, sample) != NULL ) {
			dst[dstIdx++] = str;
			result++;
		}
	}
	while(1);
	
	va_end(arguments);
	return result;

}

*/

int main()
{
	int dst_size = 10;
	int result;
	int i = 0;
	
	char* dst[dst_size];
	const char *sample = "github";
	
	const char* s1 = "i like github";
	const char* s2 = "i think git is nice!";
	const char* s3 = "i am making this exercice for sport!";
	const char* s4 = "just testing a function";
	const char* s5 = "i like github command line";
	const char* s6 = NULL;
	
	result = getStringAddr(dst, dst_size, sample, s1, s2, s3, s4, s5, s6);
	
	printf("The result must be 2 and got %d \n", result);
	printf("Showing the dst content \n");
	
	for(	; i < result; i++) {
		printf("%s \n", dst[i]);
	}
	

	return 0;
}


