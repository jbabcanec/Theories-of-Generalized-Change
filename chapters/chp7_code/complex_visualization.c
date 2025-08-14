#include <stdio.h>
#include <math.h>
#include <complex.h>

void save_domain_coloring_basic(double x_min, double x_max, double y_min, double y_max,
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
    printf("Complex visualization data saved to %s\n", filename);
}

void save_grid_mapping_basic(double x_min, double x_max, double y_min, double y_max,
                             int grid_lines, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "type,x,y,mapped_x,mapped_y\n");
    
    // Vertical lines
    for (int i = 0; i <= grid_lines; i++) {
        double x = x_min + i * (x_max - x_min) / grid_lines;
        for (int j = 0; j <= 100; j++) {
            double y = y_min + j * (y_max - y_min) / 100.0;
            double complex z = x + y * I;
            double complex w = z * z;
            fprintf(file, "vertical,%.6f,%.6f,%.6f,%.6f\n",
                    x, y, creal(w), cimag(w));
        }
    }
    
    // Horizontal lines
    for (int j = 0; j <= grid_lines; j++) {
        double y = y_min + j * (y_max - y_min) / grid_lines;
        for (int i = 0; i <= 100; i++) {
            double x = x_min + i * (x_max - x_min) / 100.0;
            double complex z = x + y * I;
            double complex w = z * z;
            fprintf(file, "horizontal,%.6f,%.6f,%.6f,%.6f\n",
                    x, y, creal(w), cimag(w));
        }
    }
    
    fclose(file);
    printf("Grid mapping data saved to %s\n", filename);
}

int main() {
    printf("Complex Function Visualization (C version)\n");
    printf("==========================================\n");
    
    save_domain_coloring_basic(-2, 2, -2, 2, 200, "z_squared_visualization_c.csv");
    save_grid_mapping_basic(-2, 2, -2, 2, 10, "grid_mapping_z2_c.csv");
    
    return 0;
}