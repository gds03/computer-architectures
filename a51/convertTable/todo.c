char code tab[] = {‘!’, ‘@’, ‘{’, ‘%’, ‘&’, ‘*’, ‘]’, ‘:’}; 

void convertTable(char xdata array[], int size) 
{ 
	for (int i = 0; i < size; i = i + 1) 
		array[i] = tab[array[i]%8]; 
} 