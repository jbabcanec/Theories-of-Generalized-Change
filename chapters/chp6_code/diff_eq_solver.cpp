#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

class ODESolver {
public:
    // Euler's method for y' = f(t, y)
    static std::vector<std::pair<double, double>> euler_method(
        std::function<double(double, double)> f,
        double t0, double y0, double h, double t_final) {
        
        std::vector<std::pair<double, double>> solution;
        double t = t0, y = y0;
        
        while (t <= t_final) {
            solution.push_back({t, y});
            y = y + h * f(t, y);
            t = t + h;
        }
        
        return solution;
    }
    
    // Runge-Kutta 4th order method
    static std::vector<std::pair<double, double>> rk4_method(
        std::function<double(double, double)> f,
        double t0, double y0, double h, double t_final) {
        
        std::vector<std::pair<double, double>> solution;
        double t = t0, y = y0;
        
        while (t <= t_final) {
            solution.push_back({t, y});
            
            double k1 = h * f(t, y);
            double k2 = h * f(t + h/2, y + k1/2);
            double k3 = h * f(t + h/2, y + k2/2);
            double k4 = h * f(t + h, y + k3);
            
            y = y + (k1 + 2*k2 + 2*k3 + k4) / 6;
            t = t + h;
        }
        
        return solution;
    }
    
    static void save_solution(const std::vector<std::pair<double, double>>& solution,
                             const std::string& filename) {
        std::ofstream file(filename);
        file << "t,y\n";
        for (const auto& point : solution) {
            file << point.first << "," << point.second << "\n";
        }
        file.close();
        std::cout << "Solution saved to " << filename << std::endl;
    }
};

int main() {
    std::cout << "ODE Solver Examples (C++)" << std::endl;
    std::cout << "=========================" << std::endl;
    
    // Example: y' = -2y + 1, y(0) = 0
    auto ode_func = [](double t, double y) { return -2*y + 1; };
    
    auto euler_sol = ODESolver::euler_method(ode_func, 0, 0, 0.1, 2);
    auto rk4_sol = ODESolver::rk4_method(ode_func, 0, 0, 0.1, 2);
    
    ODESolver::save_solution(euler_sol, "euler_solution.csv");
    ODESolver::save_solution(rk4_sol, "rk4_solution.csv");
    
    return 0;
}