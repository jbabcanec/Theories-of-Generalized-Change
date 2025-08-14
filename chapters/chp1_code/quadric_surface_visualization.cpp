#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

class QuadricSurface {
private:
    double A, B, C, D, E, F, G, H, I, J;
    
public:
    QuadricSurface(double a = 1, double b = 1, double c = 0, 
                   double d = 0, double e = 0, double f = -1,
                   double g = 0, double h = 0, double i = 0, double j = 0)
        : A(a), B(b), C(c), D(d), E(e), F(f), G(g), H(h), I(i), J(j) {}
    
    double evaluate(double x, double y) const {
        return A*x*x + B*y*y + C*x*y + D*x + E*y + F;
    }
    
    void generate_mesh(double x_min, double x_max, double y_min, double y_max, 
                      int num_points, const std::string& filename) {
        std::ofstream file(filename);
        file << std::fixed << std::setprecision(6);
        
        double dx = (x_max - x_min) / (num_points - 1);
        double dy = (y_max - y_min) / (num_points - 1);
        
        // Write header for CSV format
        file << "x,y,z\n";
        
        for (int i = 0; i < num_points; ++i) {
            for (int j = 0; j < num_points; ++j) {
                double x = x_min + i * dx;
                double y = y_min + j * dy;
                double z = evaluate(x, y);
                file << x << "," << y << "," << z << "\n";
            }
        }
        
        file.close();
        std::cout << "Surface data written to " << filename << std::endl;
    }
    
    void print_info() const {
        std::cout << "Quadric Surface Equation:" << std::endl;
        std::cout << "z = " << A << "x² + " << B << "y² + " << C << "xy + "
                  << D << "x + " << E << "y + " << F << std::endl;
        
        // Classify the surface
        double discriminant = 4*A*B - C*C;
        std::cout << "Surface type: ";
        
        if (discriminant > 0) {
            if (A > 0 && B > 0) {
                std::cout << "Elliptic Paraboloid" << std::endl;
            } else if (A < 0 && B < 0) {
                std::cout << "Elliptic Paraboloid (inverted)" << std::endl;
            }
        } else if (discriminant < 0) {
            std::cout << "Hyperbolic Paraboloid (Saddle)" << std::endl;
        } else {
            std::cout << "Parabolic Cylinder or Degenerate" << std::endl;
        }
    }
};

int main() {
    // Example 1: Elliptic Paraboloid
    std::cout << "Example 1: Elliptic Paraboloid\n";
    QuadricSurface surface1(1, 1, 0, 0, 0, -1);
    surface1.print_info();
    surface1.generate_mesh(-2, 2, -2, 2, 50, "elliptic_paraboloid.csv");
    
    std::cout << "\nSample points:\n";
    for (double x = -1; x <= 1; x += 0.5) {
        for (double y = -1; y <= 1; y += 0.5) {
            std::cout << "f(" << x << ", " << y << ") = " 
                     << surface1.evaluate(x, y) << std::endl;
        }
    }
    
    // Example 2: Hyperbolic Paraboloid (Saddle)
    std::cout << "\n\nExample 2: Hyperbolic Paraboloid (Saddle)\n";
    QuadricSurface surface2(1, -1, 0, 0, 0, 0);
    surface2.print_info();
    surface2.generate_mesh(-2, 2, -2, 2, 50, "hyperbolic_paraboloid.csv");
    
    // Example 3: Custom surface
    std::cout << "\n\nExample 3: Custom Surface\n";
    std::cout << "Students can modify coefficients to observe changes:\n";
    double A = 2, B = 3, C = 1, D = -1, E = 2, F = -5;
    QuadricSurface custom(A, B, C, D, E, F);
    custom.print_info();
    custom.generate_mesh(-3, 3, -3, 3, 75, "custom_surface.csv");
    
    std::cout << "\nNote: CSV files can be visualized using Python, MATLAB, or other tools.\n";
    std::cout << "Example Python code to visualize:\n";
    std::cout << "  import pandas as pd\n";
    std::cout << "  import matplotlib.pyplot as plt\n";
    std::cout << "  from mpl_toolkits.mplot3d import Axes3D\n";
    std::cout << "  data = pd.read_csv('elliptic_paraboloid.csv')\n";
    std::cout << "  fig = plt.figure()\n";
    std::cout << "  ax = fig.add_subplot(111, projection='3d')\n";
    std::cout << "  ax.scatter(data['x'], data['y'], data['z'])\n";
    std::cout << "  plt.show()\n";
    
    return 0;
}