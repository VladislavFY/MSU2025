#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Прототипы функций */
double *make_array(FILE*f, int *len); /* НЕ МЕНЯТЬ эту функцию — используется как есть */
double *generate_random_array(int len);
void insertion_sort(double *array, int len);
int is_sorted(const double *array, int len);
void time_sort_and_report(double *array, int len);

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

/* Генерация случайного массива double длины len */
double *generate_random_array(int len){
    if (len <= 0) return NULL;
    double *a = (double *)malloc(sizeof(double) * len);
    if (!a) return NULL;
    /* инициализация генератора случайных чисел */
    srand((unsigned)time(NULL));
    for (int i = 0; i < len; ++i){
        /* генерируем числа в диапазоне [0, 10000) с дробной частью */
        a[i] = (double)rand() / (double)RAND_MAX * 10000.0;
    }
    return a;
}

/* Вставка с последовательным поиском (обычная insertion sort) */
void insertion_sort(double *array, int len){
    for (int i = 1; i < len; ++i){
        double key = array[i];
        int j = i - 1;
        /* последовательный (линейный) поиск позиции: сдвигаем элементы вправо */
        while (j >= 0 && array[j] > key){
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = key;
    }
}

/* Проверка, упорядочен ли массив по неубыванию */
int is_sorted(const double *array, int len){
    for (int i = 1; i < len; ++i){
        if (array[i-1] > array[i]) return 0; /* не упорядочен */
    }
    return 1; /* упорядочен */
}

/* Измеряем время сортировки, вызываем сортировку, выводим результат */
void time_sort_and_report(double *array, int len){
    if (!array || len <= 0){
        printf("Пустой массив или некорректная длина (%d).\n", len);
        return;
    }

    /* Клонируем массив, если хотим оставить оригинал — тут сортируем на месте */
    /* Замер времени (CPU time) через clock() */
    clock_t start = clock();
    insertion_sort(array, len);
    clock_t end = clock();

    double elapsed_seconds = (double)(end - start) / (double)CLOCKS_PER_SEC;

    printf("Длина массива: %d\n", len);
    printf("Время, затраченное на сортировку (сек): %.6f\n", elapsed_seconds);
    printf("Массив упорядочен? %s\n", is_sorted(array, len) ? "Да" : "Нет");

    /* Покажем первые и последние элементы для быстрой проверки (если массив большой) */
    int show = 10;
    if (len <= show){
        printf("Отсортированный массив:\n");
        for (int i = 0; i < len; ++i) printf("%.6f ", array[i]);
        printf("\n");
    } else {
        printf("Первые %d элементов: ", show);
        for (int i = 0; i < show; ++i) printf("%.6f ", array[i]);
        printf("\n");
        printf("Последние %d элементов: ", show);
        for (int i = len - show; i < len; ++i) printf("%.6f ", array[i]);
        printf("\n");
    }
}

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
