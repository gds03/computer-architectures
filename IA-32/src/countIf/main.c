#include <stdio.h>


int comparator_for_integers(const char* a, const char* b) {
	return *a - *b;
}


int countIf(const void * data, int nelem, int size, 
	    const void * sample, 
	    int (*match)(const void *, const void *));

/* 		
			
int countIf(const void * data, int nelem, int size, 
			const void * sample, 
			int (*match)(const void *, const void *)
) {
	int result = 0;
	const void * iterator = data;
	const void * iterEndAddr = iterator + (nelem * size);
	
	for(	; iterator <= iterEndAddr; iterator += size)
	{
		int theyMatch = match(iterator, sample);
		if(theyMatch == 0)
			result++;	
	}
	
	return result;
}


*/

int main()
{
	const char data[] = { 'a', 'b', 'c', 'd', 'f', 'g', 'c' };
	const char* search = "c";
	
	// Declare func pointer
	int (*func) (const void*, const void*);
	
	func = &comparator_for_integers;	
	
	int result = countIf(data, 6, sizeof(char), search, func);
	printf("The result must be 2 and got %d \n", result);
	getchar();
	
	return 0;
}