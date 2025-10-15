#ifndef SUM_FUNC
#define SUM_FUNC

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#define eps 1.e-10

#define N 10.

int function_of_program(double *array, int len);
double *make_array(FILE *f, int *len);
double *modify_string( double *array, int len);
void print_string( double *array, int len);
void find_and_print(void);
void binary_print(int number);
#endif
