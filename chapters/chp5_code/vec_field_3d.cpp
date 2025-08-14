#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <functional>

class VectorField3D {
private:
    std::function<double(double, double, double)> F_x, F_y, F_z;
    
public:
    VectorField3D(std::function<double(double, double, double)> fx,
                  std::function<double(double, double, double)> fy,
                  std::function<double(double, double, double)> fz)
        : F_x(fx), F_y(fy), F_z(fz) {}
    
    std::vector<double> evaluate(double x, double y, double z) {
        return {F_x(x, y, z), F_y(x, y, z), F_z(x, y, z)};
    }
    
    double divergence(double x, double y, double z, double h = 1e-5) {
        double dFx_dx = (F_x(x + h, y, z) - F_x(x - h, y, z)) / (2 * h);
        double dFy_dy = (F_y(x, y + h, z) - F_y(x, y - h, z)) / (2 * h);
        double dFz_dz = (F_z(x, y, z + h) - F_z(x, y, z - h)) / (2 * h);
        return dFx_dx + dFy_dy + dFz_dz;
    }
    
    std::vector<double> curl(double x, double y, double z, double h = 1e-5) {
        // curl = (∂Fz/∂y - ∂Fy/∂z, ∂Fx/∂z - ∂Fz/∂x, ∂Fy/∂x - ∂Fx/∂y)
        double dFz_dy = (F_z(x, y + h, z) - F_z(x, y - h, z)) / (2 * h);
        double dFy_dz = (F_y(x, y, z + h) - F_y(x, y, z - h)) / (2 * h);
        
        double dFx_dz = (F_x(x, y, z + h) - F_x(x, y, z - h)) / (2 * h);
        double dFz_dx = (F_z(x + h, y, z) - F_z(x - h, y, z)) / (2 * h);
        
        double dFy_dx = (F_y(x + h, y, z) - F_y(x - h, y, z)) / (2 * h);
        double dFx_dy = (F_x(x, y + h, z) - F_x(x, y - h, z)) / (2 * h);
        
        return {dFz_dy - dFy_dz, dFx_dz - dFz_dx, dFy_dx - dFx_dy};
    }
    
    void save_field_data(double x_min, double x_max, double y_min, double y_max,
                        double z_min, double z_max, int resolution, 
                        const std::string& filename) {
        std::ofstream file(filename);
        file << "x,y,z,Fx,Fy,Fz,magnitude,divergence,curl_x,curl_y,curl_z\n";
        
        double dx = (x_max - x_min) / resolution;
        double dy = (y_max - y_min) / resolution;
        double dz = (z_max - z_min) / resolution;
        
        for (int i = 0; i <= resolution; ++i) {
            for (int j = 0; j <= resolution; ++j) {
                for (int k = 0; k <= resolution; ++k) {
                    double x = x_min + i * dx;
                    double y = y_min + j * dy;
                    double z = z_min + k * dz;
                    
                    auto field = evaluate(x, y, z);
                    double magnitude = std::sqrt(field[0]*field[0] + field[1]*field[1] + field[2]*field[2]);
                    double div = divergence(x, y, z);
                    auto curl_vec = curl(x, y, z);
                    
                    file << x << "," << y << "," << z << ","
                         << field[0] << "," << field[1] << "," << field[2] << ","
                         << magnitude << "," << div << ","
                         << curl_vec[0] << "," << curl_vec[1] << "," << curl_vec[2] << "\n";
                }
            }
        }
        
        file.close();
        std::cout << "3D vector field data written to " << filename << std::endl;
    }
};

int main() {
    std::cout << "3D Vector Field Analysis (C++)" << std::endl;
    std::cout << "==============================" << std::endl;
    
    // Example: F = <y², 2xy, x+z>
    VectorField3D field(
        [](double x, double y, double z) { return y*y; },
        [](double x, double y, double z) { return 2*x*y; },
        [](double x, double y, double z) { return x + z; }
    );
    
    field.save_field_data(-2, 2, -2, 2, -2, 2, 5, "vector_field_3d_cpp.csv");
    
    return 0;
}