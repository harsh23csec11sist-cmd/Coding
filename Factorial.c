#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

unsigned long long factorial_iterative(int n) {
    if (n < 0) return 0;
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        if (result > ULLONG_MAX / i) return 0;
        result *= i;
    }
    return result;
}

unsigned long long factorial_recursive(int n) {
    if (n < 0) return 0;
    if (n == 0 || n == 1) return 1;
    unsigned long long prev = factorial_recursive(n - 1);
    if (prev == 0 || prev > ULLONG_MAX / n) return 0;
    return n * prev;
}

int main() {
    int choice, n;
    unsigned long long result;
    clock_t start, end;
    double cpu_time_used;

    while (1) {
        printf("\n=== Advanced Factorial Calculator ===\n");
        printf("1. Compute factorial (iterative)\n");
        printf("2. Compute factorial (recursive)\n");
        printf("3. Compare both methods\n");
        printf("0. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }

        if (choice == 0) break;
        if (choice < 1 || choice > 3) {
            printf("Invalid choice.\n");
            continue;
        }

        printf("Enter a non-negative integer: ");
        if (scanf("%d", &n) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        if (n < 0) {
            printf("Factorial not defined for negative numbers.\n");
            continue;
        }

        if (choice == 1) {
            start = clock();
            result = factorial_iterative(n);
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
            if (result == 0)
                printf("Overflow detected (result too large).\n");
            else
                printf("%d! = %llu (iterative, time: %.3f ms)\n", n, result, cpu_time_used);
        }
        else if (choice == 2) {
            start = clock();
            result = factorial_recursive(n);
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
            if (result == 0)
                printf("Overflow detected (result too large).\n");
            else
                printf("%d! = %llu (recursive, time: %.3f ms)\n", n, result, cpu_time_used);
        }
        else {
            start = clock();
            unsigned long long res_i = factorial_iterative(n);
            end = clock();
            double time_i = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

            start = clock();
            unsigned long long res_r = factorial_recursive(n);
            end = clock();
            double time_r = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

            if (res_i == 0 || res_r == 0)
                printf("Overflow detected for n = %d.\n", n);
            else if (res_i != res_r)
                printf("Mismatch! Something went wrong.\n");
            else
                printf("%d! = %llu\nIterative: %.3f ms, Recursive: %.3f ms\n", n, res_i, time_i, time_r);
        }
    }
    return 0;
}