#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

bool is_prime_naive(unsigned long long n) {
    if (n < 2) return false;
    for (unsigned long long i = 2; i < n; i++)
        if (n % i == 0) return false;
    return true;
}

bool is_prime_sqrt(unsigned long long n) {
    if (n < 2) return false;
    unsigned long long limit = sqrt(n);
    for (unsigned long long i = 2; i <= limit; i++)
        if (n % i == 0) return false;
    return true;
}

bool is_prime_optimized(unsigned long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    unsigned long long limit = sqrt(n);
    for (unsigned long long i = 5; i <= limit; i += 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

unsigned long long mul_mod(unsigned long long a, unsigned long long b, unsigned long long m) {
    unsigned long long res = 0;
    a %= m;
    while (b) {
        if (b & 1) {
            res = (res + a) % m;
        }
        a = (a << 1) % m;
        b >>= 1;
    }
    return res;
}

unsigned long long pow_mod(unsigned long long a, unsigned long long d, unsigned long long n) {
    unsigned long long res = 1;
    a %= n;
    while (d) {
        if (d & 1) res = mul_mod(res, a, n);
        a = mul_mod(a, a, n);
        d >>= 1;
    }
    return res;
}

bool miller_rabin_test(unsigned long long n, unsigned long long a) {
    unsigned long long d = n - 1;
    while (d % 2 == 0) d /= 2;
    unsigned long long x = pow_mod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    while (d != n - 1) {
        x = mul_mod(x, x, n);
        d *= 2;
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime_miller_rabin(unsigned long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    unsigned long long bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    int num_bases = 7;
    for (int i = 0; i < num_bases; i++) {
        if (bases[i] % n == 0) continue;
        if (!miller_rabin_test(n, bases[i])) return false;
    }
    return true;
}

int main() {
    int choice;
    unsigned long long n;
    clock_t start, end;
    double time_ms;
    bool result;

    while (1) {
        printf("\n=== Advanced Prime Checker ===\n");
        printf("1. Trial division (naive: O(n))\n");
        printf("2. Trial division (sqrt(n))\n");
        printf("3. Trial division (optimized 6k±1)\n");
        printf("4. Miller-Rabin probabilistic (deterministic for 64-bit)\n");
        printf("5. Compare all methods\n");
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

        printf("Enter a positive integer: ");
        if (scanf("%llu", &n) != 1 || n == 0) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1:
                start = clock();
                result = is_prime_naive(n);
                end = clock();
                time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                printf("%llu is %s (naive, time: %.3f ms)\n", n, result ? "prime" : "composite", time_ms);
                break;
            case 2:
                start = clock();
                result = is_prime_sqrt(n);
                end = clock();
                time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                printf("%llu is %s (sqrt, time: %.3f ms)\n", n, result ? "prime" : "composite", time_ms);
                break;
            case 3:
                start = clock();
                result = is_prime_optimized(n);
                end = clock();
                time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                printf("%llu is %s (optimized, time: %.3f ms)\n", n, result ? "prime" : "composite", time_ms);
                break;
            case 4:
                start = clock();
                result = is_prime_miller_rabin(n);
                end = clock();
                time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
                printf("%llu is %s (Miller-Rabin, time: %.3f ms)\n", n, result ? "prime" : "composite", time_ms);
                break;
            case 5:
                printf("\nComparing methods for %llu:\n", n);
                start = clock();
                bool r1 = is_prime_naive(n);
                end = clock();
                double t1 = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

                start = clock();
                bool r2 = is_prime_sqrt(n);
                end = clock();
                double t2 = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

                start = clock();
                bool r3 = is_prime_optimized(n);
                end = clock();
                double t3 = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

                start = clock();
                bool r4 = is_prime_miller_rabin(n);
                end = clock();
                double t4 = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

                if (r1 != r2 || r1 != r3 || r1 != r4)
                    printf("Inconsistency among methods!\n");
                else
                    printf("%llu is %s\n", n, r1 ? "prime" : "composite");

                printf("Naive:       %.3f ms\n", t1);
                printf("Sqrt:        %.3f ms\n", t2);
                printf("Optimized:   %.3f ms\n", t3);
                printf("Miller-Rabin:%.3f ms\n", t4);
                break;
        }
    }
    return 0;
}