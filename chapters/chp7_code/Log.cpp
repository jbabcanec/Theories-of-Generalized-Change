#include <iostream>
#include <complex>
#include <cmath>
#include <fstream>

class RiemannSurface {
public:
    static void generate_log_surface(double r_min, double r_max, double theta_min, double theta_max,
                                   int nr, int ntheta, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,z,real_log,imag_log\n";
        
        double dr = (r_max - r_min) / nr;
        double dtheta = (theta_max - theta_min) / ntheta;
        
        for (int i = 0; i <= nr; ++i) {
            for (int j = 0; j <= ntheta; ++j) {
                double r = r_min + i * dr;
                double theta = theta_min + j * dtheta;
                
                // Parametrization of Riemann surface for log(z)
                double x = r * std::cos(theta);
                double y = r * std::sin(theta);
                double z = theta;  // Height represents the branch
                
                // Complex logarithm: log(z) = ln|z| + i*arg(z)
                std::complex<double> complex_z(x, y);
                std::complex<double> log_z = std::log(complex_z);
                
                file << x << "," << y << "," << z << "," 
                     << log_z.real() << "," << log_z.imag() << "\n";
            }
        }
        
        file.close();
        std::cout << "Riemann surface data saved to " << filename << std::endl;
    }
};

int main() {
    std::cout << "Riemann Surface for log(z) (C++)" << std::endl;
    std::cout << "================================" << std::endl;
    
    RiemannSurface::generate_log_surface(0.1, 2, -4*M_PI, 4*M_PI, 100, 200, "riemann_log_cpp.csv");
    
    return 0;
}