#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct {
    double A, B, C, D, E, F, G, H, I, J;
} QuadricSurface;

QuadricSurface create_quadric(double a, double b, double c, double d, double e, double f,
                             double g, double h, double i, double j) {
    QuadricSurface surface = {a, b, c, d, e, f, g, h, i, j};
    return surface;
}

double evaluate_surface(QuadricSurface s, double x, double y) {
    return s.A * x * x + s.B * y * y + s.C * x * y + s.D * x + s.E * y + s.F;
}

const char* classify_surface(QuadricSurface s) {
    double discriminant = 4 * s.A * s.B - s.C * s.C;
    
    if (discriminant > 0) {
        if (s.A > 0 && s.B > 0) {
            return "Elliptic Paraboloid";
        } else if (s.A < 0 && s.B < 0) {
            return "Elliptic Paraboloid (inverted)";
        } else {
            return "Elliptic surface";
        }
    } else if (discriminant < 0) {
        return "Hyperbolic Paraboloid (Saddle)";
    } else {
        return "Parabolic Cylinder or Degenerate";
    }
}

void print_surface_info(QuadricSurface s) {
    printf("Quadric Surface Information:\n");
    printf("Equation: z = %.1fx² + %.1fy² + %.1fxy + %.1fx + %.1fy + %.1f\n",
           s.A, s.B, s.C, s.D, s.E, s.F);
    printf("Surface type: %s\n", classify_surface(s));
    printf("Coefficients: A=%.1f, B=%.1f, C=%.1f, D=%.1f, E=%.1f, F=%.1f\n",
           s.A, s.B, s.C, s.D, s.E, s.F);
}

void generate_mesh(QuadricSurface s, double x_min, double x_max, double y_min, double y_max,
                   int num_points, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    fprintf(file, "x,y,z\n");
    
    double dx = (x_max - x_min) / (num_points - 1);
    double dy = (y_max - y_min) / (num_points - 1);
    
    for (int i = 0; i < num_points; i++) {
        for (int j = 0; j < num_points; j++) {
            double x = x_min + i * dx;
            double y = y_min + j * dy;
            double z = evaluate_surface(s, x, y);
            fprintf(file, "%.6f,%.6f,%.6f\n", x, y, z);
        }
    }
    
    fclose(file);
    printf("Surface data written to %s\n", filename);
}

void analyze_critical_points(QuadricSurface s, double x_min, double x_max, 
                           double y_min, double y_max) {
    // For quadric surfaces, critical points occur where partial derivatives are zero
    // ∂z/∂x = 2Ax + Cy + D = 0
    // ∂z/∂y = 2By + Cx + E = 0
    
    double det = 4 * s.A * s.B - s.C * s.C;
    
    if (fabs(det) > 1e-10) {
        // Unique critical point
        double x_crit = (s.C * s.E - 2 * s.B * s.D) / det;
        double y_crit = (s.C * s.D - 2 * s.A * s.E) / det;
        
        if (x_crit >= x_min && x_crit <= x_max && 
            y_crit >= y_min && y_crit <= y_max) {
            double z_crit = evaluate_surface(s, x_crit, y_crit);
            printf("Critical point found: (%.3f, %.3f, %.3f)\n", x_crit, y_crit, z_crit);
            
            // Classify critical point using second derivative test
            double fxx = 2 * s.A;
            double fyy = 2 * s.B;
            double fxy = s.C;
            
            double discriminant = fxx * fyy - fxy * fxy;
            
            if (discriminant > 0) {
                if (fxx > 0) {
                    printf("Type: Local minimum\n");
                } else {
                    printf("Type: Local maximum\n");
                }
            } else if (discriminant < 0) {
                printf("Type: Saddle point\n");
            } else {
                printf("Type: Inconclusive (second derivative test fails)\n");
            }
        } else {
            printf("Critical point exists but outside specified range\n");
        }
    } else {
        printf("No unique critical point (degenerate case)\n");
    }
}

void analyze_level_curves(QuadricSurface s) {
    printf("\nLevel Curves Analysis:\n");
    double levels[] = {-2, -1, 0, 1, 2};
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    
    for (int i = 0; i < num_levels; i++) {
        double level = levels[i];
        printf("\nLevel curve z = %.1f:\n", level);
        printf("Equation: %.1fx² + %.1fy² = %.1f\n", s.A, s.B, level - s.F);
        
        double rhs = level - s.F;
        if (rhs > 0 && s.A > 0 && s.B > 0) {
            double a = sqrt(rhs / s.A);
            double b = sqrt(rhs / s.B);
            printf("Ellipse with semi-axes a=%.3f, b=%.3f\n", a, b);
        } else if (rhs == 0) {
            printf("Point (origin)\n");
        } else {
            printf("No real solutions (empty set)\n");
        }
    }
}

int main() {
    printf("Quadric Surface Analysis (C version)\n");
    printf("====================================\n\n");
    
    // Example 1: Elliptic Paraboloid
    printf("Example 1: Elliptic Paraboloid\n");
    QuadricSurface surface1 = create_quadric(1, 1, 0, 0, 0, -1, 0, 0, 0, 0);
    print_surface_info(surface1);
    analyze_critical_points(surface1, -3, 3, -3, 3);
    generate_mesh(surface1, -2, 2, -2, 2, 50, "elliptic_paraboloid_c.csv");
    
    printf("\nSample points:\n");
    for (double x = -1; x <= 1; x += 0.5) {
        for (double y = -1; y <= 1; y += 0.5) {
            printf("f(%.1f, %.1f) = %.3f\n", x, y, evaluate_surface(surface1, x, y));
        }
    }
    
    // Example 2: Hyperbolic Paraboloid (Saddle)
    printf("\n================================================\n");
    printf("Example 2: Hyperbolic Paraboloid (Saddle)\n");
    QuadricSurface surface2 = create_quadric(1, -1, 0, 0, 0, 0, 0, 0, 0, 0);
    print_surface_info(surface2);
    analyze_critical_points(surface2, -3, 3, -3, 3);
    generate_mesh(surface2, -2, 2, -2, 2, 50, "hyperbolic_paraboloid_c.csv");
    
    // Example 3: Complex surface
    printf("\n================================================\n");
    printf("Example 3: Complex Surface with Cross Term\n");
    QuadricSurface surface3 = create_quadric(2, 3, 1, -1, 2, -5, 0, 0, 0, 0);
    print_surface_info(surface3);
    analyze_critical_points(surface3, -3, 3, -3, 3);
    generate_mesh(surface3, -3, 3, -3, 3, 75, "complex_surface_c.csv");
    
    analyze_level_curves(surface1);
    
    printf("\nNote: Generated CSV files can be visualized using:\n");
    printf("- Python with matplotlib\n");
    printf("- R with plot3d\n");
    printf("- Excel or Google Sheets with 3D plotting\n");
    printf("- Any scientific plotting software\n");
    
    return 0;
}