#include <stdio.h>
#include <math.h>

double F_x(double x, double y, double z) { return y*y; }
double F_y(double x, double y, double z) { return 2*x*y; }
double F_z(double x, double y, double z) { return x + z; }

double divergence_3d(double x, double y, double z, double h) {
    double dFx_dx = (F_x(x + h, y, z) - F_x(x - h, y, z)) / (2 * h);
    double dFy_dy = (F_y(x, y + h, z) - F_y(x, y - h, z)) / (2 * h);
    double dFz_dz = (F_z(x, y, z + h) - F_z(x, y, z - h)) / (2 * h);
    return dFx_dx + dFy_dy + dFz_dz;
}

void curl_3d(double x, double y, double z, double h, double* curl) {
    double dFz_dy = (F_z(x, y + h, z) - F_z(x, y - h, z)) / (2 * h);
    double dFy_dz = (F_y(x, y, z + h) - F_y(x, y, z - h)) / (2 * h);
    
    double dFx_dz = (F_x(x, y, z + h) - F_x(x, y, z - h)) / (2 * h);
    double dFz_dx = (F_z(x + h, y, z) - F_z(x - h, y, z)) / (2 * h);
    
    double dFy_dx = (F_y(x + h, y, z) - F_y(x - h, y, z)) / (2 * h);
    double dFx_dy = (F_x(x, y + h, z) - F_x(x, y - h, z)) / (2 * h);
    
    curl[0] = dFz_dy - dFy_dz;
    curl[1] = dFx_dz - dFz_dx;
    curl[2] = dFy_dx - dFx_dy;
}

void save_field_data_3d(double x_min, double x_max, double y_min, double y_max,
                       double z_min, double z_max, int resolution, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "x,y,z,Fx,Fy,Fz,magnitude,divergence,curl_x,curl_y,curl_z\n");
    
    double dx = (x_max - x_min) / resolution;
    double dy = (y_max - y_min) / resolution;
    double dz = (z_max - z_min) / resolution;
    double h = 1e-5;
    
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            for (int k = 0; k <= resolution; k++) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                double z = z_min + k * dz;
                
                double fx = F_x(x, y, z);
                double fy = F_y(x, y, z);
                double fz = F_z(x, y, z);
                double magnitude = sqrt(fx*fx + fy*fy + fz*fz);
                double div = divergence_3d(x, y, z, h);
                
                double curl_vec[3];
                curl_3d(x, y, z, h, curl_vec);
                
                fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                        x, y, z, fx, fy, fz, magnitude, div, 
                        curl_vec[0], curl_vec[1], curl_vec[2]);
            }
        }
    }
    
    fclose(file);
    printf("3D vector field data written to %s\n", filename);
}

int main() {
    printf("3D Vector Field Analysis (C version)\n");
    printf("====================================\n");
    
    save_field_data_3d(-2, 2, -2, 2, -2, 2, 5, "vector_field_3d_c.csv");
    
    return 0;
}