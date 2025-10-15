/*
 * insertion_double_simple.c
 *
 * Упрощённая учебная программа: сортировка вставками (для double).
 * - main(void) — без парсинга argv/argc, простой интерактивный ввод;
 * - make_array(FILE *f, int *len) — функция чтения из файла (использован ваш блок как основа);
 * - generate_random_array(int n, unsigned int seed) — генерация массива;
 * - insertion_sort(double *arr, int len) — сортировка;
 * - is_sorted(const double *arr, int len) — проверка;
 * - измерение времени через C11 timespec_get.
 *
 * Примечание: для простоты используем int для длины массива (len). В
 * реальных программах, где возможны очень большие массивы, лучше использовать size_t.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

/* --- Ваша функция чтения (с добавлением проверок на NULL) ---
 * Возвращает указатель на массив double; записывает количество
 * прочитанных элементов в *len. Если файл пуст — возвращает NULL и *len==0.
 *
 * Почему int *len, а не double **out?
 * - Функция возвращает указатель на массив как результат (return),
 *   поэтому дополнительный двойной указатель не нужен. В исходном
 *   варианте (когда функция должна была присвоить указатель в переменную
 *   вызывающего без возврата) использовался бы double **out. Здесь проще:
 *   возвращаем новый указатель и через *len говорим, сколько элементов.
 */
double *make_array(FILE *f, int *len) {
    int capacity = 2;
    double *array;
    double current;

    *len = 0; /* начинаем с нуля прочитанных элементов */

    /* выделяем начальную память. (Каст в C не обязателен, но оставлен по вашему примеру.) */
    array = (double *)malloc(capacity * sizeof(double));
    if (array == NULL) {
        /* Ошибка выделения памяти; сообщаем об этом вызывающему (возврат NULL) */
        return NULL;
    }

    /* Считываем по одному double из файла; fscanf возвращает 1 при успехе */
    while (fscanf(f, "%lf", &current) == 1) {
        ++(*len); /* увеличили количество прочитанных элементов */

        /* Если число элементов превысило текущую ёмкость, расширяем */
        if (*len > capacity) {
            capacity *= 2;
            double *tmp = (double *)realloc(array, capacity * sizeof(double));
            if (tmp == NULL) {
                /* realloc провалился — освобождаем старый буфер и возвращаем NULL */
                free(array);
                *len = 0;
                return NULL;
            }
            array = tmp;
        }
        /* индекс для нового элемента = *len - 1 (т.к. len уже увеличен) */
        array[*len - 1] = current;
    }

    /* Если была ошибка ввода-вывода (не EOF), возвращаем NULL */
    if (ferror(f)) {
        free(array);
        *len = 0;
        return NULL;
    }

    /* Если ничего не прочли — освободим и вернём NULL, len==0 */
    if (*len == 0) {
        free(array);
        return NULL;
    }

    /* Можно сжать память: realloc на точный размер. Это опционально. */
    {
        double *shrink = (double *)realloc(array, (*len) * sizeof(double));
        if (shrink != NULL) array = shrink;
        /* если shrink == NULL — оставляем старый buffer (немного больше памяти),
           это не ошибка — важно не терять данные */
    }

    return array;
}

/* --- Генерация случайного массива (возвращает указатель; len задаётся параметром) ---
 * Простая реализация: rand(), масштабирование; seed позволяет детерминировать.
 * Возвращает NULL при ошибке выделения памяти.
 */
double *generate_random_array(int n, unsigned int seed) {
    if (n <= 0) return NULL;

    double *arr = (double *)malloc(n * sizeof(double));
    if (arr == NULL) return NULL;

    srand(seed);

    for (int i = 0; i < n; ++i) {
        /* Делаем число в диапазоне [-100000, 100000) с дробной частью */
        double frac = (double)rand() / ((double)RAND_MAX + 1.0);
        double whole = (double)rand();
        double value = (frac + whole / ((double)RAND_MAX + 1.0)) * 200000.0 - 100000.0;
        arr[i] = value;
    }
    return arr;
}

