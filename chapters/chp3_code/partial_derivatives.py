import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def compute_gradient(f, x, y, h=1e-5):
    """Compute gradient of f at point (x, y) numerically"""
    df_dx = (f(x + h, y) - f(x - h, y)) / (2 * h)
    df_dy = (f(x, y + h) - f(x, y - h)) / (2 * h)
    return np.array([df_dx, df_dy])

def directional_derivative(f, x, y, direction):
    """Compute directional derivative of f at (x, y) in given direction"""
    grad = compute_gradient(f, x, y)
    unit_dir = direction / np.linalg.norm(direction)
    return np.dot(grad, unit_dir)

def tangent_plane(f, x0, y0, x_range, y_range):
    """Generate tangent plane to surface at point (x0, y0)"""
    grad = compute_gradient(f, x0, y0)
    z0 = f(x0, y0)
    
    X, Y = np.meshgrid(x_range, y_range)
    Z = z0 + grad[0] * (X - x0) + grad[1] * (Y - y0)
    return X, Y, Z

def critical_points(f, x_range, y_range, threshold=1e-4):
    """Find critical points where gradient is near zero"""
    critical = []
    x_test = np.linspace(x_range[0], x_range[1], 50)
    y_test = np.linspace(y_range[0], y_range[1], 50)
    
    for x in x_test:
        for y in y_test:
            grad = compute_gradient(f, x, y)
            if np.linalg.norm(grad) < threshold:
                critical.append((x, y, f(x, y)))
    
    return critical

# Example function: saddle point
def saddle_function(x, y):
    return x**2 - y**2 + 2*x*y

# Example usage
if __name__ == "__main__":
    # Define test function
    f = lambda x, y: np.sin(x) * np.cos(y) + 0.5 * x
    
    # Point of interest
    x0, y0 = np.pi/4, np.pi/3
    
    # Compute various derivatives
    grad = compute_gradient(f, x0, y0)
    dir_deriv = directional_derivative(f, x0, y0, np.array([1, 1]))
    
    print(f"At point ({x0:.3f}, {y0:.3f}):")
    print(f"Function value: {f(x0, y0):.3f}")
    print(f"Gradient: [{grad[0]:.3f}, {grad[1]:.3f}]")
    print(f"Gradient magnitude: {np.linalg.norm(grad):.3f}")
    print(f"Directional derivative in [1,1]: {dir_deriv:.3f}")
    
    # Create visualization
    fig = plt.figure(figsize=(15, 5))
    
    # Surface plot
    ax1 = fig.add_subplot(131, projection='3d')
    x = np.linspace(-np.pi, np.pi, 100)
    y = np.linspace(-np.pi, np.pi, 100)
    X, Y = np.meshgrid(x, y)
    Z = f(X, Y)
    
    ax1.plot_surface(X, Y, Z, cmap='viridis', alpha=0.7)
    ax1.scatter(x0, y0, f(x0, y0), color='r', s=100, label='Point')
    
    # Add tangent plane
    x_tan = np.linspace(x0-1, x0+1, 20)
    y_tan = np.linspace(y0-1, y0+1, 20)
    X_tan, Y_tan, Z_tan = tangent_plane(f, x0, y0, x_tan, y_tan)
    ax1.plot_surface(X_tan, Y_tan, Z_tan, color='red', alpha=0.3)
    
    ax1.set_xlabel('X')
    ax1.set_ylabel('Y')
    ax1.set_zlabel('Z')
    ax1.set_title('Surface with Tangent Plane')
    
    # Gradient field
    ax2 = fig.add_subplot(132)
    x_grad = np.linspace(-np.pi, np.pi, 20)
    y_grad = np.linspace(-np.pi, np.pi, 20)
    X_grad, Y_grad = np.meshgrid(x_grad, y_grad)
    
    U = np.zeros_like(X_grad)
    V = np.zeros_like(Y_grad)
    for i in range(X_grad.shape[0]):
        for j in range(X_grad.shape[1]):
            grad_vec = compute_gradient(f, X_grad[i,j], Y_grad[i,j])
            U[i,j] = grad_vec[0]
            V[i,j] = grad_vec[1]
    
    ax2.quiver(X_grad, Y_grad, U, V)
    ax2.scatter(x0, y0, color='r', s=100, zorder=5)
    ax2.set_xlabel('X')
    ax2.set_ylabel('Y')
    ax2.set_title('Gradient Field')
    ax2.set_aspect('equal')
    
    # Contour plot
    ax3 = fig.add_subplot(133)
    contour = ax3.contour(X, Y, Z, levels=20)
    ax3.clabel(contour, inline=True, fontsize=8)
    ax3.scatter(x0, y0, color='r', s=100, zorder=5)
    ax3.arrow(x0, y0, grad[0]*0.5, grad[1]*0.5, head_width=0.1, 
              head_length=0.1, fc='r', ec='r')
    ax3.set_xlabel('X')
    ax3.set_ylabel('Y')
    ax3.set_title('Level Curves with Gradient')
    ax3.set_aspect('equal')
    
    plt.tight_layout()
    plt.show()