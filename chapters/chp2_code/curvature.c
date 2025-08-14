#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double polynomial_function(double x) {
    return x*x*x - 3*x*x + x - 2;
}

double first_derivative(double (*func)(double), double x, double dx) {
    return (func(x + dx) - func(x - dx)) / (2 * dx);
}

double second_derivative(double (*func)(double), double x, double dx) {
    return (func(x + dx) - 2 * func(x) + func(x - dx)) / (dx * dx);
}

double curvature(double (*func)(double), double x, double dx) {
    double f_prime = first_derivative(func, x, dx);
    double f_double_prime = second_derivative(func, x, dx);
    double denominator = pow(1 + f_prime * f_prime, 1.5);
    return fabs(f_double_prime) / denominator;
}

void analyze_curvature(double (*func)(double), double x_min, double x_max, 
                      int num_samples, double dx) {
    double max_curvature = 0;
    double max_curvature_x = x_min;
    double step = (x_max - x_min) / (num_samples - 1);
    
    printf("Curvature analysis:\n");
    printf("x\t\tf(x)\t\tκ(x)\n");
    
    for (int i = 0; i < num_samples; ++i) {
        double x = x_min + i * step;
        double fx = func(x);
        double k = curvature(func, x, dx);
        
        printf("%.3f\t\t%.3f\t\t%.6f\n", x, fx, k);
        
        if (k > max_curvature) {
            max_curvature = k;
            max_curvature_x = x;
        }
    }
    
    printf("\nMaximum curvature: %.6f at x = %.3f\n", max_curvature, max_curvature_x);
}

void save_curvature_data(double (*func)(double), double x_min, double x_max,
                        int num_points, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    fprintf(file, "x,y,curvature\n");
    
    double dx = 0.001;
    double step = (x_max - x_min) / (num_points - 1);
    
    for (int i = 0; i < num_points; ++i) {
        double x = x_min + i * step;
        double y = func(x);
        double k = curvature(func, x, dx);
        fprintf(file, "%.6f,%.6f,%.6f\n", x, y, k);
    }
    
    fclose(file);
    printf("Data written to %s\n", filename);
}

int main() {
    printf("Curvature Analysis Tool (C version)\n");
    printf("====================================\n\n");
    
    printf("Example: Polynomial f(x) = x³ - 3x² + x - 2\n");
    
    double dx = 0.001;  // Small increment for numerical differentiation
    
    analyze_curvature(polynomial_function, -1, 4, 11, dx);
    save_curvature_data(polynomial_function, -1, 4, 100, "polynomial_curvature_c.csv");
    
    // Road design example
    printf("\n\nRoad Design Example:\n");
    printf("For a parabolic road y = x², curvature at different points:\n");
    
    for (double t = 0; t <= 5; t += 1) {
        double k = curvature(polynomial_function, t, dx);
        double radius_of_curvature = (k > 0) ? 1.0 / k : INFINITY;
        
        printf("t = %.1f: κ = %.6f, Radius = %.2f", t, k, radius_of_curvature);
        
        if (radius_of_curvature < 50) {
            printf(" (Sharp curve!)");
        }
        printf("\n");
    }
    
    return 0;
}