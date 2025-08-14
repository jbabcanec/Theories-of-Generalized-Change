#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double quadratic(double x) {
    return x * x;
}

double quadratic_derivative(double x) {
    return 2 * x;
}

double* gradient_descent(double (*func)(double), double (*grad_func)(double),
                        double initial_x, double learning_rate, 
                        int num_iterations, int* history_size) {
    double* x_history = malloc((num_iterations + 1) * sizeof(double));
    if (x_history == NULL) {
        printf("Memory allocation failed\n");
        *history_size = 0;
        return NULL;
    }
    
    double x = initial_x;
    x_history[0] = x;
    
    printf("Gradient Descent Progress:\n");
    printf("Iteration\tx\t\tf(x)\n");
    printf("0\t\t%.6f\t%.6f\n", x, func(x));
    
    for (int i = 0; i < num_iterations; ++i) {
        double grad = grad_func(x);
        x = x - learning_rate * grad;
        x_history[i + 1] = x;
        
        if ((i + 1) % 10 == 0) {
            printf("%d\t\t%.6f\t%.6f\n", i + 1, x, func(x));
        }
    }
    
    *history_size = num_iterations + 1;
    return x_history;
}

void save_optimization_path(double* x_history, int history_size,
                           double (*func)(double), const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    fprintf(file, "iteration,x,fx\n");
    
    for (int i = 0; i < history_size; ++i) {
        fprintf(file, "%d,%.6f,%.6f\n", i, x_history[i], func(x_history[i]));
    }
    
    fclose(file);
    printf("Optimization path saved to %s\n", filename);
}

void test_learning_rates(double (*func)(double), double (*grad_func)(double)) {
    printf("\nEffect of Different Learning Rates:\n");
    printf("Learning Rate\tFinal x\t\tFinal f(x)\n");
    
    double rates[] = {0.01, 0.1, 0.5, 0.9};
    int num_rates = sizeof(rates) / sizeof(rates[0]);
    
    for (int i = 0; i < num_rates; ++i) {
        int history_size;
        double* history = gradient_descent(func, grad_func, 10.0, rates[i], 
                                         20, &history_size);
        
        if (history != NULL) {
            double final_x = history[history_size - 1];
            printf("%.2f\t\t%.6f\t%.6f\n", rates[i], final_x, func(final_x));
            free(history);
        }
    }
}

double rosenbrock_1d(double x) {
    return (1 - x) * (1 - x) + 100 * (1 - x * x) * (1 - x * x);
}

double rosenbrock_1d_derivative(double x) {
    return -2 * (1 - x) - 400 * x * (1 - x * x);
}

int main() {
    printf("Gradient Descent Optimization (C version)\n");
    printf("==========================================\n\n");
    
    // Example 1: Simple quadratic
    printf("Example 1: f(x) = x²\n");
    printf("Initial x: 10.0\n");
    printf("Learning rate: 0.1\n");
    printf("Iterations: 50\n\n");
    
    int history_size;
    double* x_history = gradient_descent(quadratic, quadratic_derivative,
                                       10.0, 0.1, 50, &history_size);
    
    if (x_history != NULL) {
        printf("\nFinal result: x = %.6f, f(x) = %.6f\n", 
               x_history[history_size - 1], 
               quadratic(x_history[history_size - 1]));
        printf("Theoretical minimum: x = 0, f(x) = 0\n");
        
        save_optimization_path(x_history, history_size, quadratic, 
                             "quadratic_optimization_c.csv");
        
        // Convergence analysis
        printf("\nConvergence Analysis:\n");
        printf("Iteration\tx\t\tf(x)\t\tError\n");
        for (int i = 0; i < history_size && i < 20; i += 2) {
            double x = x_history[i];
            double fx = quadratic(x);
            double error = fabs(x - 0);  // Distance from true minimum
            printf("%d\t\t%.6f\t%.6f\t%.6f\n", i, x, fx, error);
        }
        
        free(x_history);
    }
    
    test_learning_rates(quadratic, quadratic_derivative);
    
    // Example 2: More complex function
    printf("\n\nExample 2: Modified Rosenbrock (1D)\n");
    int history_size2;
    double* x_history2 = gradient_descent(rosenbrock_1d, rosenbrock_1d_derivative,
                                        2.0, 0.001, 1000, &history_size2);
    
    if (x_history2 != NULL) {
        printf("Final result: x = %.6f, f(x) = %.6f\n",
               x_history2[history_size2 - 1],
               rosenbrock_1d(x_history2[history_size2 - 1]));
        printf("Theoretical minimum: x = 1, f(x) = 0\n");
        
        save_optimization_path(x_history2, history_size2, rosenbrock_1d,
                             "rosenbrock_optimization_c.csv");
        free(x_history2);
    }
    
    return 0;
}