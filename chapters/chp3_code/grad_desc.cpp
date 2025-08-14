#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <fstream>
#include <iomanip>

class GradientDescent {
private:
    std::function<double(double)> func;
    std::function<double(double)> grad_func;
    
public:
    GradientDescent(std::function<double(double)> f, std::function<double(double)> df)
        : func(f), grad_func(df) {}
    
    std::vector<double> optimize(double initial_x, double learning_rate, 
                                int num_iterations) {
        std::vector<double> x_history;
        double x = initial_x;
        x_history.push_back(x);
        
        for (int i = 0; i < num_iterations; ++i) {
            double grad = grad_func(x);
            x = x - learning_rate * grad;
            x_history.push_back(x);
            
            if ((i + 1) % 10 == 0) {
                std::cout << "Iteration " << (i + 1) << ": x = " << x 
                         << ", f(x) = " << func(x) << std::endl;
            }
        }
        
        return x_history;
    }
    
    void save_optimization_path(const std::vector<double>& x_history, 
                               const std::string& filename) {
        std::ofstream file(filename);
        file << "iteration,x,fx\n";
        
        for (size_t i = 0; i < x_history.size(); ++i) {
            file << i << "," << x_history[i] << "," << func(x_history[i]) << "\n";
        }
        
        file.close();
        std::cout << "Optimization path saved to " << filename << std::endl;
    }
};

// Example functions
double quadratic(double x) {
    return x * x;
}

double quadratic_derivative(double x) {
    return 2 * x;
}

double rosenbrock_1d(double x) {
    return (1 - x) * (1 - x) + 100 * (1 - x * x) * (1 - x * x);
}

double rosenbrock_1d_derivative(double x) {
    return -2 * (1 - x) - 400 * x * (1 - x * x);
}

double noisy_quadratic(double x) {
    return x * x + 0.1 * std::sin(20 * x);
}

double noisy_quadratic_derivative(double x) {
    return 2 * x + 2 * std::cos(20 * x);
}

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Gradient Descent Optimization Examples\n";
    std::cout << "======================================\n\n";
    
    // Example 1: Simple quadratic
    std::cout << "Example 1: f(x) = x²\n";
    GradientDescent gd1(quadratic, quadratic_derivative);
    
    double initial_x = 10.0;
    double learning_rate = 0.1;
    int num_iterations = 50;
    
    std::cout << "Initial x: " << initial_x << std::endl;
    std::cout << "Learning rate: " << learning_rate << std::endl;
    std::cout << "Iterations: " << num_iterations << std::endl << std::endl;
    
    auto x_history1 = gd1.optimize(initial_x, learning_rate, num_iterations);
    gd1.save_optimization_path(x_history1, "quadratic_optimization.csv");
    
    std::cout << "Final result: x = " << x_history1.back() 
              << ", f(x) = " << quadratic(x_history1.back()) << std::endl;
    std::cout << "Theoretical minimum: x = 0, f(x) = 0\n\n";
    
    // Example 2: Different learning rates
    std::cout << "Example 2: Effect of Learning Rate\n";
    std::vector<double> rates = {0.01, 0.1, 0.5, 0.9};
    
    for (double rate : rates) {
        GradientDescent gd_rate(quadratic, quadratic_derivative);
        auto history = gd_rate.optimize(10.0, rate, 20);
        std::cout << "Learning rate " << rate << ": final x = " 
                  << history.back() << std::endl;
    }
    std::cout << std::endl;
    
    // Example 3: More complex function
    std::cout << "Example 3: Modified Rosenbrock (1D)\n";
    GradientDescent gd3(rosenbrock_1d, rosenbrock_1d_derivative);
    auto x_history3 = gd3.optimize(2.0, 0.001, 1000);
    gd3.save_optimization_path(x_history3, "rosenbrock_optimization.csv");
    
    std::cout << "Final result: x = " << x_history3.back() 
              << ", f(x) = " << rosenbrock_1d(x_history3.back()) << std::endl;
    std::cout << "Theoretical minimum: x = 1, f(x) = 0\n\n";
    
    // Example 4: Function with local minima
    std::cout << "Example 4: Function with Multiple Local Minima\n";
    GradientDescent gd4(noisy_quadratic, noisy_quadratic_derivative);
    
    std::vector<double> starting_points = {-3.0, -1.0, 1.0, 3.0};
    for (double start : starting_points) {
        auto history = gd4.optimize(start, 0.05, 100);
        std::cout << "Starting from x = " << start 
                  << ", converged to x = " << history.back() << std::endl;
    }
    
    // Convergence analysis
    std::cout << "\nConvergence Analysis for Quadratic Function:\n";
    std::cout << "Iteration\tx\t\tf(x)\t\tError\n";
    for (size_t i = 0; i < std::min(size_t(20), x_history1.size()); i += 2) {
        double x = x_history1[i];
        double fx = quadratic(x);
        double error = std::abs(x - 0);  // Distance from true minimum
        std::cout << i << "\t\t" << x << "\t" << fx << "\t" << error << std::endl;
    }
    
    return 0;
}