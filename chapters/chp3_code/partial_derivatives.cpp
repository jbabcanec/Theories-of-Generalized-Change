#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <fstream>
#include <iomanip>

class PartialDerivatives {
private:
    std::function<double(double, double)> func;
    double h;
    
public:
    PartialDerivatives(std::function<double(double, double)> f, double step = 1e-5)
        : func(f), h(step) {}
    
    double partial_x(double x, double y) {
        return (func(x + h, y) - func(x - h, y)) / (2 * h);
    }
    
    double partial_y(double x, double y) {
        return (func(x, y + h) - func(x, y - h)) / (2 * h);
    }
    
    std::pair<double, double> gradient(double x, double y) {
        return {partial_x(x, y), partial_y(x, y)};
    }
    
    double directional_derivative(double x, double y, double dir_x, double dir_y) {
        auto grad = gradient(x, y);
        double magnitude = std::sqrt(dir_x * dir_x + dir_y * dir_y);
        if (magnitude == 0) return 0;
        dir_x /= magnitude;
        dir_y /= magnitude;
        return grad.first * dir_x + grad.second * dir_y;
    }
    
    double second_partial_xx(double x, double y) {
        return (func(x + h, y) - 2 * func(x, y) + func(x - h, y)) / (h * h);
    }
    
    double second_partial_yy(double x, double y) {
        return (func(x, y + h) - 2 * func(x, y) + func(x, y - h)) / (h * h);
    }
    
    double second_partial_xy(double x, double y) {
        return (func(x + h, y + h) - func(x + h, y - h) - 
                func(x - h, y + h) + func(x - h, y - h)) / (4 * h * h);
    }
    
    std::vector<std::pair<double, double>> find_critical_points(double x_min, double x_max, 
                                                              double y_min, double y_max, 
                                                              int resolution = 50,
                                                              double tolerance = 1e-4) {
        std::vector<std::pair<double, double>> critical_points;
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                auto grad = gradient(x, y);
                
                if (std::abs(grad.first) < tolerance && std::abs(grad.second) < tolerance) {
                    critical_points.push_back({x, y});
                }
            }
        }
        
        return critical_points;
    }
    
    void classify_critical_point(double x, double y) {
        double fxx = second_partial_xx(x, y);
        double fyy = second_partial_yy(x, y);
        double fxy = second_partial_xy(x, y);
        
        double discriminant = fxx * fyy - fxy * fxy;
        
        std::cout << "At point (" << x << ", " << y << "):\n";
        std::cout << "f_xx = " << fxx << ", f_yy = " << fyy << ", f_xy = " << fxy << std::endl;
        std::cout << "Discriminant D = " << discriminant << std::endl;
        
        if (discriminant > 0) {
            if (fxx > 0) {
                std::cout << "Classification: Local minimum\n";
            } else {
                std::cout << "Classification: Local maximum\n";
            }
        } else if (discriminant < 0) {
            std::cout << "Classification: Saddle point\n";
        } else {
            std::cout << "Classification: Test inconclusive\n";
        }
        std::cout << std::endl;
    }
    
    void generate_gradient_field(double x_min, double x_max, double y_min, double y_max,
                                int resolution, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,fx,fy,magnitude\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                auto grad = gradient(x, y);
                double magnitude = std::sqrt(grad.first * grad.first + grad.second * grad.second);
                
                file << x << "," << y << "," << grad.first << "," << grad.second 
                     << "," << magnitude << "\n";
            }
        }
        
        file.close();
        std::cout << "Gradient field data written to " << filename << std::endl;
    }
    
    void generate_contour_data(double x_min, double x_max, double y_min, double y_max,
                              int resolution, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,z\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                double z = func(x, y);
                
                file << x << "," << y << "," << z << "\n";
            }
        }
        
        file.close();
        std::cout << "Contour data written to " << filename << std::endl;
    }
};

// Example functions
double example_function_1(double x, double y) {
    return std::sin(x) * std::cos(y) + 0.5 * x;
}

double example_function_2(double x, double y) {
    return x * x - y * y + 2 * x * y;  // Saddle function
}

double example_function_3(double x, double y) {
    return std::exp(-(x * x + y * y)) * std::cos(x) * std::sin(y);
}

