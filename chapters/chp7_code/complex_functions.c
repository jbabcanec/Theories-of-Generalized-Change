#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <string.h>

void analyze_polynomial(const char* name, double x_min, double x_max, double y_min, double y_max,
                       int resolution, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "x,y,real_output,imag_output,magnitude,argument\n");
    
    double dx = (x_max - x_min) / resolution;
    double dy = (y_max - y_min) / resolution;
    
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;
            
            double complex z = x + y * I;
            double complex w;
            
            // Choose function based on name
            if (strcmp(name, "z_squared") == 0) {
                w = z * z;
            } else if (strcmp(name, "z_cubed") == 0) {
                w = z * z * z;
            } else if (strcmp(name, "one_over_z") == 0) {
                if (cabs(z) > 1e-10) {
                    w = 1.0 / z;
                } else {
                    continue; // Skip division by zero
                }
            } else if (strcmp(name, "exp_z") == 0) {
                w = cexp(z);
            } else if (strcmp(name, "sin_z") == 0) {
                w = csin(z);
            } else {
                w = z; // Default to identity
            }
            
            double magnitude = cabs(w);
            double argument = carg(w);
            
            fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                    x, y, creal(w), cimag(w), magnitude, argument);
        }
    }
    
    fclose(file);
    printf("Complex function analysis for %s saved to %s\n", name, filename);
}

void find_approximate_zeros(const char* func_name, double x_min, double x_max, 
                           double y_min, double y_max, int resolution) {
    printf("Searching for zeros of %s...\n", func_name);
    
    double dx = (x_max - x_min) / resolution;
    double dy = (y_max - y_min) / resolution;
    int zero_count = 0;
    
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;
            
            double complex z = x + y * I;
            double complex w;
            
            if (strcmp(func_name, "z_squared") == 0) {
                w = z * z;
            } else if (strcmp(func_name, "z_cubed") == 0) {
                w = z * z * z;
            } else if (strcmp(func_name, "one_over_z") == 0) {
                continue; // No zeros for 1/z
            } else {
                w = z;
            }
            
            if (cabs(w) < 1e-6) {
                printf("  Zero at approximately (%.3f, %.3f)\n", x, y);
                zero_count++;
                if (zero_count >= 5) break; // Limit output
            }
        }
        if (zero_count >= 5) break;
    }
    
    if (zero_count == 0) {
        printf("  No zeros found in the specified region\n");
    }
}

int main() {
    printf("Complex Functions Analysis (C version)\n");
    printf("======================================\n");
    
    // List of functions to analyze
    const char* functions[] = {"z_squared", "z_cubed", "one_over_z", "exp_z", "sin_z"};
    int num_functions = sizeof(functions) / sizeof(functions[0]);
    
    for (int f = 0; f < num_functions; f++) {
        printf("\nAnalyzing %s...\n", functions[f]);
        
        // Choose appropriate domain
        double x_min = -3, x_max = 3, y_min = -3, y_max = 3;
        if (strcmp(functions[f], "exp_z") == 0) {
            y_min = -6; y_max = 6;
        }
        
        // Generate filename
        char filename[100];
        snprintf(filename, sizeof(filename), "complex_function_%s_c.csv", functions[f]);
        
        // Analyze function
        analyze_polynomial(functions[f], x_min, x_max, y_min, y_max, 100, filename);
        
        // Find zeros
        find_approximate_zeros(functions[f], x_min, x_max, y_min, y_max, 30);
    }
    
    printf("\nFunction properties summary:\n");
    printf("• z²: Polynomial, zero at origin, maps upper half-plane to full plane\n");
    printf("• z³: Polynomial, zero at origin, triple covering of plane\n");
    printf("• 1/z: Rational, pole at origin, maps circles to circles\n");
    printf("• e^z: Entire, periodic with period 2πi, exponential growth\n");
    printf("• sin(z): Entire, periodic, zeros at nπ\n");
    
    printf("\nGenerated analysis files for %d complex functions.\n", num_functions);
    printf("Files can be visualized using domain coloring techniques.\n");
    
    return 0;
}