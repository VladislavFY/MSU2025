#include "prototypes.h"

int main(void)
{
	int capacity = 2, len =0;
	double current;
	double *array;

	FILE *f;
    f = fopen( "data_42.txt", "r");	
	
	if (f == NULL)
	{
		printf("Can't open the file\n");
		return -2;
	}
    array = (double *)malloc(capacity*sizeof(double));
	while (fscanf(f, "%lf", &current) == 1)
	{
		++len;
		if (len > capacity)
		{
			capacity *= 2;
			array = (double *)realloc(array, capacity*sizeof(double));
		}	
		array[len - 1] = current;
	}
    printf("%d\n", function_of_program(array, len));
    
	free(array);
	return 0;
}
