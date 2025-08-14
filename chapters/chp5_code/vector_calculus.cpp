#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <fstream>

class VectorCalculus {
public:
    // Green's theorem verification
    static double line_integral_2d(std::function<std::pair<double, double>(double, double)> F,
                                  std::function<std::pair<double, double>(double)> curve,
                                  double t_min, double t_max, int n_points = 1000) {
        double dt = (t_max - t_min) / n_points;
        double integral = 0;
        
        for (int i = 0; i < n_points; ++i) {
            double t = t_min + i * dt;
            double t_next = t_min + (i + 1) * dt;
            
            auto r = curve(t);
            auto r_next = curve(t_next);
            auto dr = std::make_pair(r_next.first - r.first, r_next.second - r.second);
            
            auto F_val = F(r.first, r.second);
            integral += F_val.first * dr.first + F_val.second * dr.second;
        }
        
        return integral;
    }
    
    // Double integral for Green's theorem
    static double double_integral_2d(std::function<double(double, double)> f,
                                   double x_min, double x_max, double y_min, double y_max,
                                   int nx = 100, int ny = 100) {
        double dx = (x_max - x_min) / nx;
        double dy = (y_max - y_min) / ny;
        double integral = 0;
        
        for (int i = 0; i < nx; ++i) {
            for (int j = 0; j < ny; ++j) {
                double x = x_min + (i + 0.5) * dx;
                double y = y_min + (j + 0.5) * dy;
                integral += f(x, y) * dx * dy;
            }
        }
        
        return integral;
    }
    
    // Curl for 2D vector field (returns scalar)
    static double curl_2d(std::function<double(double, double)> P,
                         std::function<double(double, double)> Q,
                         double x, double y, double h = 1e-5) {
        double dQ_dx = (Q(x + h, y) - Q(x - h, y)) / (2 * h);
        double dP_dy = (P(x, y + h) - P(x, y - h)) / (2 * h);
        return dQ_dx - dP_dy;
    }
    
    // Divergence for 2D vector field
    static double divergence_2d(std::function<double(double, double)> P,
                               std::function<double(double, double)> Q,
                               double x, double y, double h = 1e-5) {
        double dP_dx = (P(x + h, y) - P(x - h, y)) / (2 * h);
        double dQ_dy = (Q(x, y + h) - Q(x, y - h)) / (2 * h);
        return dP_dx + dQ_dy;
    }
    
    static void save_vector_field_analysis(std::function<double(double, double)> P,
                                          std::function<double(double, double)> Q,
                                          double x_min, double x_max, double y_min, double y_max,
                                          int resolution, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,P,Q,divergence,curl,magnitude\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                
                double p_val = P(x, y);
                double q_val = Q(x, y);
                double div = divergence_2d(P, Q, x, y);
                double curl = curl_2d(P, Q, x, y);
                double magnitude = std::sqrt(p_val * p_val + q_val * q_val);
                
                file << x << "," << y << "," << p_val << "," << q_val << ","
                     << div << "," << curl << "," << magnitude << "\n";
            }
        }
        
        file.close();
        std::cout << "Vector field analysis saved to " << filename << std::endl;
    }
};

int main() {
    std::cout << "Vector Calculus Analysis (C++)" << std::endl;
    std::cout << "==============================" << std::endl;
    
    // Example vector field F = <-y, x>
    auto P = [](double x, double y) { return -y; };
    auto Q = [](double x, double y) { return x; };
    
    // Verify Green's theorem for unit circle
    auto F = [&](double x, double y) { return std::make_pair(P(x, y), Q(x, y)); };
    auto unit_circle = [](double t) { return std::make_pair(std::cos(t), std::sin(t)); };
    
    double line_integral = VectorCalculus::line_integral_2d(F, unit_circle, 0, 2*M_PI);
    
    auto curl_func = [&](double x, double y) { return VectorCalculus::curl_2d(P, Q, x, y); };
    double area_integral = VectorCalculus::double_integral_2d(curl_func, -1, 1, -1, 1);
    
    std::cout << "Green's Theorem Verification:" << std::endl;
    std::cout << "Line integral: " << line_integral << std::endl;
    std::cout << "Area integral: " << area_integral << std::endl;
    std::cout << "Difference: " << std::abs(line_integral - area_integral) << std::endl;
    
    VectorCalculus::save_vector_field_analysis(P, Q, -3, 3, -3, 3, 25, "vector_calculus_cpp.csv");
    
    return 0;
}