/* --- insertion_sort для double (линейный поиск места вставки) --- */
void insertion_sort(double *arr, int len) {
    for (int i = 1; i < len; ++i) {
        double key = arr[i]; /* элемент, который вставляем в отсортированную часть */
        int j = i;
        /* сдвигаем вправо все элементы, большие key */
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

/* --- Проверка, что массив упорядочен неубывательно (a[i-1] <= a[i]) --- */
int is_sorted(const double *arr, int len) {
    for (int i = 1; i < len; ++i) {
        if (arr[i - 1] > arr[i]) return 0;
    }
    return 1;
}

/* --- Время в секундах (double) с дробной частью через timespec_get (C11) --- */
double now_seconds(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + ts.tv_nsec / 1e9;
}

/* --- Печать небольшого превью массива (первые и/или последние элементы) --- */
void print_preview(const double *arr, int len) {
    if (len <= 0) {
        printf("(пустой массив)\n");
        return;
    }
    int show = (len < 10) ? len : 5;
    printf("Первые %d элементов: ", show);
    for (int i = 0; i < show; ++i) {
        printf("%g ", arr[i]);
    }
    if (len > show) printf("... ");
    printf("\n");
}

/* --- Упрощённый интерактивный main (void) --- */
int main(void) {
    printf("Программа: сортировка вставками (double)\n");
    printf("Выберите режим:\n");
    printf("  1 - считать числа из файла\n");
    printf("  2 - сгенерировать случайный массив\n");
    printf("Ввод (1 или 2): ");

    int mode = 0;
    if (scanf("%d", &mode) != 1) {
        fprintf(stderr, "Неправильный ввод.\n");
        return 1;
    }

    double *array = NULL;
    int len = 0; /* количество элементов — используем int для простоты */

    if (mode == 1) {
        /* чтение из файла: спрашиваем имя файла */
        char filename[256];
        printf("Введите имя файла: ");
        /* съедаем оставшийся символ новой строки перед fgets/scanf */
        if (scanf("%255s", filename) != 1) {
            fprintf(stderr, "Ошибка при вводе имени файла.\n");
            return 1;
        }

        FILE *f = fopen(filename, "r");
        if (!f) {
            fprintf(stderr, "Не удалось открыть файл '%s': %s\n", filename, strerror(errno));
            return 1;
        }

        /* используем вашу функцию make_array — она сама установит len */
        array = make_array(f, &len);
        fclose(f);

        if (array == NULL && len == 0) {
            fprintf(stderr, "Файл пуст или произошла ошибка чтения/памяти.\n");
            return 1;
        }

    } else if (mode == 2) {
        int n = 0;
        unsigned int seed = (unsigned int)time(NULL);
        printf("Сколько чисел сгенерировать? ");
        if (scanf("%d", &n) != 1 || n <= 0) {
            fprintf(stderr, "Неверное количество.\n");
            return 1;
        }
        printf("Введите seed (целое) или 0 чтобы использовать текущее время: ");
        int seed_in = 0;
        if (scanf("%d", &seed_in) == 1 && seed_in != 0) seed = (unsigned int)seed_in;

        array = generate_random_array(n, seed);
        if (array == NULL) {
            fprintf(stderr, "Ошибка выделения памяти при генерации.\n");
            return 1;
        }
        len = n;

    } else {
        fprintf(stderr, "Неверный режим.\n");
        return 1;
    }

    printf("Прочитано/сгенерировано элементов: %d\n", len);
    print_preview(array, len);

    /* измеряем время сортировки */
    double t0 = now_seconds();
    insertion_sort(array, len);
    double t1 = now_seconds();

    printf("Сортировка заняла: %.9f секунд\n", t1 - t0);

    /* проверка и вывод небольшого превью результата */
    if (is_sorted(array, len)) {
        printf("Проверка: массив отсортирован (неубывание).\n");
    } else {
        printf("Проверка: массив НЕ отсортирован!\n");
    }

    print_preview(array, len);

    /* освобождаем память */
    free(array);

    return 0;
}

 
