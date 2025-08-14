import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import hsv_to_rgb, LinearSegmentedColormap
from matplotlib.patches import Circle
from mpl_toolkits.mplot3d import Axes3D
import warnings
warnings.filterwarnings('ignore')

def complex_phase_plot(f, x_range, y_range, resolution=400, contours=True):
    """Enhanced domain coloring with phase and magnitude contours"""
    x = np.linspace(*x_range, resolution)
    y = np.linspace(*y_range, resolution)
    X, Y = np.meshgrid(x, y)
    Z = X + 1j * Y
    
    # Compute f(z)
    with np.errstate(divide='ignore', invalid='ignore'):
        W = f(Z)
    
    # Calculate magnitude and argument
    magnitude = np.abs(W)
    argument = np.angle(W)
    
    # Enhanced coloring scheme
    h = (argument + np.pi) / (2 * np.pi)
    
    # Use logarithmic scaling for magnitude
    log_mag = np.log1p(magnitude)
    log_mag = np.nan_to_num(log_mag, nan=0, posinf=10, neginf=0)
    
    # Normalize for brightness
    if np.max(log_mag) > np.min(log_mag):
        v = (log_mag - np.min(log_mag)) / (np.max(log_mag) - np.min(log_mag))
    else:
        v = np.ones_like(log_mag)
    
    # Saturation based on magnitude gradient
    s = np.ones_like(h)
    
    # Create HSV image
    hsv = np.dstack((h, s, v))
    rgb = hsv_to_rgb(hsv)
    
    return X, Y, rgb, magnitude, argument

def plot_complex_mapping(f, x_range, y_range, grid_lines=20):
    """Visualize how a complex function maps a grid"""
    # Create grid in domain
    x_lines = np.linspace(*x_range, grid_lines)
    y_lines = np.linspace(*y_range, grid_lines)
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
    
    # Plot original grid
    for x in x_lines:
        y = np.linspace(*y_range, 100)
        ax1.plot(np.full_like(y, x), y, 'b-', alpha=0.5, linewidth=0.5)
    
    for y in y_lines:
        x = np.linspace(*x_range, 100)
        ax1.plot(x, np.full_like(x, y), 'r-', alpha=0.5, linewidth=0.5)
    
    ax1.set_xlabel('Real')
    ax1.set_ylabel('Imaginary')
    ax1.set_title('Original Grid')
    ax1.set_aspect('equal')
    ax1.grid(True, alpha=0.3)
    
    # Plot mapped grid
    for x_val in x_lines:
        y = np.linspace(*y_range, 100)
        z = x_val + 1j * y
        w = f(z)
        ax2.plot(np.real(w), np.imag(w), 'b-', alpha=0.5, linewidth=0.5)
    
    for y_val in y_lines:
        x = np.linspace(*x_range, 100)
        z = x + 1j * y_val
        w = f(z)
        ax2.plot(np.real(w), np.imag(w), 'r-', alpha=0.5, linewidth=0.5)
    
    ax2.set_xlabel('Real')
    ax2.set_ylabel('Imaginary')
    ax2.set_title('Mapped Grid under f(z)')
    ax2.set_aspect('equal')
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    return fig

def plot_poles_and_zeros(f, x_range, y_range, resolution=200):
    """Identify and visualize poles and zeros of a complex function"""
    x = np.linspace(*x_range, resolution)
    y = np.linspace(*y_range, resolution)
    X, Y = np.meshgrid(x, y)
    Z = X + 1j * Y
    
    with np.errstate(divide='ignore', invalid='ignore'):
        W = f(Z)
    
    magnitude = np.abs(W)
    
    # Find approximate zeros (where magnitude is very small)
    threshold_zero = np.percentile(magnitude[np.isfinite(magnitude)], 1)
    zeros_mask = magnitude < threshold_zero
    
    # Find approximate poles (where magnitude is very large)
    threshold_pole = np.percentile(magnitude[np.isfinite(magnitude)], 99)
    poles_mask = magnitude > threshold_pole
    
    return X, Y, magnitude, zeros_mask, poles_mask

def riemann_sphere_projection(f, resolution=50):
    """Project complex function onto Riemann sphere"""
    # Create sphere
    u = np.linspace(0, 2 * np.pi, resolution)
    v = np.linspace(0, np.pi, resolution)
    
    x_sphere = np.outer(np.cos(u), np.sin(v))
    y_sphere = np.outer(np.sin(u), np.sin(v))
    z_sphere = np.outer(np.ones(np.size(u)), np.cos(v))
    
    # Stereographic projection from sphere to complex plane
    Z = (x_sphere + 1j * y_sphere) / (1 - z_sphere + 1e-10)
    
    # Apply function
    W = f(Z)
    
    # Color based on argument of W
    argument = np.angle(W)
    colors = (argument + np.pi) / (2 * np.pi)
    
    return x_sphere, y_sphere, z_sphere, colors

