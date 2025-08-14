#include <stdio.h>
#include <math.h>
#include <complex.h>

void save_domain_coloring(double x_min, double x_max, double y_min, double y_max,
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
            double complex w = z * z;  // f(z) = z^2
            
            double magnitude = cabs(w);
            double argument = carg(w);
            
            fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                    x, y, creal(w), cimag(w), magnitude, argument);
        }
    }
    
    fclose(file);
    printf("Domain coloring data saved to %s\n", filename);
}

int main() {
    printf("Domain Coloring Analysis (C version)\n");
    printf("====================================\n");
    
    save_domain_coloring(-2, 2, -2, 2, 200, "z_squared_c.csv");
    
    return 0;
}