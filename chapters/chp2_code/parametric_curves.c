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
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3D vector_subtract(Vector3D a, Vector3D b) {
    return create_vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3D vector_scale(Vector3D v, double scalar) {
    return create_vector(v.x * scalar, v.y * scalar, v.z * scalar);
}

Vector3D vector_cross(Vector3D a, Vector3D b) {
    return create_vector(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Helix parametric equations
Vector3D helix(double t, double a, double b) {
    return create_vector(a * cos(t), a * sin(t), b * t);
}

// Velocity (numerical derivative)
Vector3D velocity(Vector3D (*curve)(double, double, double), double t, double a, double b, double dt) {
    Vector3D r_plus = curve(t + dt, a, b);
    Vector3D r_minus = curve(t - dt, a, b);
    return vector_scale(vector_subtract(r_plus, r_minus), 1.0 / (2 * dt));
}

// Acceleration (numerical second derivative)
Vector3D acceleration(Vector3D (*curve)(double, double, double), double t, double a, double b, double dt) {
    Vector3D v_plus = velocity(curve, t + dt, a, b, dt);
    Vector3D v_minus = velocity(curve, t - dt, a, b, dt);
    return vector_scale(vector_subtract(v_plus, v_minus), 1.0 / (2 * dt));
}

// Arc length calculation
double arc_length(Vector3D (*curve)(double, double, double), double t_start, double t_end, 
                 double a, double b, int num_points) {
    double length = 0;
    double step = (t_end - t_start) / num_points;
    
    for (int i = 0; i < num_points; i++) {
        double t1 = t_start + i * step;
        double t2 = t_start + (i + 1) * step;
        Vector3D r1 = curve(t1, a, b);
        Vector3D r2 = curve(t2, a, b);
        length += vector_magnitude(vector_subtract(r2, r1));
    }
    
    return length;
}

// Curvature calculation
double curvature(Vector3D (*curve)(double, double, double), double t, double a, double b, double dt) {
    Vector3D v = velocity(curve, t, a, b, dt);
    Vector3D acc = acceleration(curve, t, a, b, dt);
    
    Vector3D cross = vector_cross(v, acc);
    double cross_mag = vector_magnitude(cross);
    double v_mag = vector_magnitude(v);
    
    if (v_mag < 1e-10) return 0;
    return cross_mag / (v_mag * v_mag * v_mag);
}

void save_curve_data(Vector3D (*curve)(double, double, double), double t_start, double t_end,
                     double a, double b, int num_points, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    fprintf(file, "t,x,y,z,vx,vy,vz,speed\n");
    
    double step = (t_end - t_start) / (num_points - 1);
    double dt = 0.001;
    
    for (int i = 0; i < num_points; i++) {
        double t = t_start + i * step;
        Vector3D r = curve(t, a, b);
        Vector3D v = velocity(curve, t, a, b, dt);
        double speed = vector_magnitude(v);
        
        fprintf(file, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
                t, r.x, r.y, r.z, v.x, v.y, v.z, speed);
    }
    
    fclose(file);
    printf("Curve data written to %s\n", filename);
}

void print_vector(Vector3D v, const char* name) {
    printf("%s: (%.3f, %.3f, %.3f)\n", name, v.x, v.y, v.z);
}

// Lissajous curve
Vector3D lissajous(double t, double A, double B) {
    double a = 3, b = 2, delta = M_PI / 2;
    return create_vector(A * sin(a * t + delta), B * sin(b * t), 0);
}

// Cycloid
Vector3D cycloid(double t, double R, double unused) {
    return create_vector(R * (t - sin(t)), R * (1 - cos(t)), 0);
}

int main() {
    printf("Parametric Curves Analysis (C version)\n");
    printf("======================================\n\n");
    
    // Example 1: Helix
    printf("Example 1: Helix\n");
    double t_point = M_PI;
    double dt = 0.001;
    double a = 1.0, b = 0.5;
    
    Vector3D r = helix(t_point, a, b);
    Vector3D v = velocity(helix, t_point, a, b, dt);
    Vector3D acc = acceleration(helix, t_point, a, b, dt);
    double arc_len = arc_length(helix, 0, t_point, a, b, 1000);
    double k = curvature(helix, t_point, a, b, dt);
    
    printf("At t = π:\n");
    print_vector(r, "Position");
    print_vector(v, "Velocity");
    printf("Speed: %.3f\n", vector_magnitude(v));
    print_vector(acc, "Acceleration");
    printf("Arc length from 0 to π: %.3f\n", arc_len);
    printf("Curvature: %.6f\n", k);
    
    save_curve_data(helix, 0, 4 * M_PI, a, b, 200, "helix_c.csv");
    
    // Example 2: Lissajous curve
    printf("\n\nExample 2: Lissajous Curve\n");
    t_point = M_PI / 4;
    
    Vector3D r_liss = lissajous(t_point, 1.0, 1.0);
    Vector3D v_liss = velocity(lissajous, t_point, 1.0, 1.0, dt);
    
    printf("At t = π/4:\n");
    print_vector(r_liss, "Position");
    print_vector(v_liss, "Velocity");
    printf("Speed: %.3f\n", vector_magnitude(v_liss));
    
    save_curve_data(lissajous, 0, 2 * M_PI, 1.0, 1.0, 500, "lissajous_c.csv");
    
    // Example 3: Cycloid
    printf("\n\nExample 3: Cycloid\n");
    t_point = M_PI;
    double R = 1.0;
    
    Vector3D r_cyc = cycloid(t_point, R, 0);
    Vector3D v_cyc = velocity(cycloid, t_point, R, 0, dt);
    
    printf("At t = π:\n");
    print_vector(r_cyc, "Position");
    print_vector(v_cyc, "Velocity");
    printf("Speed: %.3f\n", vector_magnitude(v_cyc));
    
    save_curve_data(cycloid, 0, 4 * M_PI, R, 0, 1000, "cycloid_c.csv");
    
    // Curvature analysis for helix
    printf("\n\nCurvature Analysis for Helix:\n");
    printf("t\t\tκ(t)\n");
    for (double t = 0; t <= 2 * M_PI; t += M_PI / 4) {
        double curvature_val = curvature(helix, t, a, b, dt);
        printf("%.6f\t%.6f\n", t, curvature_val);
    }
    
    printf("\nFor a helix with radius a and pitch b, ");
    printf("the curvature is constant: κ = a/(a² + b²)\n");
    double theoretical_curvature = a / (a * a + b * b);
    printf("Theoretical curvature: %.6f\n", theoretical_curvature);
    
    // Special properties
    printf("\n\nSpecial Properties:\n");
    printf("Helix arc length per revolution: %.3f\n", 
           arc_length(helix, 0, 2 * M_PI, a, b, 1000));
    printf("Cycloid arc length for one arch: %.3f\n", 
           arc_length(cycloid, 0, 2 * M_PI, R, 0, 1000));
    
    printf("\nNote: CSV files can be visualized using plotting software.\n");
    
    return 0;
}