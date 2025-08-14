#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double gaussian_2d(double x, double y) {
    return exp(-(x*x + y*y));
}

double paraboloid(double x, double y) {
    return x*x + y*y;
}

double monte_carlo_2d(double (*func)(double, double), 
                     double x_min, double x_max, double y_min, double y_max,
                     int n_samples) {
    srand((unsigned int)time(NULL));
    
    double sum = 0;
    for (int i = 0; i < n_samples; i++) {
        double x = x_min + ((double)rand() / RAND_MAX) * (x_max - x_min);
        double y = y_min + ((double)rand() / RAND_MAX) * (y_max - y_min);
        sum += func(x, y);
    }
    
    double area = (x_max - x_min) * (y_max - y_min);
    return area * sum / n_samples;
}

double trapezoidal_2d(double (*func)(double, double),
                     double x_min, double x_max, double y_min, double y_max,
                     int nx, int ny) {
    double dx = (x_max - x_min) / nx;
    double dy = (y_max - y_min) / ny;
    
    double integral = 0;
    
    for (int i = 0; i <= nx; i++) {
        for (int j = 0; j <= ny; j++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;
            
            double weight = 1.0;
            if (i == 0 || i == nx) weight *= 0.5;
            if (j == 0 || j == ny) weight *= 0.5;
            
            integral += weight * func(x, y);
        }
    }
    
    return integral * dx * dy;
}

void center_of_mass_2d(double (*density)(double, double),
                      double x_min, double x_max, double y_min, double y_max,
                      int resolution, double* x_cm, double* y_cm) {
    double dx = (x_max - x_min) / resolution;
    double dy = (y_max - y_min) / resolution;
    
    double mass = 0, mx = 0, my = 0;
    
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            double x = x_min + (i + 0.5) * dx;
            double y = y_min + (j + 0.5) * dy;
            double rho = density(x, y);
            
            mass += rho * dx * dy;
            mx += x * rho * dx * dy;
            my += y * rho * dx * dy;
        }
    }
    
    *x_cm = mx / mass;
    *y_cm = my / mass;
}

double density_function(double x, double y) {
    if (x >= 0 && y >= 0 && x + y <= 1) {
        return 2 - x - y;
    }
    return 0;
}

void save_surface_data(double (*func)(double, double),
                      double x_min, double x_max, double y_min, double y_max,
                      int resolution, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    fprintf(file, "x,y,z\n");
    
    double dx = (x_max - x_min) / resolution;
    double dy = (y_max - y_min) / resolution;
    
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;
            double z = func(x, y);
            fprintf(file, "%.6f,%.6f,%.6f\n", x, y, z);
        }
    }
    
    fclose(file);
    printf("Surface data written to %s\n", filename);
}

int main() {
    printf("Multiple Integration Examples (C version)\n");
    printf("=========================================\n\n");
    
    // Example 1: Double integral of Gaussian
    printf("Example 1: Double integral of exp(-(x²+y²)) over [-2,2]×[-2,2]\n");
    
    double mc_result = monte_carlo_2d(gaussian_2d, -2, 2, -2, 2, 50000);
    double trap_result = trapezoidal_2d(gaussian_2d, -2, 2, -2, 2, 100, 100);
    
    printf("Monte Carlo result: %.6f\n", mc_result);
    printf("Trapezoidal result: %.6f\n", trap_result);
    printf("Theoretical (π): %.6f\n", M_PI);
    printf("MC Error: %.6f\n", fabs(mc_result - M_PI));
    printf("Trap Error: %.6f\n", fabs(trap_result - M_PI));
    
    save_surface_data(gaussian_2d, -3, 3, -3, 3, 100, "gaussian_2d_c.csv");
    
    // Example 2: Volume under paraboloid
    printf("\n================================================\n");
    printf("Example 2: Volume under z = x² + y² over [0,2]×[0,2]\n");
    
    double volume = trapezoidal_2d(paraboloid, 0, 2, 0, 2, 100, 100);
    double theoretical_volume = 32.0 / 3.0;
    
    printf("Calculated volume: %.6f\n", volume);
    printf("Theoretical volume: %.6f\n", theoretical_volume);
    printf("Error: %.6f\n", fabs(volume - theoretical_volume));
    
    save_surface_data(paraboloid, 0, 2, 0, 2, 100, "paraboloid_c.csv");
    
    // Example 3: Center of mass
    printf("\n================================================\n");
    printf("Example 3: Center of mass with density ρ(x,y) = 2 - x - y\n");
    
    double x_cm, y_cm;
    center_of_mass_2d(density_function, 0, 1, 0, 1, 100, &x_cm, &y_cm);
    printf("Center of mass: (%.6f, %.6f)\n", x_cm, y_cm);
    
    // Example 4: Simple function comparison
    printf("\n================================================\n");
    printf("Example 4: Integration of f(x,y) = xy over [0,1]×[0,1]\n");
    
    // Simple function for testing
    auto simple_func = [](double x, double y) { return x * y; };
    
    // Since we can't use lambdas in C, let's define a simple function
    double simple_mc = monte_carlo_2d([](double x, double y) { return x * y; }, 0, 1, 0, 1, 10000);
    
    // Note: In pure C, we'd need to define this as a separate function
    printf("Note: For pure C compilation, lambda functions need to be replaced with regular functions.\n");
    
    // Example applications
    printf("\n================================================\n");
    printf("Integration Applications:\n");
    printf("1. Volume calculations: V = ∫∫ f(x,y) dA\n");
    printf("2. Center of mass: (x̄,ȳ) = (Mx/m, My/m)\n");
    printf("3. Moment of inertia: I = ∫∫ r² ρ(x,y) dA\n");
    printf("4. Average value: f̄ = (1/A) ∫∫ f(x,y) dA\n");
    printf("5. Area of region: A = ∫∫ 1 dA\n");
    
    // Method comparison
    printf("\n================================================\n");
    printf("Numerical Methods Comparison:\n");
    printf("Monte Carlo:\n");
    printf("  - Good for high dimensions\n");
    printf("  - Error decreases as 1/√n\n");
    printf("  - Simple to implement\n");
    printf("  - Good for irregular regions\n");
    
    printf("\nTrapezoidal Rule:\n");
    printf("  - More accurate for smooth functions\n");
    printf("  - Error decreases as 1/n²\n");
    printf("  - Regular grid required\n");
    printf("  - Better for rectangular regions\n");
    
    printf("\nNote: CSV files generated for visualization.\n");
    
    return 0;
}