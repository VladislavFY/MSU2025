#include "prototypes.h"

int main(void)
{
	int len =0;
    double *array;
	FILE *f;
    f = fopen( "data_6.txt", "r");	
	
	if (f == NULL)
	{
		printf("Can't open the file\n");
		return -2;
	}
    array = make_array(f, &len);
	array = modify_string(array, len);
    print_string(array, len);
	free(array);
	return 0;
} 
