#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

class HeatEquation {
public:
    static std::vector<std::vector<double>> solve_1d(
        std::function<double(double)> initial_condition,
        double L, double T, double alpha, int nx = 100, int nt = 1000) {
        
        double dx = L / (nx - 1);
        double dt = T / nt;
        double r = alpha * dt / (dx * dx);
        
        if (r > 0.5) {
            std::cout << "Warning: r = " << r << " > 0.5, solution may be unstable" << std::endl;
        }
        
        std::vector<std::vector<double>> u(nt, std::vector<double>(nx, 0));
        
        // Set initial condition
        for (int i = 0; i < nx; ++i) {
            double x = i * dx;
            u[0][i] = initial_condition(x);
        }
        
        // Boundary conditions (fixed at zero)
        for (int n = 0; n < nt; ++n) {
            u[n][0] = 0;
            u[n][nx-1] = 0;
        }
        
        // Time stepping
        for (int n = 0; n < nt - 1; ++n) {
            for (int i = 1; i < nx - 1; ++i) {
                u[n+1][i] = u[n][i] + r * (u[n][i+1] - 2*u[n][i] + u[n][i-1]);
            }
        }
        
        return u;
    }
    
    static void save_solution(const std::vector<std::vector<double>>& u,
                             double L, double T, const std::string& filename) {
        std::ofstream file(filename);
        file << "x,t,u\n";
        
        int nt = u.size();
        int nx = u[0].size();
        
        for (int n = 0; n < nt; n += nt/50) { // Sample every 50th time step
            for (int i = 0; i < nx; ++i) {
                double x = i * L / (nx - 1);
                double t = n * T / (nt - 1);
                file << x << "," << t << "," << u[n][i] << "\n";
            }
        }
        
        file.close();
        std::cout << "Heat equation solution saved to " << filename << std::endl;
    }
};

int main() {
    std::cout << "Heat Equation Solver (C++)" << std::endl;
    std::cout << "==========================" << std::endl;
    
    // Initial condition: sin(πx/L)
    auto initial_temp = [](double x) { return std::sin(M_PI * x / 10.0); };
    
    auto solution = HeatEquation::solve_1d(initial_temp, 10.0, 2.0, 0.5);
    HeatEquation::save_solution(solution, 10.0, 2.0, "heat_solution_cpp.csv");
    
    return 0;
}