def animate_complex_function(f, param_range, x_range, y_range):
    """Create animation showing how function changes with parameter"""
    # This would create an animation - simplified here for static display
    params = np.linspace(*param_range, 4)
    
    fig, axes = plt.subplots(2, 2, figsize=(12, 12))
    axes = axes.flatten()
    
    for idx, param in enumerate(params):
        f_param = lambda z: f(z, param)
        X, Y, rgb, _, _ = complex_phase_plot(f_param, x_range, y_range, resolution=200)
        
        axes[idx].imshow(rgb, extent=[*x_range, *y_range], origin='lower')
        axes[idx].set_title(f'Parameter = {param:.2f}')
        axes[idx].set_xlabel('Real')
        axes[idx].set_ylabel('Imaginary')
    
    plt.tight_layout()
    return fig

# Example usage
if __name__ == "__main__":
    # Define some interesting complex functions
    f1 = lambda z: z**3 - 1  # Roots of unity
    f2 = lambda z: (z - 1) / (z + 1)  # Möbius transformation
    f3 = lambda z: np.sin(z)  # Complex sine
    f4 = lambda z: 1 / (z**2 + 1)  # Rational function with poles
    
    # Create comprehensive visualization
    fig = plt.figure(figsize=(20, 12))
    
    # Function 1: z^3 - 1 with enhanced domain coloring
    ax1 = fig.add_subplot(3, 4, 1)
    X, Y, rgb, mag, arg = complex_phase_plot(f1, (-2, 2), (-2, 2))
    ax1.imshow(rgb, extent=[-2, 2, -2, 2], origin='lower')
    ax1.set_title('$f(z) = z^3 - 1$')
    ax1.set_xlabel('Real')
    ax1.set_ylabel('Imaginary')
    
    # Add contours
    ax2 = fig.add_subplot(3, 4, 2)
    contour_mag = ax2.contour(X, Y, mag, levels=20, colors='white', alpha=0.5, linewidths=0.5)
    contour_arg = ax2.contourf(X, Y, arg, levels=20, cmap='hsv')
    plt.colorbar(contour_arg, ax=ax2, label='Argument')
    ax2.set_title('Phase with Magnitude Contours')
    ax2.set_xlabel('Real')
    ax2.set_ylabel('Imaginary')
    
    # Möbius transformation
    ax3 = fig.add_subplot(3, 4, 3)
    X2, Y2, rgb2, _, _ = complex_phase_plot(f2, (-3, 3), (-3, 3))
    ax3.imshow(rgb2, extent=[-3, 3, -3, 3], origin='lower')
    ax3.set_title('Möbius: $(z-1)/(z+1)$')
    ax3.set_xlabel('Real')
    ax3.set_ylabel('Imaginary')
    
    # Complex sine
    ax4 = fig.add_subplot(3, 4, 4)
    X3, Y3, rgb3, _, _ = complex_phase_plot(f3, (-4, 4), (-4, 4))
    ax4.imshow(rgb3, extent=[-4, 4, -4, 4], origin='lower')
    ax4.set_title('$f(z) = \\sin(z)$')
    ax4.set_xlabel('Real')
    ax4.set_ylabel('Imaginary')
    
    # Grid mapping for Möbius
    plot_complex_mapping(f2, (-2, 2), (-2, 2))
    
    # Poles and zeros visualization
    ax5 = fig.add_subplot(3, 4, 5, projection='3d')
    X4, Y4, magnitude, zeros, poles = plot_poles_and_zeros(f4, (-3, 3), (-3, 3))
    
    # Plot magnitude surface
    surf = ax5.plot_surface(X4, Y4, np.log1p(magnitude), cmap='viridis', alpha=0.8)
    ax5.set_xlabel('Real')
    ax5.set_ylabel('Imaginary')
    ax5.set_zlabel('log(|f(z)|)')
    ax5.set_title('$f(z) = 1/(z^2+1)$ Magnitude')
    
    # Mark poles
    pole_points = np.where(poles)
    if len(pole_points[0]) > 0:
        ax5.scatter(X4[pole_points], Y4[pole_points], 
                   np.log1p(magnitude[pole_points]), 
                   color='red', s=50, label='Poles')
    
    # Riemann sphere projection
    ax6 = fig.add_subplot(3, 4, 6, projection='3d')
    x_s, y_s, z_s, colors = riemann_sphere_projection(lambda z: z**2, 30)
    
    # Create custom colormap
    cmap = plt.cm.hsv
    facecolors = cmap(colors)
    
    ax6.plot_surface(x_s, y_s, z_s, facecolors=facecolors, alpha=0.9)
    ax6.set_xlabel('X')
    ax6.set_ylabel('Y')
    ax6.set_zlabel('Z')
    ax6.set_title('Riemann Sphere: $z^2$')
    ax6.set_box_aspect([1,1,1])
    
    # Complex exponential
    ax7 = fig.add_subplot(3, 4, 7)
    f_exp = lambda z: np.exp(z)
    X5, Y5, rgb5, _, _ = complex_phase_plot(f_exp, (-2, 2), (-2*np.pi, 2*np.pi))
    ax7.imshow(rgb5, extent=[-2, 2, -2*np.pi, 2*np.pi], origin='lower')
    ax7.set_title('$f(z) = e^z$')
    ax7.set_xlabel('Real')
    ax7.set_ylabel('Imaginary')
    
    # Complex logarithm (principal branch)
    ax8 = fig.add_subplot(3, 4, 8)
    f_log = lambda z: np.log(z + 0.01j)  # Small imaginary shift to avoid branch cut
    X6, Y6, rgb6, _, _ = complex_phase_plot(f_log, (-3, 3), (-3, 3))
    ax8.imshow(rgb6, extent=[-3, 3, -3, 3], origin='lower')
    ax8.set_title('$f(z) = \\log(z)$ (principal)')
    ax8.set_xlabel('Real')
    ax8.set_ylabel('Imaginary')
    
    # Rational function with multiple poles
    ax9 = fig.add_subplot(3, 4, 9)
    f_rational = lambda z: 1 / ((z - 1) * (z + 1) * (z - 1j) * (z + 1j))
    X7, Y7, rgb7, _, _ = complex_phase_plot(f_rational, (-2, 2), (-2, 2))
    ax9.imshow(rgb7, extent=[-2, 2, -2, 2], origin='lower')
    ax9.set_title('Multiple Poles')
    ax9.set_xlabel('Real')
    ax9.set_ylabel('Imaginary')
    
    # Essential singularity
    ax10 = fig.add_subplot(3, 4, 10)
    f_essential = lambda z: np.exp(1/z) if z != 0 else np.inf
    f_essential_vec = np.vectorize(f_essential)
    X8, Y8, rgb8, _, _ = complex_phase_plot(f_essential_vec, (-1, 1), (-1, 1), resolution=300)
    ax10.imshow(rgb8, extent=[-1, 1, -1, 1], origin='lower')
    ax10.set_title('$f(z) = e^{1/z}$ (Essential Sing.)')
    ax10.set_xlabel('Real')
    ax10.set_ylabel('Imaginary')
    
    # Jacobi theta function approximation
    ax11 = fig.add_subplot(3, 4, 11)
    def theta_approx(z, q=0.1, n_terms=10):
        result = 0
        for n in range(-n_terms, n_terms+1):
            result += q**(n**2) * np.exp(2*np.pi*1j*n*z)
        return result
    
    X9, Y9, rgb9, _, _ = complex_phase_plot(lambda z: theta_approx(z), (-2, 2), (-2, 2))
    ax11.imshow(rgb9, extent=[-2, 2, -2, 2], origin='lower')
    ax11.set_title('Theta Function (approx)')
    ax11.set_xlabel('Real')
    ax11.set_ylabel('Imaginary')
    
    # Composite function
    ax12 = fig.add_subplot(3, 4, 12)
    f_composite = lambda z: np.sin(1/z) if z != 0 else 0
    f_composite_vec = np.vectorize(f_composite)
    X10, Y10, rgb10, _, _ = complex_phase_plot(f_composite_vec, (-1, 1), (-1, 1), resolution=300)
    ax12.imshow(rgb10, extent=[-1, 1, -1, 1], origin='lower')
    ax12.set_title('$f(z) = \\sin(1/z)$')
    ax12.set_xlabel('Real')
    ax12.set_ylabel('Imaginary')
    
    plt.suptitle('Complex Function Visualization Gallery', fontsize=16)
    plt.tight_layout()
    plt.show()
    
    # Additional grid mapping visualization
    print("Creating grid mapping visualizations...")
    plot_complex_mapping(lambda z: z**2, (-2, 2), (-2, 2))
    plt.suptitle('Grid Mapping: $f(z) = z^2$')
    plt.show()
    
    plot_complex_mapping(np.sin, (-np.pi, np.pi), (-2, 2))
    plt.suptitle('Grid Mapping: $f(z) = \\sin(z)$')
    plt.show()