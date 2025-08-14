import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy import integrate

def divergence_2d(F_x, F_y, x, y, h=1e-5):
    """Calculate divergence of 2D vector field"""
    dFx_dx = (F_x(x + h, y) - F_x(x - h, y)) / (2 * h)
    dFy_dy = (F_y(x, y + h) - F_y(x, y - h)) / (2 * h)
    return dFx_dx + dFy_dy

def curl_2d(F_x, F_y, x, y, h=1e-5):
    """Calculate curl of 2D vector field (returns scalar for 2D)"""
    dFy_dx = (F_y(x + h, y) - F_y(x - h, y)) / (2 * h)
    dFx_dy = (F_x(x, y + h) - F_x(x, y - h)) / (2 * h)
    return dFy_dx - dFx_dy

def line_integral_scalar(f, curve_param, t_range, n_points=1000):
    """Calculate line integral of scalar field along parametric curve"""
    t = np.linspace(*t_range, n_points)
    dt = t[1] - t[0]
    
    integral = 0
    for i in range(len(t) - 1):
        r = curve_param(t[i])
        r_next = curve_param(t[i+1])
        ds = np.linalg.norm(r_next - r)
        integral += f(*r[:2]) * ds
    
    return integral

def line_integral_vector(F, curve_param, t_range, n_points=1000):
    """Calculate line integral of vector field along parametric curve"""
    t = np.linspace(*t_range, n_points)
    
    integral = 0
    for i in range(len(t) - 1):
        r = curve_param(t[i])
        r_next = curve_param(t[i+1])
        dr = r_next - r
        F_val = F(*r[:2])
        integral += np.dot(F_val, dr[:2])
    
    return integral

def greens_theorem_verification(F_x, F_y, boundary_curve, t_range):
    """Verify Green's theorem by comparing line and double integrals"""
    # Line integral around boundary
    F = lambda x, y: np.array([F_x(x, y), F_y(x, y)])
    line_int = line_integral_vector(F, boundary_curve, t_range)
    
    # Double integral of curl over region (simplified for unit circle)
    def curl_func(y, x):
        return curl_2d(F_x, F_y, x, y)
    
    # For unit circle
    double_int, _ = integrate.dblquad(
        curl_func,
        -1, 1,
        lambda x: -np.sqrt(max(0, 1-x**2)),
        lambda x: np.sqrt(max(0, 1-x**2))
    )
    
    return line_int, double_int

def plot_streamlines(F_x, F_y, x_range, y_range, density=1):
    """Plot streamlines of a vector field"""
    x = np.linspace(*x_range, 30)
    y = np.linspace(*y_range, 30)
    X, Y = np.meshgrid(x, y)
    
    U = F_x(X, Y)
    V = F_y(X, Y)
    
    return X, Y, U, V

# Example usage
if __name__ == "__main__":
    # Define a vector field
    F_x = lambda x, y: -y
    F_y = lambda x, y: x
    
    # Test point
    x0, y0 = 1.0, 0.5
    
    # Calculate divergence and curl
    div = divergence_2d(F_x, F_y, x0, y0)
    curl = curl_2d(F_x, F_y, x0, y0)
    
    print(f"Vector field F = <-y, x> at point ({x0}, {y0}):")
    print(f"Divergence: {div:.6f}")
    print(f"Curl: {curl:.6f}")
    
    # Verify Green's theorem
    # Boundary: unit circle
    boundary = lambda t: np.array([np.cos(t), np.sin(t), 0])
    line_int, area_int = greens_theorem_verification(F_x, F_y, boundary, (0, 2*np.pi))
    
    print(f"\nGreen's Theorem Verification:")
    print(f"Line integral: {line_int:.6f}")
    print(f"Double integral: {area_int:.6f}")
    print(f"Difference: {abs(line_int - area_int):.6f}")
    
    # Visualization
    fig = plt.figure(figsize=(15, 5))
    
    # Plot 1: Vector field with streamlines
    ax1 = fig.add_subplot(131)
    X, Y, U, V = plot_streamlines(F_x, F_y, (-3, 3), (-3, 3))
    
    # Normalize for better visualization
    M = np.sqrt(U**2 + V**2)
    M[M == 0] = 1
    U_norm, V_norm = U/M, V/M
    
    ax1.quiver(X, Y, U_norm, V_norm, M, cmap='viridis')
    ax1.streamplot(X, Y, U, V, density=1.5, color='white', linewidth=0.5, alpha=0.5)
    circle = plt.Circle((0, 0), 1, fill=False, color='red', linewidth=2)
    ax1.add_patch(circle)
    ax1.set_xlabel('X')
    ax1.set_ylabel('Y')
    ax1.set_title('Vector Field with Streamlines')
    ax1.set_aspect('equal')
    ax1.grid(True, alpha=0.3)
    
    # Plot 2: Divergence field
    ax2 = fig.add_subplot(132)
    div_field = np.zeros_like(X)
    for i in range(X.shape[0]):
        for j in range(X.shape[1]):
            div_field[i, j] = divergence_2d(F_x, F_y, X[i, j], Y[i, j])
    
    div_plot = ax2.contourf(X, Y, div_field, levels=20, cmap='RdBu_r')
    plt.colorbar(div_plot, ax=ax2, label='Divergence')
    ax2.set_xlabel('X')
    ax2.set_ylabel('Y')
    ax2.set_title('Divergence of Vector Field')
    ax2.set_aspect('equal')
    
    # Plot 3: Curl field
    ax3 = fig.add_subplot(133)
    curl_field = np.zeros_like(X)
    for i in range(X.shape[0]):
        for j in range(X.shape[1]):
            curl_field[i, j] = curl_2d(F_x, F_y, X[i, j], Y[i, j])
    
    curl_plot = ax3.contourf(X, Y, curl_field, levels=20, cmap='PuOr')
    plt.colorbar(curl_plot, ax=ax3, label='Curl')
    ax3.set_xlabel('X')
    ax3.set_ylabel('Y')
    ax3.set_title('Curl of Vector Field')
    ax3.set_aspect('equal')
    
    plt.tight_layout()
    plt.show()