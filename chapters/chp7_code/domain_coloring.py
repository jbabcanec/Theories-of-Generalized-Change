import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import hsv_to_rgb
from matplotlib.cm import ScalarMappable

def plot_domain_coloring(f, x_min, x_max, y_min, y_max, resolution=400, function_name=None):
    """
    Generate a domain coloring plot for a complex function f(z).

    Parameters:
    - f: The complex function to plot. Must accept a complex array and return a complex array.
    - x_min, x_max: The range for the real part of z.
    - y_min, y_max: The range for the imaginary part of z.
    - resolution: Number of points in each direction for the grid (default 400).
    - function_name: Optional string for the plot title (e.g., "f(z) = z^2").
    """
    # Create the grid
    x = np.linspace(x_min, x_max, resolution)
    y = np.linspace(y_min, y_max, resolution)
    X, Y = np.meshgrid(x, y)
    Z = X + 1j * Y

    # Compute f(Z)
    W = f(Z)

    # Calculate magnitude and argument
    magnitude = np.abs(W)
    argument = np.angle(W)

    # Normalize magnitude for brightness using a logarithmic scale
    magnitude = np.log1p(magnitude)  # log(1 + |f(z)|) to handle wide ranges
    if np.max(magnitude) == np.min(magnitude):  # Handle constant functions
        v = np.ones_like(magnitude)  # Full brightness if magnitude doesn't vary
    else:
        v = (magnitude - np.min(magnitude)) / (np.max(magnitude) - np.min(magnitude))

    # Create HSV image
    h = (argument + np.pi) / (2 * np.pi)  # Hue: map [-π, π] to [0, 1]
    s = np.ones_like(h)                   # Saturation: full
    hsv = np.dstack((h, s, v))

    # Convert to RGB
    rgb = hsv_to_rgb(hsv)

    # Plot the image
    plt.imshow(rgb, extent=(x_min, x_max, y_min, y_max), origin='lower')
    
    # Set title
    if function_name:
        title = f"Domain Coloring of {function_name}\nHue: Argument, Brightness: Magnitude"
    else:
        title = "Domain Coloring of Complex Function\nHue: Argument, Brightness: Magnitude"
    plt.title(title)
    plt.xlabel('Re(z)')
    plt.ylabel('Im(z)')

    # Add colorbar for argument
    sm = ScalarMappable(cmap='hsv', norm=plt.Normalize(-np.pi, np.pi))
    sm.set_array([])
    cbar = plt.colorbar(sm, label='Argument of f(z) (radians)')
    cbar.set_ticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi])
    cbar.set_ticklabels([r'$-\pi$', r'$-\pi/2$', r'$0$', r'$\pi/2$', r'$\pi$'])

    plt.show()

# Example usage
if __name__ == "__main__":
    # Example 1: f(z) = z^2
    f1 = lambda z: z**2
    plot_domain_coloring(f1, -2, 2, -2, 2, function_name=r"$f(z) = z^2$")