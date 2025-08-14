#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <random>
#include <fstream>
#include <iomanip>

class MultipleIntegration {
public:
    // Monte Carlo integration for 2D functions
    static double monte_carlo_2d(std::function<double(double, double)> f,
                                double x_min, double x_max, double y_min, double y_max,
                                int n_samples = 10000) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> x_dist(x_min, x_max);
        std::uniform_real_distribution<> y_dist(y_min, y_max);
        
        double sum = 0;
        for (int i = 0; i < n_samples; ++i) {
            double x = x_dist(gen);
            double y = y_dist(gen);
            sum += f(x, y);
        }
        
        double area = (x_max - x_min) * (y_max - y_min);
        return area * sum / n_samples;
    }
    
    // Trapezoidal rule for 2D integration
    static double trapezoidal_2d(std::function<double(double, double)> f,
                                double x_min, double x_max, double y_min, double y_max,
                                int nx = 100, int ny = 100) {
        double dx = (x_max - x_min) / nx;
        double dy = (y_max - y_min) / ny;
        
        double integral = 0;
        
        for (int i = 0; i <= nx; ++i) {
            for (int j = 0; j <= ny; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                
                double weight = 1.0;
                if (i == 0 || i == nx) weight *= 0.5;
                if (j == 0 || j == ny) weight *= 0.5;
                
                integral += weight * f(x, y);
            }
        }
        
        return integral * dx * dy;
    }
    
    // Volume calculation using double integration
    static double volume_under_surface(std::function<double(double, double)> f,
                                      double x_min, double x_max, double y_min, double y_max,
                                      int resolution = 100) {
        return trapezoidal_2d(f, x_min, x_max, y_min, y_max, resolution, resolution);
    }
    
    // Center of mass calculation
    static std::pair<double, double> center_of_mass_2d(std::function<double(double, double)> density,
                                                       double x_min, double x_max, 
                                                       double y_min, double y_max,
                                                       int resolution = 50) {
        // Total mass
        double mass = trapezoidal_2d(density, x_min, x_max, y_min, y_max, resolution, resolution);
        
        // First moments
        auto mx_integrand = [&](double x, double y) { return x * density(x, y); };
        auto my_integrand = [&](double x, double y) { return y * density(x, y); };
        
        double mx = trapezoidal_2d(mx_integrand, x_min, x_max, y_min, y_max, resolution, resolution);
        double my = trapezoidal_2d(my_integrand, x_min, x_max, y_min, y_max, resolution, resolution);
        
        return {mx / mass, my / mass};
    }
    
    // Surface area calculation
    static double surface_area(std::function<double(double, double)> f,
                              double x_min, double x_max, double y_min, double y_max,
                              int resolution = 100) {
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        double h = 1e-5;
        
        double area = 0;
        
        for (int i = 0; i < resolution; ++i) {
            for (int j = 0; j < resolution; ++j) {
                double x = x_min + (i + 0.5) * dx;
                double y = y_min + (j + 0.5) * dy;
                
                // Partial derivatives
                double fx = (f(x + h, y) - f(x - h, y)) / (2 * h);
                double fy = (f(x, y + h) - f(x, y - h)) / (2 * h);
                
                // Surface area element
                double ds = std::sqrt(1 + fx*fx + fy*fy);
                area += ds * dx * dy;
            }
        }
        
        return area;
    }
    
    // Polar coordinate integration
    static double polar_integration(std::function<double(double, double)> f_polar,
                                   double r_min, double r_max, double theta_min, double theta_max,
                                   int nr = 50, int ntheta = 50) {
        double dr = (r_max - r_min) / nr;
        double dtheta = (theta_max - theta_min) / ntheta;
        
        double integral = 0;
        
        for (int i = 0; i < nr; ++i) {
            for (int j = 0; j < ntheta; ++j) {
                double r = r_min + (i + 0.5) * dr;
                double theta = theta_min + (j + 0.5) * dtheta;
                
                // Include Jacobian factor r for polar coordinates
                integral += f_polar(r, theta) * r * dr * dtheta;
            }
        }
        
        return integral;
    }
    
    static void save_integration_data(std::function<double(double, double)> f,
                                     double x_min, double x_max, double y_min, double y_max,
                                     int resolution, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,z\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                double z = f(x, y);
                file << x << "," << y << "," << z << "\n";
            }
        }
        
        file.close();
        std::cout << "Integration surface data written to " << filename << std::endl;
    }
};

// Example functions
double gaussian_2d(double x, double y) {
    return std::exp(-(x*x + y*y));
}

double paraboloid(double x, double y) {
    return x*x + y*y;
}

double saddle(double x, double y) {
    return x*x - y*y;
}

