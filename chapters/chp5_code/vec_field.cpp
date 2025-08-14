#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

class VectorField2D {
private:
    std::function<double(double, double)> P, Q;
    
public:
    VectorField2D(std::function<double(double, double)> p_func,
                  std::function<double(double, double)> q_func)
        : P(p_func), Q(q_func) {}
    
    std::pair<double, double> evaluate(double x, double y) {
        return {P(x, y), Q(x, y)};
    }
    
    double divergence(double x, double y, double h = 1e-5) {
        double dP_dx = (P(x + h, y) - P(x - h, y)) / (2 * h);
        double dQ_dy = (Q(x, y + h) - Q(x, y - h)) / (2 * h);
        return dP_dx + dQ_dy;
    }
    
    double curl(double x, double y, double h = 1e-5) {
        double dQ_dx = (Q(x + h, y) - Q(x - h, y)) / (2 * h);
        double dP_dy = (P(x, y + h) - P(x, y - h)) / (2 * h);
        return dQ_dx - dP_dy;
    }
    
    void save_field_data(double x_min, double x_max, double y_min, double y_max,
                        int resolution, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,P,Q,magnitude,divergence,curl\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                
                auto field = evaluate(x, y);
                double magnitude = std::sqrt(field.first * field.first + field.second * field.second);
                double div = divergence(x, y);
                double curl_val = curl(x, y);
                
                file << x << "," << y << "," << field.first << "," << field.second
                     << "," << magnitude << "," << div << "," << curl_val << "\n";
            }
        }
        
        file.close();
        std::cout << "Vector field data written to " << filename << std::endl;
    }
};

int main() {
    std::cout << "2D Vector Field Analysis (C++)" << std::endl;
    std::cout << "==============================" << std::endl;
    
    // Example 1: Circulation field F = <-y, x>
    VectorField2D circulation_field(
        [](double x, double y) { return -y; },
        [](double x, double y) { return x; }
    );
    
    std::cout << "Example 1: Circulation field F = <-y, x>" << std::endl;
    circulation_field.save_field_data(-10, 10, -10, 10, 25, "circulation_field.csv");
    
    // Example 2: Radial field F = <x, y>
    VectorField2D radial_field(
        [](double x, double y) { return x; },
        [](double x, double y) { return y; }
    );
    
    std::cout << "Example 2: Radial field F = <x, y>" << std::endl;
    radial_field.save_field_data(-5, 5, -5, 5, 25, "radial_field.csv");
    
    return 0;
}