#include <iostream>
#include <complex>
#include <vector>
#include <cmath>
#include <fstream>
#include <functional>

class ComplexVisualization {
public:
    static void domain_coloring_comprehensive(
        std::function<std::complex<double>(std::complex<double>)> f,
        double x_min, double x_max, double y_min, double y_max,
        int resolution, const std::string& filename) {
        
        std::ofstream file(filename);
        file << "x,y,real_output,imag_output,magnitude,argument,hue,brightness,is_pole,is_zero\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                
                std::complex<double> z(x, y);
                std::complex<double> w;
                bool is_pole = false, is_zero = false;
                
                try {
                    w = f(z);
                    
                    double magnitude = std::abs(w);
                    if (magnitude > 1e6) is_pole = true;
                    if (magnitude < 1e-6) is_zero = true;
                    
                    double argument = std::arg(w);
                    double hue = (argument + M_PI) / (2 * M_PI);
                    double brightness = std::log(1 + magnitude);
                    
                    file << x << "," << y << "," << w.real() << "," << w.imag()
                         << "," << magnitude << "," << argument << "," << hue 
                         << "," << brightness << "," << is_pole << "," << is_zero << "\n";
                         
                } catch (...) {
                    file << x << "," << y << ",NaN,NaN,NaN,NaN,NaN,NaN,1,0\n";
                }
            }
        }
        
        file.close();
        std::cout << "Complex visualization data saved to " << filename << std::endl;
    }
    
    static void grid_mapping(std::function<std::complex<double>(std::complex<double>)> f,
                           double x_min, double x_max, double y_min, double y_max,
                           int grid_lines, const std::string& filename) {
        std::ofstream file(filename);
        file << "type,x,y,mapped_x,mapped_y\n";
        
        // Vertical lines
        for (int i = 0; i <= grid_lines; ++i) {
            double x = x_min + i * (x_max - x_min) / grid_lines;
            for (int j = 0; j <= 100; ++j) {
                double y = y_min + j * (y_max - y_min) / 100;
                std::complex<double> z(x, y);
                std::complex<double> w = f(z);
                file << "vertical," << x << "," << y << "," << w.real() << "," << w.imag() << "\n";
            }
        }
        
        // Horizontal lines
        for (int j = 0; j <= grid_lines; ++j) {
            double y = y_min + j * (y_max - y_min) / grid_lines;
            for (int i = 0; i <= 100; ++i) {
                double x = x_min + i * (x_max - x_min) / 100;
                std::complex<double> z(x, y);
                std::complex<double> w = f(z);
                file << "horizontal," << x << "," << y << "," << w.real() << "," << w.imag() << "\n";
            }
        }
        
        file.close();
        std::cout << "Grid mapping data saved to " << filename << std::endl;
    }
};

// Complex functions
std::complex<double> f_square(std::complex<double> z) { return z * z; }
std::complex<double> f_exp(std::complex<double> z) { return std::exp(z); }
std::complex<double> f_sin(std::complex<double> z) { return std::sin(z); }
std::complex<double> f_mobius(std::complex<double> z) { return (z - 1.0) / (z + 1.0); }

int main() {
    std::cout << "Complex Function Visualization (C++)" << std::endl;
    std::cout << "====================================" << std::endl;
    
    ComplexVisualization::domain_coloring_comprehensive(f_square, -2, 2, -2, 2, 200, "z_squared_visualization_cpp.csv");
    ComplexVisualization::domain_coloring_comprehensive(f_exp, -2, 2, -6, 6, 200, "exp_z_visualization_cpp.csv");
    ComplexVisualization::domain_coloring_comprehensive(f_mobius, -3, 3, -3, 3, 200, "mobius_visualization_cpp.csv");
    
    ComplexVisualization::grid_mapping(f_square, -2, 2, -2, 2, 10, "grid_mapping_z2_cpp.csv");
    
    return 0;
}