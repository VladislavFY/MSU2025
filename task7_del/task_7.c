#include "prototypes.h"

int main(void)
{
	int len =0;
    int result;
    double *array;
	FILE *f;
    f = fopen( "data_2.txt", "r");	
	
	if (f == NULL)
	{
		printf("Can't open the file\n");
		return -2;
	}
    array = make_array(f, &len);
	result = function_of_program(array, len);
	if(result==0){
         printf("\nYES\n");
    }
    else if(result!=0 )printf("\nNO\n");
	free(array);
	return 0;
} 
