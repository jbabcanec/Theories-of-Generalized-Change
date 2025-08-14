import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy import integrate

def monte_carlo_integration_2d(f, x_bounds, y_bounds, n_samples=10000):
    """Monte Carlo integration for 2D functions"""
    x_min, x_max = x_bounds
    y_min, y_max = y_bounds
    
    # Random sampling
    x_samples = np.random.uniform(x_min, x_max, n_samples)
    y_samples = np.random.uniform(y_min, y_max, n_samples)
    
    # Evaluate function
    f_values = f(x_samples, y_samples)
    
    # Calculate integral
    area = (x_max - x_min) * (y_max - y_min)
    integral = area * np.mean(f_values)
    std_error = area * np.std(f_values) / np.sqrt(n_samples)
    
    return integral, std_error

def polar_integration(f_polar, r_bounds, theta_bounds, n_r=100, n_theta=100):
    """Integrate function in polar coordinates"""
    r_min, r_max = r_bounds
    theta_min, theta_max = theta_bounds
    
    r = np.linspace(r_min, r_max, n_r)
    theta = np.linspace(theta_min, theta_max, n_theta)
    R, Theta = np.meshgrid(r, theta)
    
    # Convert to Cartesian for visualization
    X = R * np.cos(Theta)
    Y = R * np.sin(Theta)
    Z = f_polar(R, Theta)
    
    # Numerical integration using trapezoidal rule
    # Remember to include the Jacobian r for polar coordinates
    integrand = Z * R  # f(r,θ) * r
    integral = np.trapz(np.trapz(integrand, r, axis=1), theta, axis=0)
    
    return integral, X, Y, Z

def center_of_mass_2d(density_func, x_bounds, y_bounds):
    """Calculate center of mass for 2D region with given density"""
    # Mass
    mass, _ = integrate.dblquad(density_func, *y_bounds, *x_bounds)
    
    # Moments
    m_x = lambda y, x: x * density_func(x, y)
    m_y = lambda y, x: y * density_func(x, y)
    
    moment_x, _ = integrate.dblquad(m_x, *y_bounds, *x_bounds)
    moment_y, _ = integrate.dblquad(m_y, *y_bounds, *x_bounds)
    
    x_cm = moment_x / mass
    y_cm = moment_y / mass
    
    return x_cm, y_cm, mass

def visualize_solid_of_revolution(f, x_bounds, axis='x'):
    """Visualize a solid of revolution"""
    x = np.linspace(*x_bounds, 100)
    theta = np.linspace(0, 2*np.pi, 100)
    X_grid, Theta = np.meshgrid(x, theta)
    
    if axis == 'x':
        # Revolve around x-axis
        R = f(X_grid)
        Y = R * np.cos(Theta)
        Z = R * np.sin(Theta)
        X = X_grid
    else:  # axis == 'y'
        # Revolve around y-axis
        R = X_grid
        X = R * np.cos(Theta)
        Z = R * np.sin(Theta)
        Y = f(R)
    
    return X, Y, Z

# Example usage
if __name__ == "__main__":
    # Example 1: Double integral
    f = lambda x, y: np.exp(-(x**2 + y**2))
    
    # Monte Carlo integration
    mc_result, mc_error = monte_carlo_integration_2d(f, (-2, 2), (-2, 2), 50000)
    
    # Scipy integration for comparison
    scipy_result, _ = integrate.dblquad(f, -2, 2, -2, 2)
    
    print("Double Integral of exp(-(x²+y²)) over [-2,2]×[-2,2]:")
    print(f"Monte Carlo: {mc_result:.6f} ± {mc_error:.6f}")
    print(f"Scipy (exact): {scipy_result:.6f}")
    print(f"Difference: {abs(mc_result - scipy_result):.6f}")
    
    # Example 2: Polar integration
    f_polar = lambda r, theta: r * np.exp(-r**2)
    polar_result, X_polar, Y_polar, Z_polar = polar_integration(
        f_polar, (0, 3), (0, 2*np.pi)
    )
    print(f"\nPolar integral result: {polar_result:.6f}")
    
    # Example 3: Center of mass
    density = lambda x, y: 2 - x - y if x >= 0 and y >= 0 and x + y <= 1 else 0
    x_cm, y_cm, mass = center_of_mass_2d(density, (0, 1), (0, lambda x: 1-x))
    print(f"\nCenter of mass: ({x_cm:.3f}, {y_cm:.3f})")
    print(f"Total mass: {mass:.3f}")
    
    # Visualization
    fig = plt.figure(figsize=(15, 5))
    
    # Plot 1: Function surface
    ax1 = fig.add_subplot(131, projection='3d')
    x = np.linspace(-2, 2, 100)
    y = np.linspace(-2, 2, 100)
    X, Y = np.meshgrid(x, y)
    Z = f(X, Y)
    ax1.plot_surface(X, Y, Z, cmap='viridis', alpha=0.8)
    ax1.set_title('Gaussian Function')
    ax1.set_xlabel('X')
    ax1.set_ylabel('Y')
    ax1.set_zlabel('Z')
    
    # Plot 2: Polar function
    ax2 = fig.add_subplot(132, projection='3d')
    ax2.plot_surface(X_polar, Y_polar, Z_polar, cmap='coolwarm', alpha=0.8)
    ax2.set_title('Function in Polar Coordinates')
    ax2.set_xlabel('X')
    ax2.set_ylabel('Y')
    ax2.set_zlabel('Z')
    
    # Plot 3: Solid of revolution
    ax3 = fig.add_subplot(133, projection='3d')
    f_rev = lambda x: np.sqrt(1 - (x-1)**2) if abs(x-1) <= 1 else 0
    f_rev_vec = np.vectorize(f_rev)
    X_rev, Y_rev, Z_rev = visualize_solid_of_revolution(f_rev_vec, (0, 2))
    ax3.plot_surface(X_rev, Y_rev, Z_rev, cmap='plasma', alpha=0.9)
    ax3.set_title('Solid of Revolution (Sphere)')
    ax3.set_xlabel('X')
    ax3.set_ylabel('Y')
    ax3.set_zlabel('Z')
    ax3.set_box_aspect([1,1,1])
    
    plt.tight_layout()
    plt.show()