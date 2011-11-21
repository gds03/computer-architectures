#include <stdio.h>

/*
	Physically, a function's stack frame is the area between the addresses
	contained in esp, the stack pointer, and ebp, the frame pointer 
	(base pointer in Intel terminology). Thus, if a function pushes more values onto the stack,
	it is effectively growing its frame.
*/

void* getStackInfo(int* stackFrameSize);

int main()
{
	int i = 0;
	/*
	int x = 4;
	int a = 4;
	int b = 4;
	int c = 4;
	int d = 4;
	int e = 0;
	*/
	int* stackSize = &i;
	void* memoryPtr;
	
	// printf("Printing local variable i = %d \n", i);
	memoryPtr = getStackInfo(stackSize);
	printf("Stack frame starts at 0x%x \n", memoryPtr);
	printf("Stack frame size is %d bytes \n", *stackSize);
	getchar();

	return 0;
}