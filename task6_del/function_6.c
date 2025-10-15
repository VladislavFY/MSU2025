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

double *modify_string( double *array, int len){ 

    int write = 0;
    /* 1) Найти минимальный элемент */
    double min = array[0];
    for (int i = 1; i < len; ++i) {
        if (array[i] < min) min = array[i];
    }

    /* 2) Уплотнить — записывать те элементы, которые не равны min (в пределах eps) */
    for (int read = 0; read < len; ++read) {
        if (fabs(array[read] - min) > eps) {
            array[write++] = array[read];
        }
    }

    /* 3) Замазать остаток нулями */
    for (int k = write; k < len; ++k) {
        array[k] = 0;
    }

    return array;
}

void print_string( double *array, int len){
       for(int i=0; i<len; i++) printf( "\narray[%d] = %lf\n", i, array[i]);
}
