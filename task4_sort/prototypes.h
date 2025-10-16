#ifndef SUM_FUNC
#define SUM_FUNC

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define eps 1.e-10

#define N 10.

double *make_array(FILE*f, int *len); /* НЕ МЕНЯТЬ эту функцию — используется как есть */
double *generate_random_array(int len);
void insertion_sort(double *array, int len);
int is_sorted(const double *array, int len);
void time_sort_and_report(double *array, int len);


#endif
