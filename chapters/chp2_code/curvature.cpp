#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>

class CurvatureCalculator {
private:
    std::function<double(double)> func;
    double dx;
    
public:
    CurvatureCalculator(std::function<double(double)> f, double delta_x = 0.001) 
        : func(f), dx(delta_x) {}
    
    double first_derivative(double x) {
        return (func(x + dx) - func(x - dx)) / (2 * dx);
    }
    
    double second_derivative(double x) {
        return (func(x + dx) - 2 * func(x) + func(x - dx)) / (dx * dx);
    }
    
    double curvature(double x) {
        double f_prime = first_derivative(x);
        double f_double_prime = second_derivative(x);
        double denominator = std::pow(1 + f_prime * f_prime, 1.5);
        return std::abs(f_double_prime) / denominator;
    }
    
    void generate_data(double x_min, double x_max, int num_points, 
                      const std::string& filename, double scaling_factor = 1.0) {
        std::ofstream file(filename);
        file << std::fixed << std::setprecision(6);
        file << "x,y,curvature\n";
        
        double step = (x_max - x_min) / (num_points - 1);
        
        for (int i = 0; i < num_points; ++i) {
            double x = x_min + i * step;
            double y = func(x);
            double k = curvature(x) * scaling_factor;
            file << x << "," << y << "," << k << "\n";
        }
        
        file.close();
        std::cout << "Data written to " << filename << std::endl;
    }
    
    void analyze_curvature(double x_min, double x_max, int num_samples = 100) {
        double max_curvature = 0;
        double max_curvature_x = x_min;
        double min_curvature = 1e10;
        double min_curvature_x = x_min;
        
        double step = (x_max - x_min) / (num_samples - 1);
        
        for (int i = 0; i < num_samples; ++i) {
            double x = x_min + i * step;
            double k = curvature(x);
            
            if (k > max_curvature) {
                max_curvature = k;
                max_curvature_x = x;
            }
            if (k < min_curvature) {
                min_curvature = k;
                min_curvature_x = x;
            }
        }
        
        std::cout << "Curvature Analysis:" << std::endl;
        std::cout << "Maximum curvature: " << max_curvature 
                  << " at x = " << max_curvature_x << std::endl;
        std::cout << "Minimum curvature: " << min_curvature 
                  << " at x = " << min_curvature_x << std::endl;
    }
};

// Example polynomial function
double polynomial_function(double x) {
    return x*x*x - 3*x*x + x - 2;
}

// Example trigonometric function
double trig_function(double x) {
    return std::sin(x) * std::cos(2*x);
}

// Example exponential function
double exp_function(double x) {
    return std::exp(-x*x/2);
}

int main() {
    std::cout << "Curvature Analysis Tool\n";
    std::cout << "========================\n\n";
    
    // Example 1: Polynomial function
    std::cout << "Example 1: Polynomial f(x) = x³ - 3x² + x - 2\n";
    CurvatureCalculator poly_calc(polynomial_function);
    poly_calc.analyze_curvature(-1, 4);
    poly_calc.generate_data(-1, 4, 100, "polynomial_curvature.csv");
    
    // Sample points
    std::cout << "\nSample values:\n";
    for (double x = -1; x <= 4; x += 1) {
        std::cout << "x = " << x << ": ";
        std::cout << "f(x) = " << polynomial_function(x) << ", ";
        std::cout << "κ(x) = " << poly_calc.curvature(x) << std::endl;
    }
    
    // Example 2: Trigonometric function
    std::cout << "\n\nExample 2: Trigonometric f(x) = sin(x)cos(2x)\n";
    CurvatureCalculator trig_calc(trig_function);
    trig_calc.analyze_curvature(-M_PI, M_PI);
    trig_calc.generate_data(-M_PI, M_PI, 200, "trig_curvature.csv");
    
    // Example 3: Gaussian function
    std::cout << "\n\nExample 3: Gaussian f(x) = exp(-x²/2)\n";
    CurvatureCalculator exp_calc(exp_function);
    exp_calc.analyze_curvature(-3, 3);
    exp_calc.generate_data(-3, 3, 150, "gaussian_curvature.csv");
    
    // Road design example (parabolic curve)
    std::cout << "\n\nRoad Design Example: y = x²\n";
    auto parabola = [](double x) { return x * x; };
    CurvatureCalculator road_calc(parabola);
    
    std::cout << "Curvature at different points along the road:\n";
    for (double t = 0; t <= 5; t += 1) {
        double k = road_calc.curvature(t);
        std::cout << "t = " << t << ": κ = " << k;
        
        // Safety analysis
        double radius_of_curvature = (k > 0) ? 1.0 / k : INFINITY;
        std::cout << ", Radius of curvature = " << radius_of_curvature;
        
        if (radius_of_curvature < 50) {
            std::cout << " (Sharp curve - reduce speed!)";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nNote: Generated CSV files can be plotted using Python or other tools.\n";
    
    return 0;
}