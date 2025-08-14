#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double initial_condition(double x) {
    return sin(M_PI * x / 10.0);
}

double** solve_heat_equation_1d(double L, double T, double alpha, int nx, int nt) {
    double dx = L / (nx - 1);
    double dt = T / nt;
    double r = alpha * dt / (dx * dx);
    
    if (r > 0.5) {
        printf("Warning: r = %.3f > 0.5, solution may be unstable\n", r);
    }
    
    // Allocate memory for solution
    double** u = malloc(nt * sizeof(double*));
    for (int n = 0; n < nt; n++) {
        u[n] = malloc(nx * sizeof(double));
        for (int i = 0; i < nx; i++) {
            u[n][i] = 0;
        }
    }
    
    // Set initial condition
    for (int i = 0; i < nx; i++) {
        double x = i * dx;
        u[0][i] = initial_condition(x);
    }
    
    // Boundary conditions (fixed at zero)
    for (int n = 0; n < nt; n++) {
        u[n][0] = 0;
        u[n][nx-1] = 0;
    }
    
    // Time stepping
    for (int n = 0; n < nt - 1; n++) {
        for (int i = 1; i < nx - 1; i++) {
            u[n+1][i] = u[n][i] + r * (u[n][i+1] - 2*u[n][i] + u[n][i-1]);
        }
    }
    
    return u;
}

void save_solution(double** u, int nx, int nt, double L, double T, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "x,t,u\n");
    
    for (int n = 0; n < nt; n += nt/50) { // Sample every 50th time step
        for (int i = 0; i < nx; i++) {
            double x = i * L / (nx - 1);
            double t = n * T / (nt - 1);
            fprintf(file, "%.6f,%.6f,%.6f\n", x, t, u[n][i]);
        }
    }
    
    fclose(file);
    printf("Heat equation solution saved to %s\n", filename);
}

void free_solution(double** u, int nt) {
    for (int n = 0; n < nt; n++) {
        free(u[n]);
    }
    free(u);
}

int main() {
    printf("Heat Equation Solver (C version)\n");
    printf("================================\n");
    
    int nx = 100, nt = 1000;
    double L = 10.0, T = 2.0, alpha = 0.5;
    
    double** solution = solve_heat_equation_1d(L, T, alpha, nx, nt);
    save_solution(solution, nx, nt, L, T, "heat_solution_c.csv");
    free_solution(solution, nt);
    
    return 0;
}