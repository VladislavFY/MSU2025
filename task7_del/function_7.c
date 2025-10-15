#include "prototypes.h"

double *make_array(FILE*f, int *len){
    int capacity = 2;
    double *array;
    double current;
    array = (double *)malloc(capacity*sizeof(double));
	while (fscanf(f, "%lf", &current) == 1)
	{
		++(*len);
		if (*len > capacity)
		{
			capacity *= 2;
			array = (double *)realloc(array, capacity*sizeof(double));
		}	
		array[*len - 1] = current;
	}
    return array;
}

int function_of_program(double *array, int len){
     int i;
     for (i=0;2*i<len;i++){
         if(fabs(array[i]-array[len-1-i])>eps) return -3;
    }
    return 0;
}
 
