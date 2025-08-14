#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <array>
#include <iomanip>

struct Vector3D {
    double x, y, z;
    
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }
    
    double magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    void print() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
    }
};

class ParametricCurve {
protected:
    double dt = 0.001;  // Small increment for numerical differentiation
    
public:
    virtual Vector3D position(double t) const = 0;
    
    Vector3D velocity(double t) const {
        Vector3D r_plus = position(t + dt);
        Vector3D r_minus = position(t - dt);
        return (r_plus - r_minus) * (1.0 / (2 * dt));
    }
    
    Vector3D acceleration(double t) const {
        Vector3D v_plus = velocity(t + dt);
        Vector3D v_minus = velocity(t - dt);
        return (v_plus - v_minus) * (1.0 / (2 * dt));
    }
    
    double speed(double t) const {
        return velocity(t).magnitude();
    }
    
    double arc_length(double t_start, double t_end, int num_points = 1000) const {
        double length = 0;
        double step = (t_end - t_start) / num_points;
        
        for (int i = 0; i < num_points; ++i) {
            double t1 = t_start + i * step;
            double t2 = t_start + (i + 1) * step;
            Vector3D r1 = position(t1);
            Vector3D r2 = position(t2);
            length += (r2 - r1).magnitude();
        }
        
        return length;
    }
    
    void generate_curve_data(double t_start, double t_end, int num_points, 
                            const std::string& filename) const {
        std::ofstream file(filename);
        file << std::fixed << std::setprecision(6);
        file << "t,x,y,z,vx,vy,vz,speed\n";
        
        double step = (t_end - t_start) / (num_points - 1);
        
        for (int i = 0; i < num_points; ++i) {
            double t = t_start + i * step;
            Vector3D r = position(t);
            Vector3D v = velocity(t);
            double s = v.magnitude();
            
            file << t << "," << r.x << "," << r.y << "," << r.z << ","
                 << v.x << "," << v.y << "," << v.z << "," << s << "\n";
        }
        
        file.close();
        std::cout << "Curve data written to " << filename << std::endl;
    }
};

class Helix : public ParametricCurve {
private:
    double a, b;  // a = radius, b = vertical speed
    
public:
    Helix(double radius = 1.0, double vertical_speed = 0.5) 
        : a(radius), b(vertical_speed) {}
    
    Vector3D position(double t) const override {
        return Vector3D(a * std::cos(t), a * std::sin(t), b * t);
    }
};

class Lissajous : public ParametricCurve {
private:
    double A, B, a, b, delta;
    
public:
    Lissajous(double A = 1, double B = 1, double a = 3, double b = 2, double delta = M_PI/2)
        : A(A), B(B), a(a), b(b), delta(delta) {}
    
    Vector3D position(double t) const override {
        return Vector3D(A * std::sin(a * t + delta), B * std::sin(b * t), 0);
    }
};

class TorusKnot : public ParametricCurve {
private:
    double R, r, p, q;
    
public:
    TorusKnot(double R = 2, double r = 1, double p = 3, double q = 2)
        : R(R), r(r), p(p), q(q) {}
    
    Vector3D position(double t) const override {
        double x = (R + r * std::cos(q * t)) * std::cos(p * t);
        double y = (R + r * std::cos(q * t)) * std::sin(p * t);
        double z = r * std::sin(q * t);
        return Vector3D(x, y, z);
    }
};

int main() {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Parametric Curves Analysis\n";
    std::cout << "===========================\n\n";
    
    // Example 1: Helix
    std::cout << "Example 1: Helix\n";
    Helix helix(1.0, 0.5);
    
    double t_point = M_PI;
    Vector3D r = helix.position(t_point);
    Vector3D v = helix.velocity(t_point);
    Vector3D a = helix.acceleration(t_point);
    double arc_len = helix.arc_length(0, t_point);
    
    std::cout << "At t = π:\n";
    std::cout << "Position: "; r.print(); std::cout << std::endl;
    std::cout << "Velocity: "; v.print(); std::cout << std::endl;
    std::cout << "Speed: " << v.magnitude() << std::endl;
    std::cout << "Acceleration: "; a.print(); std::cout << std::endl;
    std::cout << "Arc length from 0 to π: " << arc_len << std::endl;
    
    helix.generate_curve_data(0, 4*M_PI, 200, "helix.csv");
    
    // Example 2: Lissajous curve
    std::cout << "\n\nExample 2: Lissajous Curve\n";
    Lissajous lissajous(1, 1, 3, 2, M_PI/2);
    
    t_point = M_PI/4;
    r = lissajous.position(t_point);
    v = lissajous.velocity(t_point);
    
    std::cout << "At t = π/4:\n";
    std::cout << "Position: "; r.print(); std::cout << std::endl;
    std::cout << "Velocity: "; v.print(); std::cout << std::endl;
    std::cout << "Speed: " << v.magnitude() << std::endl;
    
    lissajous.generate_curve_data(0, 2*M_PI, 500, "lissajous.csv");
    
    // Example 3: Torus knot
    std::cout << "\n\nExample 3: Torus Knot (3,2)\n";
    TorusKnot knot(2, 1, 3, 2);
    
    t_point = 0;
    r = knot.position(t_point);
    v = knot.velocity(t_point);
    
    std::cout << "At t = 0:\n";
    std::cout << "Position: "; r.print(); std::cout << std::endl;
    std::cout << "Velocity: "; v.print(); std::cout << std::endl;
    std::cout << "Speed: " << v.magnitude() << std::endl;
    
    double total_length = knot.arc_length(0, 2*M_PI);
    std::cout << "Total arc length (one period): " << total_length << std::endl;
    
    knot.generate_curve_data(0, 2*M_PI, 1000, "torus_knot.csv");
    
    // Curvature analysis for helix
    std::cout << "\n\nCurvature Analysis for Helix:\n";
    std::cout << "t\tκ(t)\n";
    for (double t = 0; t <= 2*M_PI; t += M_PI/4) {
        v = helix.velocity(t);
        a = helix.acceleration(t);
        
        // Curvature formula: |v × a| / |v|³
        Vector3D cross(
            v.y * a.z - v.z * a.y,
            v.z * a.x - v.x * a.z,
            v.x * a.y - v.y * a.x
        );
        
        double curvature = cross.magnitude() / std::pow(v.magnitude(), 3);
        std::cout << t << "\t" << curvature << std::endl;
    }
    
    std::cout << "\nNote: For a helix with radius a and pitch b, ";
    std::cout << "the curvature is constant: κ = a/(a² + b²)\n";
    double theoretical_curvature = 1.0 / (1.0 + 0.5*0.5);
    std::cout << "Theoretical curvature: " << theoretical_curvature << std::endl;
    
    return 0;
}