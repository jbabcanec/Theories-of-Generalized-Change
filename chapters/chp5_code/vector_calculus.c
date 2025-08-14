#include <stdio.h>
#include <math.h>

double P_func(double x, double y) { return -y; }
double Q_func(double x, double y) { return x; }

double curl_2d_func(double x, double y, double h) {
    double dQ_dx = (Q_func(x + h, y) - Q_func(x - h, y)) / (2 * h);
    double dP_dy = (P_func(x, y + h) - P_func(x, y - h)) / (2 * h);
    return dQ_dx - dP_dy;
}

double divergence_2d_func(double x, double y, double h) {
    double dP_dx = (P_func(x + h, y) - P_func(x - h, y)) / (2 * h);
    double dQ_dy = (Q_func(x, y + h) - Q_func(x, y - h)) / (2 * h);
    return dP_dx + dQ_dy;
}

double line_integral_circle(double radius, int n_points) {
    double dt = 2 * M_PI / n_points;
    double integral = 0;
    
    for (int i = 0; i < n_points; i++) {
        double t = i * dt;
        double t_next = (i + 1) * dt;
        
        double x = radius * cos(t);
        double y = radius * sin(t);
        double x_next = radius * cos(t_next);
        double y_next = radius * sin(t_next);
        
        double dx = x_next - x;
        double dy = y_next - y;
        
        integral += P_func(x, y) * dx + Q_func(x, y) * dy;
    }
    
    return integral;
}

void save_vector_analysis(double x_min, double x_max, double y_min, double y_max,
                         int resolution, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "x,y,P,Q,divergence,curl,magnitude\n");
    
    double dx = (x_max - x_min) / resolution;
    double dy = (y_max - y_min) / resolution;
    double h = 1e-5;
    
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;
            
            double p = P_func(x, y);
            double q = Q_func(x, y);
            double div = divergence_2d_func(x, y, h);
            double curl = curl_2d_func(x, y, h);
            double magnitude = sqrt(p*p + q*q);
            
            fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                    x, y, p, q, div, curl, magnitude);
        }
    }
    
    fclose(file);
    printf("Vector field analysis saved to %s\n", filename);
}

int main() {
    printf("Vector Calculus Analysis (C version)\n");
    printf("====================================\n");
    
    double line_int = line_integral_circle(1.0, 1000);
    printf("Line integral around unit circle: %.6f\n", line_int);
    printf("Expected (2π): %.6f\n", 2 * M_PI);
    printf("Error: %.6f\n", fabs(line_int - 2 * M_PI));
    
    save_vector_analysis(-3, 3, -3, 3, 25, "vector_calculus_c.csv");
    
    return 0;
}