#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

unsigned long long fib_iterative_series(int n, int print) {
    if (n <= 0) return 0;
    if (n == 1) { if (print) printf("0 "); return 0; }
    unsigned long long a = 0, b = 1, next;
    if (print) printf("0 1 ");
    for (int i = 3; i <= n; i++) {
        if (a > ULLONG_MAX - b) { printf("\nOverflow detected at term %d\n", i); return 0; }
        next = a + b;
        if (print) printf("%llu ", next);
        a = b;
        b = next;
    }
    if (print) printf("\n");
    return b;
}

unsigned long long fib_recursive_naive(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    unsigned long long f1 = fib_recursive_naive(n - 1);
    unsigned long long f2 = fib_recursive_naive(n - 2);
    if (f1 > ULLONG_MAX - f2) return 0;
    return f1 + f2;
}

unsigned long long fib_memo[1000] = {0};
unsigned long long fib_recursive_memo(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (fib_memo[n] != 0) return fib_memo[n];
    unsigned long long f1 = fib_recursive_memo(n - 1);
    unsigned long long f2 = fib_recursive_memo(n - 2);
    if (f1 > ULLONG_MAX - f2) return 0;
    fib_memo[n] = f1 + f2;
    return fib_memo[n];
}

void fib_fast_double(int n, unsigned long long *f_n, unsigned long long *f_n1) {
    if (n == 0) {
        *f_n = 0;
        *f_n1 = 1;
        return;
    }
    int m = n >> 1;
    unsigned long long a, b;
    fib_fast_double(m, &a, &b);
    unsigned long long c = a * (b * 2 - a);
    unsigned long long d = a * a + b * b;
    if (n & 1) {
        *f_n = d;
        *f_n1 = c + d;
    } else {
        *f_n = c;
        *f_n1 = d;
    }
}

unsigned long long fib_fast_double_n(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    unsigned long long a, b;
    fib_fast_double(n - 1, &a, &b);
    return a;
}

int main() {
    int choice, n;
    clock_t start, end;
    double time_ms;
    unsigned long long result;

    while (1) {
        printf("\n=== Advanced Fibonacci Generator ===\n");
        printf("1. Generate series up to n terms (iterative)\n");
        printf("2. Compute nth Fibonacci (naive recursion)\n");
        printf("3. Compute nth Fibonacci (memoized recursion)\n");
        printf("4. Compute nth Fibonacci (fast doubling)\n");
        printf("5. Compare all methods for nth term\n");
        printf("0. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }

        if (choice == 0) break;
        if (choice < 1 || choice > 5) {
            printf("Invalid choice.\n");
            continue;
        }

        printf("Enter n (positive integer): ");
        if (scanf("%d", &n) != 1 || n < 1) {
            while (getchar() != '\n');
            printf("Invalid input. Must be positive integer.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("Fibonacci series up to %d terms:\n", n);
                start = clock();
                fib_iterative_series(n, 1);
                end = clock();
                time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                printf("Time: %.3f ms\n", time_ms);
                break;

            case 2:
                start = clock();
                result = fib_recursive_naive(n);
                end = clock();
                time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                if (result == 0 && n > 2)
                    printf("Overflow detected or too large.\n");
                else
                    printf("fib(%d) = %llu (naive, time: %.3f ms)\n", n, result, time_ms);
                break;

            case 3:
                for (int i = 0; i <= n; i++) fib_memo[i] = 0;
                start = clock();
                result = fib_recursive_memo(n);
                end = clock();
                time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                if (result == 0 && n > 2)
                    printf("Overflow detected.\n");
                else
                    printf("fib(%d) = %llu (memoized, time: %.3f ms)\n", n, result, time_ms);
                break;

            case 4:
                start = clock();
                result = fib_fast_double_n(n);
                end = clock();
                time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                if (result == 0 && n > 2)
                    printf("Overflow detected.\n");
                else
                    printf("fib(%d) = %llu (fast doubling, time: %.3f ms)\n", n, result, time_ms);
                break;

            case 5:
                printf("\nComparing methods for n = %d:\n", n);
                start = clock();
                unsigned long long res1 = fib_recursive_naive(n);
                end = clock();
                double time1 = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

                for (int i = 0; i <= n; i++) fib_memo[i] = 0;
                start = clock();
                unsigned long long res2 = fib_recursive_memo(n);
                end = clock();
                double time2 = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

                start = clock();
                unsigned long long res3 = fib_fast_double_n(n);
                end = clock();
                double time3 = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

                if ((n > 2) && (res1 == 0 || res2 == 0 || res3 == 0))
                    printf("Overflow detected in one or more methods.\n");
                else if (res1 != res2 || res1 != res3)
                    printf("Mismatch! Inconsistency among methods.\n");
                else
                    printf("fib(%d) = %llu\nNaive: %.3f ms\nMemo: %.3f ms\nFast: %.3f ms\n",
                           n, res1, time1, time2, time3);
                break;
        }
    }
    return 0;
}