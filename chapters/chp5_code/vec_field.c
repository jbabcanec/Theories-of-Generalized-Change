#include <stdio.h>
#include <math.h>

double P_circulation(double x, double y) { return -y; }
double Q_circulation(double x, double y) { return x; }

double P_radial(double x, double y) { return x; }
double Q_radial(double x, double y) { return y; }

double divergence(double (*P)(double, double), double (*Q)(double, double),
                 double x, double y, double h) {
    double dP_dx = (P(x + h, y) - P(x - h, y)) / (2 * h);
    double dQ_dy = (Q(x, y + h) - Q(x, y - h)) / (2 * h);
    return dP_dx + dQ_dy;
}

double curl_2d(double (*P)(double, double), double (*Q)(double, double),
               double x, double y, double h) {
    double dQ_dx = (Q(x + h, y) - Q(x - h, y)) / (2 * h);
    double dP_dy = (P(x, y + h) - P(x, y - h)) / (2 * h);
    return dQ_dx - dP_dy;
}

void save_field_data(double (*P)(double, double), double (*Q)(double, double),
                    double x_min, double x_max, double y_min, double y_max,
                    int resolution, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "x,y,P,Q,magnitude,divergence,curl\n");
    
    double dx = (x_max - x_min) / resolution;
    double dy = (y_max - y_min) / resolution;
    double h = 1e-5;
    
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;
            
            double p_val = P(x, y);
            double q_val = Q(x, y);
            double magnitude = sqrt(p_val * p_val + q_val * q_val);
            double div = divergence(P, Q, x, y, h);
            double curl_val = curl_2d(P, Q, x, y, h);
            
            fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                    x, y, p_val, q_val, magnitude, div, curl_val);
        }
    }
    
    fclose(file);
    printf("Vector field data written to %s\n", filename);
}

int main() {
    printf("2D Vector Field Analysis (C version)\n");
    printf("====================================\n");
    
    printf("Example 1: Circulation field F = <-y, x>\n");
    save_field_data(P_circulation, Q_circulation, -10, 10, -10, 10, 25, "circulation_field_c.csv");
    
    printf("Example 2: Radial field F = <x, y>\n");
    save_field_data(P_radial, Q_radial, -5, 5, -5, 5, 25, "radial_field_c.csv");
    
    return 0;
}