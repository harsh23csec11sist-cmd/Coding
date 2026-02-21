#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b) { if (b != 0) return a / b; else return NAN; }
double modulus(double a, double b) { if (b != 0) return fmod(a, b); else return NAN; }
double power(double a, double b) { return pow(a, b); }
double square_root(double a, double b) { (void)b; if (a >= 0) return sqrt(a); else return NAN; }
double cube_root(double a, double b) { (void)b; return cbrt(a); }
double sine(double a, double b) { (void)b; return sin(a); }
double cosine(double a, double b) { (void)b; return cos(a); }
double tangent(double a, double b) { (void)b; return tan(a); }
double log_base10(double a, double b) { (void)b; if (a > 0) return log10(a); else return NAN; }
double natural_log(double a, double b) { (void)b; if (a > 0) return log(a); else return NAN; }
double factorial_op(double a, double b) {
    (void)b;
    if (a < 0 || floor(a) != a) return NAN;
    unsigned long long n = (unsigned long long)a;
    unsigned long long fact = 1;
    for (unsigned long long i = 2; i <= n; i++) fact *= i;
    return (double)fact;
}
double memory_store(double a, double b) { memory = a; return a; }
double memory_recall(double a, double b) { (void)a; (void)b; return memory; }
double memory_clear(double a, double b) { (void)a; (void)b; memory = 0.0; return 0.0; }

typedef struct {
    int key;
    char symbol[15];
    char description[40];
    double (*func)(double, double);
} Operation;

static double memory = 0.0;

int main() {
    Operation ops[] = {
        {1, "+", "Addition", add},
        {2, "-", "Subtraction", subtract},
        {3, "*", "Multiplication", multiply},
        {4, "/", "Division", divide},
        {5, "%%", "Modulus", modulus},
        {6, "^", "Power", power},
        {7, "sqrt", "Square root", square_root},
        {8, "cbrt", "Cube root", cube_root},
        {9, "sin", "Sine (radians)", sine},
        {10, "cos", "Cosine (radians)", cosine},
        {11, "tan", "Tangent (radians)", tangent},
        {12, "log10", "Log10", log_base10},
        {13, "ln", "Natural log", natural_log},
        {14, "!", "Factorial (integer)", factorial_op},
        {15, "M+", "Memory store", memory_store},
        {16, "MR", "Memory recall", memory_recall},
        {17, "MC", "Memory clear", memory_clear},
        {0, "exit", "Exit program", NULL}
    };
    int choice;
    double a, b, res;
    int num_ops = sizeof(ops) / sizeof(ops[0]) - 1;

    while (1) {
        printf("\n=== Advanced Calculator ===\n");
        for (int i = 0; i < num_ops; i++) {
            printf("%2d. %-6s - %s\n", ops[i].key, ops[i].symbol, ops[i].description);
        }
        printf(" 0. exit   - Exit program\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (choice == 0) break;

        int idx = -1;
        for (int i = 0; i < num_ops; i++) {
            if (ops[i].key == choice) { idx = i; break; }
        }
        if (idx == -1) {
            printf("Invalid choice.\n");
            continue;
        }

        if (ops[idx].func == memory_recall) {
            res = memory_recall(0, 0);
            printf("Result: %g\n", res);
            continue;
        }
        if (ops[idx].func == memory_clear) {
            memory_clear(0, 0);
            printf("Memory cleared.\n");
            continue;
        }

        printf("Enter value for a: ");
        if (scanf("%lf", &a) != 1) {
            while (getchar() != '\n');
            printf("Invalid number.\n");
            continue;
        }

        if (ops[idx].func == square_root || ops[idx].func == cube_root ||
            ops[idx].func == sine || ops[idx].func == cosine ||
            ops[idx].func == tangent || ops[idx].func == log_base10 ||
            ops[idx].func == natural_log || ops[idx].func == factorial_op ||
            ops[idx].func == memory_store) {
            b = 0;
        } else {
            printf("Enter value for b: ");
            if (scanf("%lf", &b) != 1) {
                while (getchar() != '\n');
                printf("Invalid number.\n");
                continue;
            }
        }

        res = ops[idx].func(a, b);
        if (isnan(res)) {
            printf("Error: Operation not possible (division by zero, negative sqrt, etc.)\n");
        } else {
            printf("Result: %g\n", res);
            if (ops[idx].func == memory_store) {
                memory = res;
                printf("Stored in memory.\n");
            }
        }
    }
    return 0;
}