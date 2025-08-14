#include <stdio.h>
#include <math.h>

void generate_log_surface(double r_min, double r_max, double theta_min, double theta_max,
                         int nr, int ntheta, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "x,y,z,real_log,imag_log\n");
    
    double dr = (r_max - r_min) / nr;
    double dtheta = (theta_max - theta_min) / ntheta;
    
    for (int i = 0; i <= nr; i++) {
        for (int j = 0; j <= ntheta; j++) {
            double r = r_min + i * dr;
            double theta = theta_min + j * dtheta;
            
            double x = r * cos(theta);
            double y = r * sin(theta);
            double z = theta;
            
            double real_log = log(r);
            double imag_log = theta;
            
            fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f\n",
                    x, y, z, real_log, imag_log);
        }
    }
    
    fclose(file);
    printf("Riemann surface data saved to %s\n", filename);
}

int main() {
    printf("Riemann Surface for log(z) (C version)\n");
    printf("======================================\n");
    
    generate_log_surface(0.1, 2, -4*M_PI, 4*M_PI, 100, 200, "riemann_log_c.csv");
    
    return 0;
}