#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double example_function(double x, double y) {
    return sin(x) * cos(y) + 0.5 * x;
}

double saddle_function(double x, double y) {
    return x * x - y * y + 2 * x * y;
}

double partial_x(double (*func)(double, double), double x, double y, double h) {
    return (func(x + h, y) - func(x - h, y)) / (2 * h);
}

double partial_y(double (*func)(double, double), double x, double y, double h) {
    return (func(x, y + h) - func(x, y - h)) / (2 * h);
}

void compute_gradient(double (*func)(double, double), double x, double y, double h, double* grad) {
    grad[0] = partial_x(func, x, y, h);
    grad[1] = partial_y(func, x, y, h);
}

double directional_derivative(double (*func)(double, double), double x, double y, 
                            double dir_x, double dir_y, double h) {
    double grad[2];
    compute_gradient(func, x, y, h, grad);
    
    double magnitude = sqrt(dir_x * dir_x + dir_y * dir_y);
    if (magnitude == 0) return 0;
    
    dir_x /= magnitude;
    dir_y /= magnitude;
    
    return grad[0] * dir_x + grad[1] * dir_y;
}

double second_partial_xx(double (*func)(double, double), double x, double y, double h) {
    return (func(x + h, y) - 2 * func(x, y) + func(x - h, y)) / (h * h);
}

double second_partial_yy(double (*func)(double, double), double x, double y, double h) {
    return (func(x, y + h) - 2 * func(x, y) + func(x, y - h)) / (h * h);
}

double second_partial_xy(double (*func)(double, double), double x, double y, double h) {
    return (func(x + h, y + h) - func(x + h, y - h) - 
            func(x - h, y + h) + func(x - h, y - h)) / (4 * h * h);
}

void classify_critical_point(double (*func)(double, double), double x, double y, double h) {
    double fxx = second_partial_xx(func, x, y, h);
    double fyy = second_partial_yy(func, x, y, h);
    double fxy = second_partial_xy(func, x, y, h);
    
    double discriminant = fxx * fyy - fxy * fxy;
    
    printf("At point (%.3f, %.3f):\n", x, y);
    printf("f_xx = %.6f, f_yy = %.6f, f_xy = %.6f\n", fxx, fyy, fxy);
    printf("Discriminant D = %.6f\n", discriminant);
    
    if (discriminant > 0) {
        if (fxx > 0) {
            printf("Classification: Local minimum\n");
        } else {
            printf("Classification: Local maximum\n");
        }
    } else if (discriminant < 0) {
        printf("Classification: Saddle point\n");
    } else {
        printf("Classification: Test inconclusive\n");
    }
    printf("\n");
}

void save_gradient_field(double (*func)(double, double), double x_min, double x_max,
                        double y_min, double y_max, int resolution, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    fprintf(file, "x,y,fx,fy,magnitude\n");
    
    double dx = (x_max - x_min) / resolution;
    double dy = (y_max - y_min) / resolution;
    double h = 1e-5;
    
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;
            double grad[2];
            compute_gradient(func, x, y, h, grad);
            double magnitude = sqrt(grad[0] * grad[0] + grad[1] * grad[1]);
            
            fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f\n", x, y, grad[0], grad[1], magnitude);
        }
    }
    
    fclose(file);
    printf("Gradient field data written to %s\n", filename);
}

int main() {
    printf("Partial Derivatives Analysis (C version)\n");
    printf("========================================\n\n");
    
    double h = 1e-5;  // Step size for numerical differentiation
    
    // Example 1: Mixed trigonometric function
    printf("Example 1: f(x,y) = sin(x)cos(y) + 0.5x\n");
    
    double x0 = M_PI / 4, y0 = M_PI / 3;
    double grad[2];
    compute_gradient(example_function, x0, y0, h, grad);
    double dir_deriv = directional_derivative(example_function, x0, y0, 1, 1, h);
    
    printf("At point (%.6f, %.6f):\n", x0, y0);
    printf("Function value: %.6f\n", example_function(x0, y0));
    printf("Gradient: [%.6f, %.6f]\n", grad[0], grad[1]);
    printf("Gradient magnitude: %.6f\n", sqrt(grad[0] * grad[0] + grad[1] * grad[1]));
    printf("Directional derivative in direction [1,1]: %.6f\n", dir_deriv);
    
    save_gradient_field(example_function, -M_PI, M_PI, -M_PI, M_PI, 20, "gradient_field_c_1.csv");
    
    // Example 2: Saddle function
    printf("\n================================================\n");
    printf("Example 2: f(x,y) = x² - y² + 2xy (Saddle)\n");
    
    // Check origin for critical point
    compute_gradient(saddle_function, 0, 0, h, grad);
    printf("Gradient at origin: [%.6f, %.6f]\n", grad[0], grad[1]);
    
    if (fabs(grad[0]) < 1e-4 && fabs(grad[1]) < 1e-4) {
        printf("Critical point found at origin\n");
        classify_critical_point(saddle_function, 0, 0, h);
    }
    
    save_gradient_field(saddle_function, -3, 3, -3, 3, 20, "gradient_field_c_2.csv");
    
    // Analysis at multiple points
    printf("\nAnalysis at multiple points:\n");
    printf("Point\t\tf(x,y)\t\t∇f\t\t|∇f|\n");
    printf("------------------------------------------------------------\n");
    
    double test_points[][2] = {{0, 0}, {1, 1}, {-1, 1}, {2, 0}, {0, 2}};
    int num_points = sizeof(test_points) / sizeof(test_points[0]);
    
    for (int i = 0; i < num_points; i++) {
        double x = test_points[i][0];
        double y = test_points[i][1];
        double fx = example_function(x, y);
        compute_gradient(example_function, x, y, h, grad);
        double grad_mag = sqrt(grad[0] * grad[0] + grad[1] * grad[1]);
        
        printf("(%.1f,%.1f)\t\t%.6f\t[%.3f,%.3f]\t%.6f\n", 
               x, y, fx, grad[0], grad[1], grad_mag);
    }
    
    // Chain rule demonstration
    printf("\n================================================\n");
    printf("Chain Rule Application:\n");
    printf("For f(x,y) = sin(x)cos(y) + 0.5x along curve x = t, y = t²\n");
    
    for (double t = 0; t <= 2; t += 0.5) {
        double x = t;
        double y = t * t;
        compute_gradient(example_function, x, y, h, grad);
        double dx_dt = 1;
        double dy_dt = 2 * t;
        double df_dt = grad[0] * dx_dt + grad[1] * dy_dt;
        
        printf("t = %.1f: df/dt = %.6f\n", t, df_dt);
    }
    
    // Tangent plane approximation
    printf("\n================================================\n");
    printf("Tangent Plane Approximation at (π/4, π/3):\n");
    double f0 = example_function(x0, y0);
    compute_gradient(example_function, x0, y0, h, grad);
    
    printf("Linear approximation: L(x,y) = %.6f + %.6f(x - %.6f) + %.6f(y - %.6f)\n",
           f0, grad[0], x0, grad[1], y0);
    
    // Test accuracy of approximation
    double test_x = x0 + 0.1, test_y = y0 + 0.1;
    double actual = example_function(test_x, test_y);
    double approx = f0 + grad[0] * (test_x - x0) + grad[1] * (test_y - y0);
    double error = fabs(actual - approx);
    
    printf("At nearby point (%.3f, %.3f):\n", test_x, test_y);
    printf("Actual value: %.6f\n", actual);
    printf("Linear approximation: %.6f\n", approx);
    printf("Error: %.6f\n", error);
    
    printf("\nNote: CSV files generated for visualization in plotting software.\n");
    
    return 0;
}