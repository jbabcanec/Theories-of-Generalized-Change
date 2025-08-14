#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    double t, y;
} Point;

double ode_function(double t, double y) {
    return -2*y + 1;
}

Point* euler_method(double (*f)(double, double), double t0, double y0, 
                   double h, double t_final, int* size) {
    int n = (int)((t_final - t0) / h) + 1;
    Point* solution = malloc(n * sizeof(Point));
    
    double t = t0, y = y0;
    int i = 0;
    
    while (t <= t_final && i < n) {
        solution[i].t = t;
        solution[i].y = y;
        y = y + h * f(t, y);
        t = t + h;
        i++;
    }
    
    *size = i;
    return solution;
}

void save_solution(Point* solution, int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "t,y\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%.6f,%.6f\n", solution[i].t, solution[i].y);
    }
    
    fclose(file);
    printf("Solution saved to %s\n", filename);
}

int main() {
    printf("ODE Solver Examples (C version)\n");
    printf("===============================\n");
    
    int size;
    Point* solution = euler_method(ode_function, 0, 0, 0.1, 2, &size);
    save_solution(solution, size, "euler_solution_c.csv");
    
    free(solution);
    return 0;
}