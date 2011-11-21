#include <stdio.h>

typedef struct { 
	int res; 
	int average1; 
	int average2;
} results;

results sumProd(const int data1[], const int data2[], int nelem);

/*
results sumProd(const int data1[], const int data2[], int nelem)
{
	results r;
	int i = 0;
	
	r.res = r.average1 = r.average2 = 0;
	
	for(	; i < nelem; i++)
	{
		r.res = r.res + (data1[i] * data2[i]);
		r.average1 = r.average1 + data1[i];
		r.average2 = r.average2 + data2[i];	
	}
	
	// adjust averages
	r.average1 = r.average1 / nelem;
	r.average2 = r.average2 / nelem;
	
	return r;
}

*/


int main()
{
	const int data1[] = { 1,2,3,4,5,6,7,8 };
	const int data2[] = { 7,8,9,10,11,12,13,14};

	results r = sumProd(data1, data2, 8);
	
	printf("Showing the results.. \n");
	printf("res = %d \n", r.res);
	printf("average1 = %d \n", r.average1);
	printf("average2 = %d \n", r.average2);
	
	getchar();
	return 0;
}