double density_function(double x, double y) {
    return 2 - x - y;
}

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Multiple Integration Examples (C++)\n";
    std::cout << "===================================\n\n";
    
    // Example 1: Double integral of Gaussian
    std::cout << "Example 1: Double integral of exp(-(x²+y²)) over [-2,2]×[-2,2]\n";
    
    double mc_result = MultipleIntegration::monte_carlo_2d(gaussian_2d, -2, 2, -2, 2, 50000);
    double trap_result = MultipleIntegration::trapezoidal_2d(gaussian_2d, -2, 2, -2, 2, 100, 100);
    
    std::cout << "Monte Carlo result: " << mc_result << std::endl;
    std::cout << "Trapezoidal result: " << trap_result << std::endl;
    std::cout << "Theoretical (π): " << M_PI << std::endl;
    std::cout << "MC Error: " << std::abs(mc_result - M_PI) << std::endl;
    std::cout << "Trap Error: " << std::abs(trap_result - M_PI) << std::endl;
    
    MultipleIntegration::save_integration_data(gaussian_2d, -3, 3, -3, 3, 100, "gaussian_2d.csv");
    
    // Example 2: Volume under paraboloid
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Example 2: Volume under z = x² + y² over [0,2]×[0,2]\n";
    
    double volume = MultipleIntegration::volume_under_surface(paraboloid, 0, 2, 0, 2);
    double theoretical_volume = 32.0 / 3.0;  // Analytical result
    
    std::cout << "Calculated volume: " << volume << std::endl;
    std::cout << "Theoretical volume: " << theoretical_volume << std::endl;
    std::cout << "Error: " << std::abs(volume - theoretical_volume) << std::endl;
    
    MultipleIntegration::save_integration_data(paraboloid, 0, 2, 0, 2, 100, "paraboloid.csv");
    
    // Example 3: Center of mass
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Example 3: Center of mass with density ρ(x,y) = 2 - x - y\n";
    
    auto cm = MultipleIntegration::center_of_mass_2d(density_function, 0, 1, 0, 1);
    std::cout << "Center of mass: (" << cm.first << ", " << cm.second << ")\n";
    
    // Example 4: Surface area
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Example 4: Surface area of z = x² + y² over [0,1]×[0,1]\n";
    
    double surf_area = MultipleIntegration::surface_area(paraboloid, 0, 1, 0, 1, 50);
    std::cout << "Surface area: " << surf_area << std::endl;
    
    // Example 5: Polar coordinate integration
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Example 5: Polar integration of r*exp(-r²) over disk of radius 2\n";
    
    auto polar_func = [](double r, double theta) { return r * std::exp(-r*r); };
    double polar_result = MultipleIntegration::polar_integration(polar_func, 0, 2, 0, 2*M_PI);
    double polar_theoretical = M_PI * (1 - std::exp(-4));
    
    std::cout << "Polar integration result: " << polar_result << std::endl;
    std::cout << "Theoretical result: " << polar_theoretical << std::endl;
    std::cout << "Error: " << std::abs(polar_result - polar_theoretical) << std::endl;
    
    // Example 6: Comparison of methods
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Example 6: Method comparison for ∫∫ xy dA over [0,1]×[0,1]\n";
    
    auto simple_func = [](double x, double y) { return x * y; };
    
    double mc_simple = MultipleIntegration::monte_carlo_2d(simple_func, 0, 1, 0, 1, 10000);
    double trap_simple = MultipleIntegration::trapezoidal_2d(simple_func, 0, 1, 0, 1, 50, 50);
    double exact_simple = 0.25;  // Analytical result
    
    std::cout << "Function: f(x,y) = xy\n";
    std::cout << "Monte Carlo: " << mc_simple << " (error: " << std::abs(mc_simple - exact_simple) << ")\n";
    std::cout << "Trapezoidal: " << trap_simple << " (error: " << std::abs(trap_simple - exact_simple) << ")\n";
    std::cout << "Exact: " << exact_simple << std::endl;
    
    // Applications
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "Applications Summary:\n";
    std::cout << "1. Volume calculations: V = ∫∫ f(x,y) dA\n";
    std::cout << "2. Surface area: A = ∫∫ √(1 + f_x² + f_y²) dA\n";
    std::cout << "3. Center of mass: (x̄,ȳ) = (M_x/m, M_y/m)\n";
    std::cout << "4. Moment of inertia: I = ∫∫ r² ρ(x,y) dA\n";
    std::cout << "5. Average value: f̄ = (1/A) ∫∫ f(x,y) dA\n";
    
    std::cout << "\nNote: CSV files generated for 3D visualization.\n";
    
    return 0;
}