double rosenbrock_function(double x, double y) {
    return (1 - x) * (1 - x) + 100 * (y - x * x) * (y - x * x);
}

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Partial Derivatives Analysis (C++)\n";
    std::cout << "===================================\n\n";
    
    // Example 1: Mixed trigonometric function
    std::cout << "Example 1: f(x,y) = sin(x)cos(y) + 0.5x\n";
    PartialDerivatives pd1(example_function_1);
    
    double x0 = M_PI / 4, y0 = M_PI / 3;
    auto grad1 = pd1.gradient(x0, y0);
    double dir_deriv = pd1.directional_derivative(x0, y0, 1, 1);
    
    std::cout << "At point (" << x0 << ", " << y0 << "):\n";
    std::cout << "Function value: " << example_function_1(x0, y0) << std::endl;
    std::cout << "Gradient: [" << grad1.first << ", " << grad1.second << "]\n";
    std::cout << "Gradient magnitude: " << std::sqrt(grad1.first * grad1.first + grad1.second * grad1.second) << std::endl;
    std::cout << "Directional derivative in direction [1,1]: " << dir_deriv << std::endl;
    
    pd1.generate_gradient_field(-M_PI, M_PI, -M_PI, M_PI, 20, "gradient_field_1.csv");
    pd1.generate_contour_data(-M_PI, M_PI, -M_PI, M_PI, 100, "contour_data_1.csv");
    
    // Example 2: Saddle function
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Example 2: f(x,y) = x² - y² + 2xy (Saddle)\n";
    PartialDerivatives pd2(example_function_2);
    
    auto critical_points = pd2.find_critical_points(-3, 3, -3, 3, 30, 1e-3);
    std::cout << "Critical points found: " << critical_points.size() << std::endl;
    
    for (const auto& point : critical_points) {
        pd2.classify_critical_point(point.first, point.second);
    }
    
    pd2.generate_gradient_field(-3, 3, -3, 3, 20, "gradient_field_2.csv");
    pd2.generate_contour_data(-3, 3, -3, 3, 100, "contour_data_2.csv");
    
    // Example 3: Complex function
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Example 3: f(x,y) = exp(-(x²+y²))cos(x)sin(y)\n";
    PartialDerivatives pd3(example_function_3);
    
    // Analyze several points
    std::vector<std::pair<double, double>> test_points = {
        {0, 0}, {1, 1}, {-1, 1}, {2, 0}, {0, 2}
    };
    
    std::cout << "Analysis at multiple points:\n";
    std::cout << "Point\t\tf(x,y)\t\t∇f\t\t|∇f|\n";
    std::cout << std::string(60, '-') << std::endl;
    
    for (const auto& point : test_points) {
        double x = point.first, y = point.second;
        double fx = example_function_3(x, y);
        auto grad = pd3.gradient(x, y);
        double grad_mag = std::sqrt(grad.first * grad.first + grad.second * grad.second);
        
        std::cout << "(" << x << "," << y << ")\t\t" << fx << "\t["
                  << grad.first << "," << grad.second << "]\t" << grad_mag << std::endl;
    }
    
    pd3.generate_gradient_field(-3, 3, -3, 3, 25, "gradient_field_3.csv");
    pd3.generate_contour_data(-3, 3, -3, 3, 100, "contour_data_3.csv");
    
    // Example 4: Rosenbrock function (optimization classic)
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Example 4: Rosenbrock function f(x,y) = (1-x)² + 100(y-x²)²\n";
    PartialDerivatives pd4(rosenbrock_function);
    
    auto rosenbrock_critical = pd4.find_critical_points(-2, 2, -1, 3, 50, 1e-2);
    std::cout << "Critical points found: " << rosenbrock_critical.size() << std::endl;
    
    for (const auto& point : rosenbrock_critical) {
        std::cout << "Critical point: (" << point.first << ", " << point.second << ")\n";
        std::cout << "Function value: " << rosenbrock_function(point.first, point.second) << std::endl;
        pd4.classify_critical_point(point.first, point.second);
    }
    
    pd4.generate_gradient_field(-2, 2, -1, 3, 30, "gradient_field_rosenbrock.csv");
    pd4.generate_contour_data(-2, 2, -1, 3, 100, "contour_data_rosenbrock.csv");
    
    // Chain rule example
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Chain Rule Application:\n";
    std::cout << "For f(x,y) = sin(x)cos(y) + 0.5x along curve x = t, y = t²\n";
    
    auto chain_rule_demo = [&](double t) {
        double x = t;
        double y = t * t;
        auto grad = pd1.gradient(x, y);
        double dx_dt = 1;
        double dy_dt = 2 * t;
        return grad.first * dx_dt + grad.second * dy_dt;
    };
    
    for (double t = 0; t <= 2; t += 0.5) {
        double df_dt = chain_rule_demo(t);
        std::cout << "t = " << t << ": df/dt = " << df_dt << std::endl;
    }
    
    std::cout << "\nAll CSV files generated for visualization in plotting software.\n";
    
    return 0;
}