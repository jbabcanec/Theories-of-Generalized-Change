#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include <fstream>
#include <map>
#include <string>

class ComplexFunction {
private:
    std::function<std::complex<double>(std::complex<double>)> func;
    
public:
    ComplexFunction(std::function<std::complex<double>(std::complex<double>)> f) : func(f) {}
    
    std::vector<std::complex<double>> find_zeros(double x_min, double x_max, double y_min, double y_max, 
                                                int resolution = 50, double tolerance = 1e-6) {
        std::vector<std::complex<double>> zeros;
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                std::complex<double> z(x, y);
                
                try {
                    std::complex<double> w = func(z);
                    if (std::abs(w) < tolerance) {
                        zeros.push_back(z);
                    }
                } catch (...) {
                    // Skip points where function is undefined
                }
            }
        }
        
        return zeros;
    }
    
    std::vector<std::complex<double>> find_poles(double x_min, double x_max, double y_min, double y_max,
                                                int resolution = 50, double threshold = 1e6) {
        std::vector<std::complex<double>> poles;
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                std::complex<double> z(x, y);
                
                try {
                    std::complex<double> w = func(z);
                    if (std::abs(w) > threshold) {
                        poles.push_back(z);
                    }
                } catch (...) {
                    poles.push_back(z);
                }
            }
        }
        
        return poles;
    }
    
    void save_analysis(double x_min, double x_max, double y_min, double y_max,
                      int resolution, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,real_output,imag_output,magnitude,argument\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                std::complex<double> z(x, y);
                
                try {
                    std::complex<double> w = func(z);
                    double magnitude = std::abs(w);
                    double argument = std::arg(w);
                    
                    file << x << "," << y << "," << w.real() << "," << w.imag()
                         << "," << magnitude << "," << argument << "\n";
                } catch (...) {
                    file << x << "," << y << ",NaN,NaN,NaN,NaN\n";
                }
            }
        }
        
        file.close();
        std::cout << "Complex function analysis saved to " << filename << std::endl;
    }
};

// Define complex functions
std::map<std::string, std::function<std::complex<double>(std::complex<double>)>> create_function_library() {
    std::map<std::string, std::function<std::complex<double>(std::complex<double>)>> functions;
    
    // Polynomial functions
    functions["z"] = [](std::complex<double> z) { return z; };
    functions["z_squared"] = [](std::complex<double> z) { return z * z; };
    functions["z_cubed"] = [](std::complex<double> z) { return z * z * z; };
    functions["z_squared_plus_1"] = [](std::complex<double> z) { return z * z + 1.0; };
    
    // Rational functions
    functions["one_over_z"] = [](std::complex<double> z) { 
        if (std::abs(z) < 1e-10) throw std::runtime_error("Division by zero");
        return 1.0 / z; 
    };
    functions["one_over_z_squared_plus_1"] = [](std::complex<double> z) { return 1.0 / (z * z + 1.0); };
    functions["mobius"] = [](std::complex<double> z) { return (z - 1.0) / (z + 1.0); };
    
    // Transcendental functions
    functions["exp_z"] = [](std::complex<double> z) { return std::exp(z); };
    functions["sin_z"] = [](std::complex<double> z) { return std::sin(z); };
    functions["cos_z"] = [](std::complex<double> z) { return std::cos(z); };
    functions["log_z"] = [](std::complex<double> z) { 
        if (std::abs(z) < 1e-10) throw std::runtime_error("Log of zero");
        return std::log(z); 
    };
    
    // Special functions
    functions["sqrt_z"] = [](std::complex<double> z) { return std::sqrt(z); };
    functions["exp_one_over_z"] = [](std::complex<double> z) { 
        if (std::abs(z) < 1e-10) throw std::runtime_error("Essential singularity");
        return std::exp(1.0 / z); 
    };
    functions["z_times_exp_z"] = [](std::complex<double> z) { return z * std::exp(z); };
    
    return functions;
}

int main() {
    std::cout << "Complex Functions Analysis (C++)" << std::endl;
    std::cout << "================================" << std::endl;
    
    auto functions = create_function_library();
    
    // Analyze selected functions
    std::vector<std::string> selected = {"z_squared", "one_over_z", "exp_z", "sin_z", "mobius", "exp_one_over_z"};
    
    for (const auto& func_name : selected) {
        if (functions.find(func_name) != functions.end()) {
            std::cout << "Analyzing " << func_name << "..." << std::endl;
            
            ComplexFunction cf(functions[func_name]);
            
            // Choose appropriate domain
            double x_min = -3, x_max = 3, y_min = -3, y_max = 3;
            if (func_name == "exp_z") {
                y_min = -6; y_max = 6;
            } else if (func_name == "exp_one_over_z") {
                x_min = -1; x_max = 1; y_min = -1; y_max = 1;
            }
            
            // Save analysis
            std::string filename = "complex_function_" + func_name + "_cpp.csv";
            cf.save_analysis(x_min, x_max, y_min, y_max, 100, filename);
            
            // Find zeros and poles
            auto zeros = cf.find_zeros(x_min, x_max, y_min, y_max, 30);
            auto poles = cf.find_poles(x_min, x_max, y_min, y_max, 30);
            
            if (!zeros.empty()) {
                std::cout << "  Zeros found: " << zeros.size() << std::endl;
                for (size_t i = 0; i < std::min(size_t(5), zeros.size()); ++i) {
                    std::cout << "    " << zeros[i] << std::endl;
                }
            }
            
            if (!poles.empty()) {
                std::cout << "  Poles found: " << poles.size() << std::endl;
                for (size_t i = 0; i < std::min(size_t(5), poles.size()); ++i) {
                    std::cout << "    " << poles[i] << std::endl;
                }
            }
        }
    }
    
    std::cout << "\nFunction properties summary:" << std::endl;
    std::cout << "• Polynomial functions: Entire (analytic everywhere)" << std::endl;
    std::cout << "• Rational functions: Meromorphic (poles at zeros of denominator)" << std::endl;
    std::cout << "• Exponential functions: Entire, periodic in imaginary direction" << std::endl;
    std::cout << "• Trigonometric functions: Meromorphic, periodic" << std::endl;
    std::cout << "• Essential singularities: exp(1/z) has dense behavior near z=0" << std::endl;
    
    std::cout << "\nGenerated analysis files for " << selected.size() << " complex functions." << std::endl;
    std::cout << "Files can be visualized using domain coloring techniques." << std::endl;
    
    return 0;
}