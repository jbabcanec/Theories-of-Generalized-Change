#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <iomanip>

class Vector3D {
public:
    double x, y, z;
    
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    double magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    Vector3D normalize() const {
        double mag = magnitude();
        if (mag == 0) return Vector3D(0, 0, 0);
        return Vector3D(x/mag, y/mag, z/mag);
    }
    
    double dot(const Vector3D& other) const {
        return x*other.x + y*other.y + z*other.z;
    }
    
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x
        );
    }
    
    Vector3D project_onto(const Vector3D& other) const {
        double scalar = this->dot(other) / other.dot(other);
        return Vector3D(scalar * other.x, scalar * other.y, scalar * other.z);
    }
    
    double angle_with(const Vector3D& other) const {
        double cos_angle = this->dot(other) / (this->magnitude() * other.magnitude());
        cos_angle = std::max(-1.0, std::min(1.0, cos_angle)); // Clamp to [-1, 1]
        return std::acos(cos_angle);
    }
    
    void print() const {
        std::cout << "[" << x << ", " << y << ", " << z << "]";
    }
};

double triple_scalar_product(const Vector3D& a, const Vector3D& b, const Vector3D& c) {
    return a.dot(b.cross(c));
}

int main() {
    // Define three vectors
    Vector3D v1(1, 2, 3);
    Vector3D v2(4, -1, 2);
    Vector3D v3(2, 3, -1);
    
    // Calculate various operations
    Vector3D proj = v1.project_onto(v2);
    double angle = v1.angle_with(v2);
    double triple = triple_scalar_product(v1, v2, v3);
    Vector3D cross_product = v1.cross(v2);
    double dot_product = v1.dot(v2);
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Vector v1: "; v1.print(); std::cout << std::endl;
    std::cout << "Vector v2: "; v2.print(); std::cout << std::endl;
    std::cout << "Vector v3: "; v3.print(); std::cout << std::endl;
    std::cout << "\nDot product v1·v2: " << dot_product << std::endl;
    std::cout << "Cross product v1×v2: "; cross_product.print(); std::cout << std::endl;
    std::cout << "Projection of v1 onto v2: "; proj.print(); std::cout << std::endl;
    std::cout << "Angle between v1 and v2: " << angle * 180.0 / M_PI << " degrees" << std::endl;
    std::cout << "Triple scalar product: " << triple << std::endl;
    std::cout << "Volume of parallelepiped: " << std::abs(triple) << std::endl;
    
    return 0;
}