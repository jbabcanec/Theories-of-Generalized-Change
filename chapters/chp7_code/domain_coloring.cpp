#include <iostream>
#include <complex>
#include <cmath>
#include <fstream>
#include <vector>

class ComplexFunction {
public:
    static std::complex<double> square(std::complex<double> z) {
        return z * z;
    }
    
    static std::complex<double> inverse(std::complex<double> z) {
        return 1.0 / z;
    }
    
    static std::complex<double> exponential(std::complex<double> z) {
        return std::exp(z);
    }
    
    static void domain_coloring_data(std::function<std::complex<double>(std::complex<double>)> f,
                                    double x_min, double x_max, double y_min, double y_max,
                                    int resolution, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,real_output,imag_output,magnitude,argument,hue,brightness\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                
                std::complex<double> z(x, y);
                std::complex<double> w;
                
                try {
                    w = f(z);
                } catch (...) {
                    continue;
                }
                
                double magnitude = std::abs(w);
                double argument = std::arg(w);
                double hue = (argument + M_PI) / (2 * M_PI);
                double brightness = std::log(1 + magnitude);
                
                file << x << "," << y << "," << w.real() << "," << w.imag()
                     << "," << magnitude << "," << argument << "," << hue << "," << brightness << "\n";
            }
        }
        
        file.close();
        std::cout << "Domain coloring data saved to " << filename << std::endl;
    }
};

int main() {
    std::cout << "Domain Coloring Analysis (C++)" << std::endl;
    std::cout << "==============================" << std::endl;
    
    // Generate data for f(z) = z^2
    ComplexFunction::domain_coloring_data(ComplexFunction::square, -2, 2, -2, 2, 200, "z_squared_cpp.csv");
    
    // Generate data for f(z) = 1/z
    ComplexFunction::domain_coloring_data(ComplexFunction::inverse, -2, 2, -2, 2, 200, "inverse_z_cpp.csv");
    
    return 0;
}