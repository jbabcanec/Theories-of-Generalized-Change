#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    double x, y, z;
} Vector3D;

Vector3D create_vector(double x, double y, double z) {
    Vector3D v = {x, y, z};
    return v;
}

double vector_magnitude(Vector3D v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector3D vector_normalize(Vector3D v) {
    double mag = vector_magnitude(v);
    if (mag == 0) return create_vector(0, 0, 0);
    return create_vector(v.x/mag, v.y/mag, v.z/mag);
}

double vector_dot(Vector3D a, Vector3D b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3D vector_cross(Vector3D a, Vector3D b) {
    return create_vector(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

Vector3D vector_project(Vector3D a, Vector3D b) {
    double scalar = vector_dot(a, b) / vector_dot(b, b);
    return create_vector(scalar * b.x, scalar * b.y, scalar * b.z);
}

double vector_angle(Vector3D a, Vector3D b) {
    double cos_angle = vector_dot(a, b) / (vector_magnitude(a) * vector_magnitude(b));
    if (cos_angle > 1.0) cos_angle = 1.0;
    if (cos_angle < -1.0) cos_angle = -1.0;
    return acos(cos_angle);
}

double triple_scalar_product(Vector3D a, Vector3D b, Vector3D c) {
    return vector_dot(a, vector_cross(b, c));
}

void print_vector(Vector3D v, const char* name) {
    printf("%s: [%.3f, %.3f, %.3f]\n", name, v.x, v.y, v.z);
}

int main() {
    Vector3D v1 = create_vector(1, 2, 3);
    Vector3D v2 = create_vector(4, -1, 2);
    Vector3D v3 = create_vector(2, 3, -1);
    
    print_vector(v1, "Vector v1");
    print_vector(v2, "Vector v2");
    print_vector(v3, "Vector v3");
    
    double dot_product = vector_dot(v1, v2);
    Vector3D cross_product = vector_cross(v1, v2);
    Vector3D projection = vector_project(v1, v2);
    double angle = vector_angle(v1, v2);
    double triple = triple_scalar_product(v1, v2, v3);
    
    printf("\nOperations:\n");
    printf("Dot product v1·v2: %.3f\n", dot_product);
    print_vector(cross_product, "Cross product v1×v2");
    print_vector(projection, "Projection of v1 onto v2");
    printf("Angle between v1 and v2: %.3f degrees\n", angle * 180.0 / M_PI);
    printf("Triple scalar product: %.3f\n", triple);
    printf("Volume of parallelepiped: %.3f\n", fabs(triple));
    
    return 0;
}