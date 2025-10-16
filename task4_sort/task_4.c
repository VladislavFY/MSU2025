#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Прототипы функций */
double *make_array(FILE*f, int *len); /* НЕ МЕНЯТЬ эту функцию — используется как есть */
double *generate_random_array(int len);
void insertion_sort(double *array, int len);
int is_sorted(const double *array, int len);
void time_sort_and_report(double *array, int len);


/* main: только открываем файл и вызываем функции */
int main(void){
    const char *filename = "input.txt";
    FILE *f = fopen(filename, "r");
    int len = 0;
    double *arr = NULL;

    if (f != NULL){
        /* читаем массив из файла (файл должен содержать числа double, пробел/перенос строки разделитель) */
        arr = make_array(f, &len); /* ЭТУ ФУНКЦИЮ НЕЛЬЗЯ МЕНЯТЬ — используем как есть */
        fclose(f);
        if (!arr){
            fprintf(stderr, "Ошибка выделения памяти при чтении файла.\n");
            return 1;
        }
    } else {
        /* Если файла нет — генерируем случайный массив указанной длины */
        printf("Файл \"%s\" не найден. Будет сгенерирован случайный массив длины 10000.\n", filename);
        len = 10000;
        arr = generate_random_array(len);
        if (!arr){
            fprintf(stderr, "Ошибка выделения памяти при генерации массива.\n");
            return 1;
        }
    }

    /* Вызов функции, которая сортирует и выводит время/проверку */
    time_sort_and_report(arr, len);

    free(arr);
    return 0;
}
